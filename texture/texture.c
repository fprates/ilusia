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
#include <jpeglib.h>
#include "texture.h"

#ifdef __linux__
#define ILS_JPEG_32 "/usr/lib/libjpeg.so"
#define ILS_JPEG_64 "/usr/lib64/libjpeg.so"
#define ILS_PNG "libpng.so"
#endif

struct ils_texture {
    char *name, *path;
    unsigned int id, w, h;
    float r, g, b, a;
};

struct s_link {
    struct ils_obj *obj;
    struct ils_texture *texture;
};

struct s_image {
    char *path;
    enum e_tpcolor tpcolor;
    unsigned int w;
    unsigned int h;
    void *data;
};

struct s_jpeg_err_manager {
	struct jpeg_error_mgr pub;
	jmp_buf jmpbuf;
};

struct s_jpeg {
    void(*jpeg_destroy)JPP((j_common_ptr));
    struct jpeg_error_mgr *(*jpeg_std_error)JPP((struct jpeg_error_mgr *));
    void(*jpeg_CreateDecompress)JPP((j_decompress_ptr, int, size_t));
    void(*jpeg_stdio_src) JPP((j_decompress_ptr, FILE *));
    int(*jpeg_read_header)JPP((j_decompress_ptr, boolean));
    boolean(*jpeg_start_decompress)JPP((j_decompress_ptr));
    JDIMENSION(*jpeg_read_scanlines)JPP(
            (j_decompress_ptr, JSAMPARRAY, JDIMENSION));
    boolean(*jpeg_finish_decompress)JPP((j_decompress_ptr));
    void(*jpeg_destroy_decompress)JPP((j_decompress_ptr));
};

struct s_png {
    int (*png_sig_cmp)(png_bytep, png_size_t, png_size_t);
    png_structp (*png_create_read_struct)(
            png_const_charp, png_voidp, png_error_ptr, png_error_ptr);
    png_infop (*png_create_info_struct)(png_structp);
    void (*png_destroy_read_struct)(png_structpp, png_infopp, png_infopp);
    void (*png_init_io)(png_structp, png_FILE_p);
    void (*png_set_sig_bytes)(png_structp, int);
    void (*png_read_info)(png_structp, png_infop);
    png_uint_32 (*png_get_image_height)(png_structp, png_infop);
    png_uint_32 (*png_get_image_width)(png_structp, png_infop);
    png_uint_32 (*png_get_bit_depth)(png_structp, png_infop);
    png_uint_32 (*png_get_channels)(png_structp, png_infop);
    png_uint_32 (*png_get_color_type)(png_structp, png_infop);
    void (*png_set_palette_to_rgb)(png_structp);
    void (*png_set_tRNS_to_alpha)(png_structp);
    png_uint_32 (*png_get_valid)(png_structp, png_infop, png_uint_32);
    void (*png_set_strip_16)(png_structp);
    void (*png_read_image)(png_structp, png_bytepp);
};

static struct s_global {
	void *jpeg_lib;
	struct s_jpeg jpeg;
	void *png_lib;
	struct s_png png;
	struct fac_lista *textures;
	struct s_jpeg_err_manager jpeg_err;
} global;

void _ini_textures(void)
{
    struct s_jpeg *jpeg = &global.jpeg;
    struct s_png *png = &global.png;

    global.textures = fac_ini_lista();

    /*
     * dlopen intercepta incialmente libjpeg da jre.
     * especificamos, então, caminhos absolutos.
     */
    global.jpeg_lib = fac_ld_lib(ILS_JPEG_64);
    if (global.jpeg_lib == NULL)
    	global.jpeg_lib = fac_ld_lib(ILS_JPEG_32);

    if (global.jpeg_lib != NULL) {
        jpeg->jpeg_destroy = fac_ret_proc_lib(global.jpeg_lib, "jpeg_destroy");
        jpeg->jpeg_std_error = fac_ret_proc_lib(
                global.jpeg_lib, "jpeg_std_error");
        jpeg->jpeg_CreateDecompress = fac_ret_proc_lib(
                global.jpeg_lib, "jpeg_CreateDecompress");
        jpeg->jpeg_stdio_src = fac_ret_proc_lib(
                global.jpeg_lib, "jpeg_stdio_src");
        jpeg->jpeg_read_header = fac_ret_proc_lib(
                global.jpeg_lib, "jpeg_read_header");
        jpeg->jpeg_start_decompress = fac_ret_proc_lib(
                global.jpeg_lib, "jpeg_start_decompress");
        jpeg->jpeg_read_scanlines = fac_ret_proc_lib(
                global.jpeg_lib, "jpeg_read_scanlines");
        jpeg->jpeg_finish_decompress = fac_ret_proc_lib(
                global.jpeg_lib, "jpeg_finish_decompress");
        jpeg->jpeg_destroy_decompress = fac_ret_proc_lib(
                global.jpeg_lib, "jpeg_destroy_decompress");
    }

    global.png_lib = fac_ld_lib(ILS_PNG);

    if (global.png_lib != NULL) {
        png->png_sig_cmp = fac_ret_proc_lib(global.png_lib, "png_sig_cmp");
        png->png_create_read_struct = fac_ret_proc_lib(
                global.png_lib, "png_create_read_struct");
        png->png_create_info_struct = fac_ret_proc_lib(
                global.png_lib, "png_create_info_struct");
        png->png_destroy_read_struct = fac_ret_proc_lib(
                global.png_lib, "png_destroy_read_struct");
        png->png_init_io = fac_ret_proc_lib(global.png_lib, "png_init_io");
        png->png_set_sig_bytes = fac_ret_proc_lib(
                global.png_lib, "png_set_sig_bytes");
        png->png_read_info = fac_ret_proc_lib(
                global.png_lib, "png_read_info");
        png->png_get_image_height = fac_ret_proc_lib(
                global.png_lib, "png_get_image_height");
        png->png_get_image_width = fac_ret_proc_lib(
                global.png_lib, "png_get_image_width");
        png->png_get_bit_depth = fac_ret_proc_lib(
                global.png_lib, "png_get_bit_depth");
        png->png_get_channels = fac_ret_proc_lib(
                global.png_lib, "png_get_channels");
        png->png_get_color_type = fac_ret_proc_lib(
                global.png_lib, "png_get_color_type");
        png->png_set_palette_to_rgb = fac_ret_proc_lib(
                global.png_lib, "png_set_palette_to_rgb");
        png->png_set_tRNS_to_alpha = fac_ret_proc_lib(
                global.png_lib, "png_set_tRNS_to_alpha");
        png->png_get_valid = fac_ret_proc_lib(
                global.png_lib, "png_get_valid");
        png->png_set_strip_16 = fac_ret_proc_lib(
                global.png_lib, "png_set_strip_16");
        png->png_read_image = fac_ret_proc_lib(
                global.png_lib, "png_read_image");
    }
}

void _term_textures(void)
{
	if (global.jpeg_lib != NULL)
		 fac_lib_term(global.jpeg_lib);

	if (global.png_lib != NULL)
		fac_lib_term(global.png_lib);

	fac_rm_lista(global.textures);
	global.textures = NULL;
}

struct ils_texture *_def_texture(char *id)
{
    struct ils_texture *texture = malloc(sizeof(*texture));

    texture->name = id;
    texture->path = NULL;
    texture->r = 1.0f;
    texture->g = 1.0f;
    texture->b = 1.0f;
    texture->a = 1.0f;
    texture->id = 0;

    return texture;
}

struct ils_texture *ils_texture_inc(char *id, char *path)
{
    struct ils_texture *texture = _def_texture(id);

    texture->path = path;
    fac_inc_item(global.textures, texture);

    return texture;
}

void _rm_texture(struct ils_texture *texture)
{
    struct ils_gl *gl = ils_ret_gl_fncs();

    if (texture->id == 0)
        return;

    gl->glDeleteTextures(1, &texture->id);
    texture->id = 0;
}

void _load_texture(
        struct ils_texture *texture, enum e_tpcolor comp, void *data)
{
    GLenum formato;
    struct ils_gl *gl = ils_ret_gl_fncs();

    gl->glGenTextures(1, &texture->id);
    gl->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    gl->glBindTexture(GL_TEXTURE_2D, texture->id);

    switch (comp) {
    case ILS_RGB:
    	formato = GL_RGB;
    	break;
    case ILS_LUMINANCE:
    	formato = GL_LUMINANCE;
    	break;
    case ILS_BGRA:
    	formato = GL_BGRA;
    	break;
    }

    gl->gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texture->w, texture->h,
            formato, GL_UNSIGNED_BYTE, data);
    gl->glTexParameteri (
            GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gl->glTexParameteri (
            GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    gl->glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    gl->glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    gl->glEnable(GL_TEXTURE_GEN_S);
    gl->glEnable(GL_TEXTURE_GEN_T);
}

static char def_texture(char *name, struct s_image *image)
{
    struct fac_iterador *it;
    struct ils_texture *texture = NULL;

    if (image->data == NULL)
        return 1;

    it = fac_ini_iterador(global.textures);
    while (fac_existe_prox(it)) {
        texture = fac_proximo(it);

        if (strcmp(texture->name, name) == 0) {
            texture->w = image->w;
            texture->h = image->h;
            _load_texture(texture, image->tpcolor, image->data);
            break;
        }

        texture = NULL;
    }

    fac_rm_iterador(it);

    free(image->data);

    return 0;
}

static void jpeg_error_exit_handler(j_common_ptr cinfo)
{
	global.jpeg.jpeg_destroy(cinfo);
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
	struct s_png *png = &global.png;

	if (fread(&pngid, 1, PNG_ID_SIZE, fd) == 0)
		return 0;

	if (png->png_sig_cmp(&pngid, 0, PNG_ID_SIZE))
		return 0;

	png_ptr = png->png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		return 0;

	info_ptr = png->png_create_info_struct(png_ptr);
	if (!info_ptr) {
	    png->png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}

	end_info = png->png_create_info_struct(png_ptr);
	if (!end_info) {
	    png->png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return 0;
	}

	png->png_init_io(png_ptr, fd);
	png->png_set_sig_bytes(png_ptr, PNG_ID_SIZE);
	png->png_read_info(png_ptr, info_ptr);

	image->h = png->png_get_image_height(png_ptr, info_ptr);
	image->w = png->png_get_image_width(png_ptr, info_ptr);
    bitdep = png->png_get_bit_depth(png_ptr, info_ptr);
    channels = png->png_get_channels(png_ptr, info_ptr);
    color_type = png->png_get_color_type(png_ptr, info_ptr);

	switch (color_type) {
    case PNG_COLOR_TYPE_PALETTE:
        png->png_set_palette_to_rgb(png_ptr);

        channels = 3;
        image->tpcolor = ILS_RGB;

        break;

    case PNG_COLOR_TYPE_GRAY:
        bitdep = 8;
        image->tpcolor = ILS_LUMINANCE;

        break;

    default:
        image->tpcolor = ILS_RGB;

        break;
	}

	if (png->png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
	    png->png_set_tRNS_to_alpha(png_ptr);
		channels++;
	}

	if (bitdep == 16)
	    png->png_set_strip_16(png_ptr);

    row_ptrs = malloc(sizeof(png_bytep) * image->h);
    image->data = malloc(image->w * image->h * bitdep * channels / 8);
    stride = image->w * bitdep * channels / 8;

    for (i = 0; i < image->h; i++) {
    	q = (image->h - i - 1) * stride;
    	row_ptrs[i] = image->data + q;
    }

    png->png_read_image(png_ptr, row_ptrs);
	free(row_ptrs);
	png->png_destroy_read_struct(&png_ptr, &info_ptr,0);

	return 1;
}

static int load_jpeg(FILE *fd, struct s_image *image)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr err;
    unsigned int row_span;
    unsigned int rows_read;
    JSAMPARRAY buffer;
    struct s_jpeg *jpeg = &global.jpeg;

    cinfo.err = jpeg->jpeg_std_error(&err);
    cinfo.err->error_exit = &jpeg_error_exit_handler;

    jpeg->jpeg_create_decompress(&cinfo);

	if (setjmp(global.jpeg_err.jmpbuf)) {
		if (image->data != NULL) {
			free(image->data);
			image->data = NULL;
		}

		return 0;
	}

	jpeg->jpeg_stdio_src(&cinfo, fd);
	jpeg->jpeg_read_header(&cinfo, TRUE);
	jpeg->jpeg_start_decompress(&cinfo);

    image->tpcolor = (cinfo.output_components == 1)?ILS_LUMINANCE:ILS_RGB;
    image->w = cinfo.output_width;
    image->h = cinfo.output_height;

    row_span = cinfo.output_components * image->w;
    image->data = malloc(row_span * image->h);

    buffer = (*cinfo.mem->alloc_sarray)
            ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_span, 1);

    rows_read = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg->jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(image->data + rows_read, buffer[0], row_span);
        rows_read += row_span;
    }

    jpeg->jpeg_finish_decompress(&cinfo);
    jpeg->jpeg_destroy_decompress(&cinfo);

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

void _def_img(void)
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
    gl->glScalef(pos->sw, pos->sh, pos->sd);
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

void _set_texture(
        struct ils_texture *texture, unsigned int w, unsigned int h)
{
    texture->w = w;
    texture->h = h;
}
