/*
 * texture.c
 *
 *  Created on: 11/04/2011
 *      Author: francisco.prates
 */

#include <stdio.h>
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
    unsigned char *data;
};

static struct fac_lista *textures;

void ils_ini_textures(void)
{
    textures = fac_ini_lista();
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
    fac_inc_item(textures, texture);

    return texture;
}

static unsigned int next_power_of_2(unsigned int x)
{
    double logbase2 = log(x)/log(2);

    return round(pow(2,ceil(logbase2)));
}

static char def_texture(char *name, struct s_image *image)
{
    GLenum formato;
    struct fac_iterador *it;
    struct ils_texture *texture = NULL;
    struct ils_gl *gl = ils_ret_gl_fncs();

    if (image->data == NULL)
        return 1;

    it = fac_ini_iterador(textures);
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

static void image_load(struct s_image *image)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr err;
    unsigned int row_span;
    unsigned int rows_read;
    JSAMPARRAY buffer;
    FILE *fd = fopen(image->path, "rb");

    image->data = NULL;

    if (fd == NULL)
        return;

    cinfo.err = jpeg_std_error(&err);

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fd);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    image->n_comp = cinfo.output_components;
    image->w = next_power_of_2(cinfo.output_width);
    image->h = next_power_of_2(cinfo.output_height);

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
    fclose(fd);
}

void ils_def_img(void)
{
    struct ils_texture *texture;
    struct fac_iterador *it = fac_ini_iterador(textures);
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

    gl->glEnable(GL_TEXTURE_2D);
    gl->glBindTexture(GL_TEXTURE_2D, texture->id);

    gl->glPushMatrix();
    if (mode == INVERT)
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

    gl->glDisable(GL_TEXTURE_2D);
}
