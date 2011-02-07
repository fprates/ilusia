/*
 * sdl.c
 *
 *  Created on: 03/12/2010
 *      Author: francisco.prates
 */

#include <faclib.h>
#include "sdl.h"

static struct s_sdl {
    void *lib;
    void *ttf;
    void *img;
    SDL_Surface *screen;
    struct ils_sdl fnc;
} sdl;

static int ini_video(struct ils_config *config, struct s_sdl *sdl_)
{
    sdl_->screen = sdl_->fnc.SDL_SetVideoMode(
            config->video.w,
            config->video.h,
            config->video.bpp,
            SDL_SWSURFACE
    );

    if (sdl_->screen == NULL)
        return -1;

    return 0;
}

static int def_sdl_fnc(struct s_sdl *sdl_)
{
    sdl_->fnc.SDL_Init = fac_ret_proc_lib(sdl_->lib, "SDL_Init");

    if (sdl_->fnc.SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return -1;

    sdl_->fnc.SDL_FreeSurface = fac_ret_proc_lib(sdl_->lib, "SDL_FreeSurface");
    sdl_->fnc.SDL_GetTicks = fac_ret_proc_lib(sdl_->lib, "SDL_GetTicks");
    sdl_->fnc.SDL_GL_GetProcAddress = fac_ret_proc_lib(
            sdl_->lib, "SDL_GL_GetProcAddress");
    sdl_->fnc.SDL_GL_LoadLibrary = fac_ret_proc_lib(
            sdl_->lib, "SDL_GL_LoadLibrary");
    sdl_->fnc.SDL_GL_SetAttribute = fac_ret_proc_lib(
            sdl_->lib, "SDL_GL_SetAttribute");
    sdl_->fnc.SDL_GL_SwapBuffers = fac_ret_proc_lib(
            sdl_->lib, "SDL_GL_SwapBuffers");
    sdl_->fnc.SDL_PollEvent = fac_ret_proc_lib(sdl_->lib, "SDL_PollEvent");
    sdl_->fnc.SDL_SetVideoMode = fac_ret_proc_lib(sdl_->lib, "SDL_SetVideoMode");
    sdl_->fnc.SDL_WM_SetCaption = fac_ret_proc_lib(sdl_->lib, "SDL_WM_SetCaption");
    sdl_->fnc.SDL_LockSurface = fac_ret_proc_lib(sdl_->lib, "SDL_LockSurface");
    sdl_->fnc.SDL_UnlockSurface = fac_ret_proc_lib(sdl_->lib, "SDL_UnlockSurface");
    sdl_->fnc.SDL_CreateRGBSurface = fac_ret_proc_lib(
            sdl_->lib, "SDL_CreateRGBSurface");
    sdl_->fnc.SDL_BlitSurface = fac_ret_proc_lib(sdl_->lib, "SDL_UpperBlit");
    sdl_->fnc.SDL_Quit = fac_ret_proc_lib(sdl_->lib, "SDL_Quit");

    /*
     * Funções SDL TTF
     */
    sdl_->fnc.TTF_Init = fac_ret_proc_lib(sdl_->ttf, "TTF_Init");
    sdl_->fnc.TTF_OpenFont = fac_ret_proc_lib(sdl_->ttf, "TTF_OpenFont");
    sdl_->fnc.TTF_RenderText_Blended = fac_ret_proc_lib(sdl_->ttf,
        "TTF_RenderText_Blended");

    /*
     * Funções SDL IMG
     */
    sdl_->fnc.IMG_Load = fac_ret_proc_lib(sdl.img, "IMG_Load");

    printf("i: funções SDL carregadas.\n");

    return 0;
}

int _ini_devices(struct ils_config *config)
{
    sdl.lib = fac_ld_lib(ILS_SDL);
    if (sdl.lib == NULL)
        return -1;

    sdl.ttf = fac_ld_lib(ILS_TTF);
    if (sdl.ttf == NULL)
        return -1;

    sdl.img = fac_ld_lib(ILS_IMG);
    if (sdl.img == NULL)
        return -1;

    if (def_sdl_fnc(&sdl) < 0)
        return -2;

    ini_video(config, &sdl);

    return 0;
}

void _term_devices(void)
{
    if ((sdl.lib == NULL) || (sdl.ttf == NULL) || (sdl.img == NULL))
        return;

    sdl.fnc.SDL_FreeSurface(sdl.screen);
    sdl.fnc.SDL_Quit();

    fac_lib_term(sdl.lib);
    fac_lib_term(sdl.ttf);
    fac_lib_term(sdl.img);
}

struct ils_sdl *ils_ret_sdl_fncs(void)
{
    return &sdl.fnc;
}
