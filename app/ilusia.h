/*
 * ilusia.h
 *
 *  Created on: 18/11/2010
 *      Author: francisco.prates
 */

#ifndef ILUSIA_H_
#define ILUSIA_H_

//#ifdef ILUSIA_LINUX
//#define ILSPREFIX
//#define ILS_SDL "libSDL.so"
//#define ILS_GL "libGL.so"
//#define ILS_GLU "libGLU.so"
//#define ILS_TTF "libSDL_ttf.so"
//#define ILS_IMG "libSDL_image.so"
//#endif
//
//#ifdef ILUSIA_WIN32
//#include <windows.h>
//#define ILS_SDL "SDL.dll"
//#define ILS_GL "opengl32.dll"
//#define ILS_GLU "glu32.dll"
//#define ILS_TTF "SDL_ttf.dll"
//#define ILS_IMG "SDL_image.dll"
//#if BUILDING_DLL
//# define ILSPREFIX __declspec (dllexport)
//#else /* Not BUILDING_DLL */
//# define ILSPREFIX __declspec (dllimport)
//#endif /* Not BUILDING_DLL */
//#endif

struct ils_obj;
struct ils_complex_obj;
struct ils_control;

struct ils_config {
    char *title;
};

struct ils_evento {
	struct ils_obj *obj;
	struct ils_obj *cen;
	struct ils_control *control;
	int key_code;
};

extern struct ils_obj *ils_ini(const struct ils_config);
extern struct ils_obj *ils_def_obj(const char *);
extern void ils_inc_obj(struct ils_obj *, struct ils_obj *);
extern struct ils_control *ils_def_control(struct ils_obj *, char *);
extern struct fac_iterador *ils_ret_complex_objs(struct ils_obj *);
const extern char *ils_ret_name(struct ils_obj *);
extern void ils_def_pos(struct ils_obj *, struct ils_obj *,
		float, float, float);
extern struct ils_obj *ils_ret_obj_from_complex(struct ils_complex_obj *);

extern void ils_def_key(struct ils_control *, int);
extern void ils_def_input_proc(struct ils_control *,
		void (*)(struct ils_evento));
extern void ils_def_obj_control(struct ils_obj *, struct ils_control *);
extern struct ils_control *ils_ret_obj_control(struct ils_obj *);
extern int ils_verif_key_event(struct ils_obj *, int);
extern void ils_send_event(struct ils_obj *, struct ils_evento *);

extern void ils_start(struct ils_obj *, struct ils_obj *);
extern void ils_term(struct ils_obj *);
extern void ils_term_all(struct ils_obj *);

#endif /* ILUSIA_H_ */
