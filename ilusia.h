/*
 * ilusia.h
 *
 *  Created on: 18/11/2010
 *      Author: francisco.prates
 */

#ifndef ILUSIA_H_
#define ILUSIA_H_

#include "render.h"

enum ils_signal {ILS_TERM};

struct ils_obj;
struct ils_complex_obj;
struct ils_control;

struct ils_video {
    unsigned int w;
    unsigned int h;
    unsigned int bpp;
};

struct ils_pos {
	float x;
	float y;
	float z;
	float w;
	float h;
	float d;
};

struct ils_persp {
    float fovy;
    float aspec;
    float zprox;
    float zdist;
};

struct ils_camera {
    float obsx;
    float obsy;
    float obsz;
    float objx;
    float objy;
    float objz;
    float vupx;
    float vupy;
    float vupz;
};

struct ils_config {
    char *title;
    struct ils_video video;
    struct ils_pos view;
    struct ils_persp persp;
    struct ils_camera camera;
};

struct ils_view {
	struct ils_obj *cen;
	struct ils_obj *obj;
};

struct ils_evento {
	struct ils_obj *obj;
	struct ils_obj *cen;
	int key_code;
	int evcode;
};

extern struct ils_obj *ils_def_obj(const char *);
extern void ils_inc_obj(struct ils_obj *, struct ils_obj *);
extern struct fac_iterador *ils_ret_complex_objs(struct ils_obj *);
const extern char *ils_ret_name(struct ils_obj *);
extern void ils_def_pos(struct ils_obj *, struct ils_obj *,
		float, float, float);
extern struct ils_obj *ils_ret_obj_from_complex(struct ils_complex_obj *);
extern void ils_def_relat_pos(struct ils_obj *, struct ils_obj *,
		float, float, float);
extern struct ils_pos ils_ret_obj_pos(struct ils_obj *, struct ils_obj *);

extern void ils_def_output_proc(struct ils_obj *, void (*)(struct ils_view));

extern void ils_ini_controls(void);
extern struct ils_control *ils_def_control(char *);
extern void ils_def_key(struct ils_control *, int, int);
extern void ils_def_input_proc(struct ils_control *,
		void (*)(struct ils_evento));
extern void ils_def_obj_control(struct ils_obj *, struct ils_control *);
extern struct ils_control *ils_ret_obj_control(struct ils_obj *);
extern int ils_ret_key_event(struct ils_obj *, int);
extern void ils_send_event(struct ils_obj *, struct ils_evento *);
extern void ils_term_controls(void);

extern struct ils_sdl *ils_ret_sdl_fncs(void);
extern struct ils_gl *ils_ret_gl_fncs(void);

extern void ils_def_signal(int);
extern void ils_start(struct ils_obj *, struct ils_obj *, struct ils_config);
extern void ils_term(struct ils_obj *);
extern void ils_term_all(struct ils_obj *);

#endif /* ILUSIA_H_ */
