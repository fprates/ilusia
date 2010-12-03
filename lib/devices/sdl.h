/*
 * sdlout.h
 *
 *  Created on: 27/11/2010
 *      Author: francisco.prates
 */

#ifndef SDLOUT_H_
#define SDLOUT_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "../ilusia.h"

#ifdef ILUSIA_LINUX
#define ILSPREFIX
#define ILS_SDL "libSDL.so"
#define ILS_TTF "libSDL_ttf.so"
#define ILS_IMG "libSDL_image.so"
#endif

#ifdef ILUSIA_WIN32
#include <windows.h>
#define ILS_SDL "SDL.dll"
#define ILS_TTF "SDL_ttf.dll"
#define ILS_IMG "SDL_image.dll"
#if BUILDING_DLL
# define ILSPREFIX __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define ILSPREFIX __declspec (dllimport)
#endif /* Not BUILDING_DLL */
#endif

struct ils_sdl {
    void SDLCALL (*SDL_Delay)(unsigned int);
    void SDLCALL (*SDL_FreeSurface)(SDL_Surface *);
    unsigned int SDLCALL (*SDL_GetTicks)(void);
    void * SDLCALL (*SDL_GL_GetProcAddress)(const char*);
    int SDLCALL (*SDL_GL_LoadLibrary)(const char *);
    int SDLCALL (*SDL_GL_SetAttribute)(SDL_GLattr attr, int);
    void SDLCALL (*SDL_GL_SwapBuffers)(void);
    int SDLCALL (*SDL_Init)(unsigned int);
    int SDLCALL (*SDL_PollEvent)(SDL_Event *);
    void SDLCALL (*SDL_Quit)(void);
    SDL_Surface * SDLCALL (*SDL_SetVideoMode)(int, int, int, unsigned int);
    void SDLCALL (*SDL_WM_SetCaption)(const char *, const char *);
    void SDLCALL (*SDL_LockSurface)(SDL_Surface *);
    void SDLCALL (*SDL_UnlockSurface)(SDL_Surface *);
    SDL_Surface * SDLCALL (*SDL_CreateRGBSurface)(Uint32, int, int, int,
            Uint32, Uint32, Uint32, Uint32);
    int SDLCALL (*SDL_BlitSurface)(SDL_Surface *, SDL_Rect *, SDL_Surface *,
            SDL_Rect *);
    int SDLCALL (*TTF_Init)(void);
    TTF_Font * SDLCALL (*TTF_OpenFont)(const char *, int);
    SDL_Surface * SDLCALL (*TTF_RenderText_Blended)(TTF_Font *, const char *,
            SDL_Color);
    SDL_Surface * SDLCALL (*IMG_Load)(const char *);
};

extern int _ini_devices(struct ils_config *);
extern void _term_devices(void);
extern int _ret_key_code(void);

#endif /* SDLOUT_H_ */
