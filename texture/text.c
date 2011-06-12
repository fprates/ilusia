#include <faclib.h>
#include <stdlib.h>
#include <SDL/SDL_ttf.h>
#include "texture.h"
#include "text.h"

struct ils_font {
    char *id;
    char *fontname;
    TTF_Font *ttf_font;
    unsigned int size;
    struct ils_cor cor;
};

struct s_text {
    char *text;
    struct ils_font *font;
    struct ils_texture *texture;
    unsigned char changed;
};

static struct s_global {
    struct fac_lista *fonts;
} global;

void _ini_fonts(void)
{
    global.fonts = fac_ini_lista();
}

void _term_fonts(void)
{
    struct ils_font *font;
    struct ils_sdl *sdl = ils_ret_sdl_fncs();
    struct fac_iterador *it = fac_ini_iterador(global.fonts);

    while (fac_existe_prox(it)) {
        font = fac_proximo(it);
        sdl->TTF_CloseFont(font->ttf_font);
        free(font);
    }

    fac_rm_lista(global.fonts);
}

void _def_fonts(void)
{
    struct ils_font *font;
    struct fac_iterador *it = fac_ini_iterador(global.fonts);
    struct ils_sdl *sdl = ils_ret_sdl_fncs();

    while (fac_existe_prox(it)) {
        font = fac_proximo(it);

        font->ttf_font = sdl->TTF_OpenFont(font->fontname, font->size);
        if (font->ttf_font == NULL)
            printf("w: fonte %s não carregada. Textos com "
                    "essa fonte não serão exibidos.\n", font->id);
    }

    fac_rm_iterador(it);
}

static void create_texture_from_text(char *name, struct s_text *text)
{
    struct ils_sdl *sdl = ils_ret_sdl_fncs();
    struct ils_font *font = text->font;
    SDL_Surface *surface, *surface_;

    SDL_Color cor = {
            .r = 255 * font->cor.r,
            .g = 255 * font->cor.g,
            .b = 255 * font->cor.b
    };

    text->changed = 0;
    surface = sdl->TTF_RenderText_Blended(font->ttf_font, text->text, cor);

    sdl->SDL_LockSurface(surface);
    _set_texture(text->texture, surface->w, surface->h);
    surface_ = sdl->SDL_CreateRGBSurface(0, surface->w, surface->h,
    		32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    sdl->SDL_UnlockSurface(surface);

    sdl->SDL_BlitSurface(surface, 0, surface_, 0);

    sdl->SDL_LockSurface(surface_);
    _set_texture(text->texture, surface_->w, surface_->h);
    _load_texture(text->texture, ILS_BGRA, surface_->pixels);
    sdl->SDL_UnlockSurface(surface_);

    sdl->SDL_FreeSurface(surface);
    sdl->SDL_FreeSurface(surface_);
}

static void text_output_proc(struct ils_view view)
{
    struct s_text *text = ils_ret_obj_espec(view.obj);
    struct ils_pos pos = ils_ret_obj_pos(view.obj, view.cen);

    pos.sw = 1;
    pos.sh = 1;
    pos.sd = 1;

    if ((text->texture == NULL) || (text->changed == 1))
        create_texture_from_text(ils_ret_obj_name(view.obj), text);

    ils_show_texture(text->texture, &pos, INVERT);
}

struct ils_obj *ils_def_text_obj(char *name)
{
    struct s_text *text = malloc(sizeof(*text));
    struct ils_obj *obj = ils_def_obj(name);

    text->texture = _def_texture(name);
    ils_def_obj_espec(obj, text);
    ils_def_output_proc(obj, text_output_proc);

    return obj;
}

void ils_set_text(struct ils_obj *obj, char *text, struct ils_font *font)
{
    struct s_text *text_ = ils_ret_obj_espec(obj);

    text_->text = text;
    text_->font = font;
    text_->changed = 1;
}

struct ils_font *ils_def_font(
        char *id, char *fontname, unsigned int size, struct ils_cor *cor)
{
    struct ils_font *font = malloc(sizeof(*font));

    font->id = id;
    font->fontname = fontname;
    font->size = size;
    font->ttf_font = NULL;
    font->cor = *cor;

    fac_inc_item(global.fonts, font);

    return font;
}
