/*
 * texture.c
 *
 *  Created on: 11/04/2011
 *      Author: francisco.prates
 */

#include <stdio.h>
#include <png.h>
#include <setjmp.h>
#include <faclib.h>
#include <math.h>
#include <jpeglib.h>
#include "../ilusia.h"

struct ils_texture {
    char *name;
    char *path;
    unsigned int id;
    unsigned int w;
    unsigned int h;
    float r;
    float g;
    float b;
    float a;
};

struct s_link {
    struct ils_obj *obj;
    struct ils_texture *texture;
};

struct s_image {
    char *path;
    unsigned int n_comp;
    unsigned int w;
    unsigned int h;
    void *data;
};

struct s_jpeg_err_manager {
	struct jpeg_error_mgr pub;
	jmp_buf jmpbuf;
};

static struct s_global {
	struct fac_lista *textures;
	struct s_jpeg_err_manager jpeg_err;
} global;

void ils_ini_textures(void)
{
    global.textures = fac_ini_lista();
}

struct ils_texture *ils_texture_inc(char *id, char *path)
{
    struct ils_texture *texture = malloc(sizeof(*texture));

    texture->name = id;
    texture->path = path;
    texture->r = 1.0f;
    texture->g = 1.0f;
    texture->b = 1.0f;
    texture->a = 1.0f;
    texture->id = 0;
    fac_inc_item(global.textures, texture);

    return texture;
}

static char def_texture(char *name, struct s_image *image)
{
    GLenum formato;
    struct fac_iterador *it;
    struct ils_texture *texture = NULL;
    struct ils_gl *gl = ils_ret_gl_fncs();

    if (image->data == NULL)
        return 1;

    it = fac_ini_iterador(global.textures);
    while (fac_existe_prox(it)) {
        texture = fac_proximo(it);
        if (strcmp(texture->name, name) == 0)
            break;

        texture = NULL;
    }

    fac_rm_iterador(it);

    if (texture == NULL)
        return 2;

    gl->glGenTextures(1, &texture->id);
    gl->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    gl->glBindTexture(GL_TEXTURE_2D, texture->id);

    formato = (image->n_comp == 1)?GL_LUMINANCE:GL_RGB;

    texture->w = image->w;
    texture->h = image->h;

    gl->gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texture->w, texture->h,
            formato, GL_UNSIGNED_BYTE, image->data);
    gl->glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gl->glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    gl->glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    gl->glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    gl->glEnable(GL_TEXTURE_GEN_S);
    gl->glEnable(GL_TEXTURE_GEN_T);

    free(image->data);

    return 0;
}

static void jpeg_error_exit_handler(j_common_ptr cinfo)
{
	jpeg_destroy(cinfo);
	longjmp(global.jpeg_err.jmpbuf, 1);
}

static int load_png(FILE *fd, struct s_image *image)
{
	const unsigned int PNG_ID_SIZE = 8;
	png_byte pngid;
	png_structp png_ptr;
	png_infop info_ptr;
	png_infop end_info;
	png_bytep *row_ptrs;
	png_uint_32 bitdep;
	png_uint_32 color_type;
	png_uint_32 channels;
	unsigned int i;
	unsigned int q;
	unsigned int stride;

	if (fread(&pngid, 1, PNG_ID_SIZE, fd) == 0)
		return 0;

	if (png_sig_cmp(&pngid, 0, PNG_ID_SIZE))
		return 0;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		return 0;

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}

	end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return 0;
	}

	png_init_io(png_ptr, fd);
	png_set_sig_bytes(png_ptr, PNG_ID_SIZE);
	png_read_info(png_ptr, info_ptr);

	image->h = png_get_image_height(png_ptr, info_ptr);
	image->w = png_get_image_width(png_ptr, info_ptr);
    bitdep = png_get_bit_depth(png_ptr, info_ptr);
    channels = png_get_channels(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);

	switch (color_type) {
    case PNG_COLOR_TYPE_PALETTE:
        png_set_palette_to_rgb(png_ptr);

        channels = 3;
        image->n_comp = 0;

        break;

    case PNG_COLOR_TYPE_GRAY:
        if (bitdep < 8)
        	png_set_gray_1_2_4_to_8(png_ptr);

        bitdep = 8;
        image->n_comp = 1;

        break;
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
		channels++;
	}

	if (bitdep == 16)
        png_set_strip_16(png_ptr);

    row_ptrs = malloc(sizeof(png_bytep) * image->h);
    image->data = malloc(image->w * image->h * bitdep * channels / 8);
    stride = image->w * bitdep * channels / 8;

    for (i = 0; i < image->h; i++) {
    	q = (image->h - i - 1) * stride;
    	row_ptrs[i] = image->data + q;
    }

	png_read_image(png_ptr, row_ptrs);
	free(row_ptrs);
	png_destroy_read_struct(&png_ptr, &info_ptr,0);

	return 1;
}

static int load_jpeg(FILE *fd, struct s_image *image)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr err;
    unsigned int row_span;
    unsigned int rows_read;
    JSAMPARRAY buffer;

    cinfo.err = jpeg_std_error(&err);
    cinfo.err->error_exit = &jpeg_error_exit_handler;

    jpeg_create_decompress(&cinfo);

	if (setjmp(global.jpeg_err.jmpbuf)) {
		if (image->data != NULL) {
			free(image->data);
			image->data = NULL;
		}

		return 0;
	}

    jpeg_stdio_src(&cinfo, fd);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    image->n_comp = cinfo.output_components;
    image->w = cinfo.output_width;
    image->h = cinfo.output_height;

    row_span = image->n_comp * image->w;
    image->data = malloc(row_span * image->h);

    buffer = (*cinfo.mem->alloc_sarray)
            ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_span, 1);

    rows_read = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(image->data + rows_read, buffer[0], row_span);
        rows_read += row_span;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return 1;
}

static void image_load(struct s_image *image)
{
    FILE *fd = NULL;

    image->data = NULL;

    fd = fopen(image->path, "rb");

    if (fd == NULL)
        return;

    if (load_png(fd, image)) {
    	fclose(fd);
    	return;
    }

    rewind(fd);
    load_jpeg(fd, image);

    fclose(fd);
}

void ils_def_img(void)
{
    struct ils_texture *texture;
    struct fac_iterador *it = fac_ini_iterador(global.textures);
    struct s_image image;

    while (fac_existe_prox(it)) {
        texture = fac_proximo(it);
        image.path = texture->path;
        image_load(&image);

        if (image.data == NULL) {
            printf("w: imagem \"%s\" não encontrada.\n", texture->name);
            return;
        }

        switch (def_texture(texture->name, &image)) {
        case 0:
            printf("i: textura \"%s\" carregada com sucesso.\n",
                    texture->name);
            break;
        case 1:
            printf("w: dados de imagem inválidos.\n");
            break;

        case 2:
            printf("w: textura informada não encontrada.\n");
            break;

        default:
            printf("w: erro de carga de textura não identificado.\n");
            break;
        }
    }
}

void ils_show_texture(struct ils_texture *texture, struct ils_pos *pos,
		unsigned int mode)
{
    struct ils_gl *gl = ils_ret_gl_fncs();

    if (texture->id > 0) {
		gl->glEnable(GL_TEXTURE_2D);
		gl->glBindTexture(GL_TEXTURE_2D, texture->id);
    }

    gl->glPushMatrix();
    if ((mode == INVERT) && (texture->id > 0))
    	gl->glRotatef(180, 1, 0, 0);

    gl->glColor4f(texture->r, texture->g, texture->b, texture->a);
    gl->glTranslatef(pos->x, pos->y, pos->z);
    gl->glScalef(pos->sw, pos->sh, 1);
    gl->glBegin(GL_QUADS);
        gl->glVertex3f(0, 0, 0);
        gl->glVertex3f(pos->dw, 0, 0);
        gl->glVertex3f(pos->dw, pos->dh, 0);
        gl->glVertex3f(0, pos->dh, 0);
    gl->glEnd();
    gl->glPopMatrix();

    if (texture->id > 0)
    	gl->glDisable(GL_TEXTURE_2D);
}
