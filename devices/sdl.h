/*
 * sdlout.h
 *
 *  Created on: 27/11/2010
 *      Author: francisco.prates
 */

#ifndef SDLOUT_H_
#define SDLOUT_H_

#include "../ilusia.h"

#ifdef linux
#define ILSPREFIX
#define ILS_SDL "libSDL.so"
#define ILS_TTF "libSDL_ttf.so"
#define ILS_IMG "libSDL_image.so"
#define ILS_GL "libGL.so"
#define ILS_GLU "libGLU.so"
#endif

#ifdef __WIN32__
#include <windows.h>
#define ILS_SDL "SDL.dll"
#define ILS_TTF "SDL_ttf.dll"
#define ILS_IMG "SDL_image.dll"
#define ILS_GL "opengl32.dll"
#define ILS_GLU "glu32.dll"
#if BUILDING_DLL
# define ILSPREFIX __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define ILSPREFIX __declspec (dllimport)
#endif /* Not BUILDING_DLL */
#endif

extern int _ini_devices(struct ils_config *);
extern void _term_devices(void);
extern int _ret_key_code(void);
extern void _frame_start(void);
extern void _output_commit(void);
extern void _pop_state(void);
extern void _push_state(void);

#endif /* SDLOUT_H_ */
