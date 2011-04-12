/*
 * texture.c
 *
 *  Created on: 11/04/2011
 *      Author: francisco.prates
 */

#include <stdio.h>
#include <faclib.h>
#include <math.h>
#include "../ilusia.h"

struct ils_texture {
    char *name;
    char *path;
    unsigned int id;
    unsigned int l;
    unsigned int a;
};

struct s_link {
    struct ils_obj *obj;
    struct ils_texture *texture;
};

struct fac_lista *textures;
struct fac_lista *links;

void ils_ini_textures(void)
{
    textures = fac_ini_lista();
    links = fac_ini_lista();
}

struct ils_texture *ils_texture_inc(char *id, char *path)
{
    struct ils_texture *texture = malloc(sizeof(*texture));

    texture->name = id;
    texture->path = path;
    fac_inc_item(textures, texture);

    return texture;
}

void ils_assign_texture(struct ils_obj * obj, struct ils_texture *texture)
{
    struct s_link *link = malloc(sizeof(*link));

    link->obj = obj;
    link->texture = texture;

    fac_inc_item(links, link);
}

static unsigned int next_power_of_2(unsigned int x)
{
    double logbase2 = log(x)/log(2);

    return round(pow(2,ceil(logbase2)));
}

static char def_texture(char *name, SDL_Surface *surface)
{
    struct ils_texture *texture = NULL;
    struct ils_gl *gl = ils_ret_gl_fncs();
    struct ils_sdl *sdl = ils_ret_sdl_fncs();
    struct SDL_Surface *new_surface;
    struct fac_iterador *it;

    if (surface == NULL)
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

    sdl->SDL_LockSurface(surface);
    texture->l = next_power_of_2(surface->w);
    texture->a = next_power_of_2(surface->h);
    sdl->SDL_UnlockSurface(surface);

    new_surface = sdl->SDL_CreateRGBSurface(0, texture->l, texture->a, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    sdl->SDL_BlitSurface(surface, 0, new_surface, 0);

    gl->glGenTextures(1, &texture->id);
    gl->glBindTexture(GL_TEXTURE_2D, texture->id);

    sdl->SDL_LockSurface(new_surface);
    gl->glTexImage2D(GL_TEXTURE_2D, 0, 4, texture->l, texture->a, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, new_surface->pixels);
    sdl->SDL_UnlockSurface(new_surface);

    sdl->SDL_FreeSurface(new_surface);
    sdl->SDL_FreeSurface(surface);

    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    gl->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    return 0;
}

void ils_def_img(void)
{
    struct ils_texture *texture;
    SDL_Surface *surface;
    struct ils_sdl *sdl = ils_ret_sdl_fncs();
    struct fac_iterador *it = fac_ini_iterador(textures);

    while (fac_existe_prox(it)) {
        texture = fac_proximo(it);
        surface = sdl->IMG_Load(texture->path);

        if (surface == NULL)
            printf("w: imagem \"%s\" não encontrada.\n", texture->name);
        else
            switch (def_texture(texture->name, surface)) {
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
