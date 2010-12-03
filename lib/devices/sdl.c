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
    struct ils_sdl fnc;
} sdl;

int _ini_devices(void)
{
    sdl.lib = fac_ld_lib(ILS_SDL);
    sdl.ttf = fac_ld_lib(ILS_TTF);
    sdl.img = fac_ld_lib(ILS_IMG);

    if ((sdl.lib == NULL) || (sdl.ttf == NULL) || (sdl.img == NULL))
        return -1;

    sdl.fnc.SDL_Init = fac_ret_proc_lib(sdl.lib, "SDL_Init");

    if (sdl.fnc.SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return -2;

    sdl.fnc.SDL_FreeSurface = fac_ret_proc_lib(sdl.lib, "SDL_FreeSurface");
    sdl.fnc.SDL_GetTicks = fac_ret_proc_lib(sdl.lib, "SDL_GetTicks");
    sdl.fnc.SDL_GL_GetProcAddress = fac_ret_proc_lib(sdl.lib, "SDL_GL_GetProcAddress");
    sdl.fnc.SDL_GL_LoadLibrary = fac_ret_proc_lib(sdl.lib, "SDL_GL_LoadLibrary");
    sdl.fnc.SDL_GL_SetAttribute = fac_ret_proc_lib(sdl.lib, "SDL_GL_SetAttribute");
    sdl.fnc.SDL_GL_SwapBuffers = fac_ret_proc_lib(sdl.lib, "SDL_GL_SwapBuffers");
    sdl.fnc.SDL_PollEvent = fac_ret_proc_lib(sdl.lib, "SDL_PollEvent");
    sdl.fnc.SDL_SetVideoMode = fac_ret_proc_lib(sdl.lib, "SDL_SetVideoMode");
    sdl.fnc.SDL_WM_SetCaption = fac_ret_proc_lib(sdl.lib, "SDL_WM_SetCaption");
    sdl.fnc.SDL_LockSurface = fac_ret_proc_lib(sdl.lib, "SDL_LockSurface");
    sdl.fnc.SDL_UnlockSurface = fac_ret_proc_lib(sdl.lib, "SDL_UnlockSurface");
    sdl.fnc.SDL_CreateRGBSurface = fac_ret_proc_lib(sdl.lib, "SDL_CreateRGBSurface");
    sdl.fnc.SDL_BlitSurface = fac_ret_proc_lib(sdl.lib, "SDL_UpperBlit");

    if (sdl.ttf == NULL) {
        sdl.fnc.TTF_Init = NULL;
        sdl.fnc.TTF_OpenFont = NULL;
        sdl.fnc.TTF_RenderText_Blended = NULL;
    } else {
        sdl.fnc.TTF_Init = fac_ret_proc_lib(sdl.ttf, "TTF_Init");
        sdl.fnc.TTF_OpenFont = fac_ret_proc_lib(sdl.ttf, "TTF_OpenFont");
        sdl.fnc.TTF_RenderText_Blended = fac_ret_proc_lib(sdl.ttf,
            "TTF_RenderText_Blended");
    }

    if (sdl.img == NULL)
        sdl.fnc.IMG_Load = NULL;
    else
        sdl.fnc.IMG_Load = fac_ret_proc_lib(sdl.img, "IMG_Load");

    return 0;
}

void _term_devices(void)
{
    sdl.fnc.SDL_Quit();
}
