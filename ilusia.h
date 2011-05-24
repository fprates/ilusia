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

enum ils_keypress {
    ILS_SINGLE, ILS_CONTINUOUS
};

enum ils_e_texture {
	DONT_INVERT, INVERT
};

enum ils_event_source {
	ILS_KEY, ILS_BOT
};

struct ils_obj;
struct ils_complex_obj;
struct ils_control;
struct ils_timer;

struct ils_video {
    unsigned int w;
    unsigned int h;
    unsigned int bpp;
};

struct ils_pos {
	float x;
	float y;
	float z;
	float sw;
	float sh;
	float sd;
    float dw;
    float dh;
    float dd;
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

struct ils_key_press {
    SDLKey code;
    int pressed;
};

struct ils_key;

struct ils_texture;

extern void ils_ini(void);

extern struct ils_obj *ils_def_obj(const char *);

extern void ils_def_obj_espec(struct ils_obj *, void *);

extern void *ils_ret_obj_espec(struct ils_obj *);

extern const char *ils_ret_obj_name(struct ils_obj *);

extern void ils_inc_obj(struct ils_obj *, struct ils_obj *);

extern struct fac_iterador *ils_ret_complex_objs(struct ils_obj *);

const extern char *ils_ret_name(struct ils_obj *);

extern void ils_def_pos(struct ils_obj *obj, struct ils_obj *cen,
		float x, float y, float z);

extern void ils_def_dim(struct ils_obj *obj, float w, float h, float d);

extern struct ils_obj *ils_ret_obj_from_complex(struct ils_complex_obj *);

extern void ils_def_relat_pos(struct ils_obj *orig, struct ils_obj *dest,
		float x, float y, float z);

extern struct ils_pos ils_ret_obj_pos(
        struct ils_obj *obj, struct ils_obj *cen);

extern void ils_def_output_proc(struct ils_obj *, void (*)(struct ils_view));

extern void ils_ini_controls(void);

extern struct ils_control *ils_def_control(char *control_name);

extern struct ils_key *ils_def_key(struct ils_control *, int, int,
        enum ils_keypress);

extern struct ils_key *ils_def_bot_proc(struct ils_control *control,
		unsigned int (*bot_proc)(struct ils_obj *cen, struct ils_obj *obj));

extern int ils_ret_event_code(struct ils_key *);

extern void ils_def_key_timer(struct ils_key *, unsigned int);

extern struct ils_timer *ils_ret_key_timer(struct ils_key *);

extern void ils_def_input_proc(struct ils_control *control,
		void (*input_proc)(struct ils_evento));

extern void ils_def_obj_control(struct ils_obj *obj,
		struct ils_control *control, enum ils_event_source event_source);

extern struct ils_control *ils_ret_obj_control(struct ils_obj *);

extern enum ils_event_source ils_ret_event_source(struct ils_obj *obj);

extern struct ils_key *ils_ret_bot_event(struct ils_obj *cen,
		struct ils_obj *obj);

extern struct ils_key *ils_ret_key_event(struct ils_obj *obj,
		struct ils_key_press *key_press);

extern void ils_send_event(struct ils_obj *, struct ils_evento *);

extern void ils_ini_timers(void);

extern struct ils_timer *ils_def_timer(char *name, unsigned int tempo);

extern unsigned char ils_ret_timer_mrk(struct ils_timer *timer);

extern unsigned char ils_is_timer_stopped(struct ils_timer *timer);

extern void ils_start_timer(struct ils_timer *timer);

extern void ils_stop_timer(struct ils_timer *timer);

extern void ils_reset_timer_cnt(struct ils_timer *timer);

extern void ils_term_timer(struct ils_timer *timer);

extern void ils_term_timers(void);

extern void ils_term_controls(void);

extern struct ils_sdl *ils_ret_sdl_fncs(void);

extern struct ils_gl *ils_ret_gl_fncs(void);

extern void ils_def_signal(int);

extern void ils_start(struct ils_obj *, struct ils_obj *, struct ils_config);

extern void ils_term(struct ils_obj *);

extern void ils_term_all(struct ils_obj *);

extern void ils_ini_textures(void);

extern struct ils_texture *ils_texture_inc(char *name, char *path);

extern void ils_def_img(void);

extern void ils_show_texture(struct ils_texture *texture, struct ils_pos *pos,
		unsigned int mode);

#endif /* ILUSIA_H_ */
