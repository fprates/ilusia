/*
 * ilusia.h
 *
 *  Created on: 18/11/2010
 *      Author: francisco.prates
 */

#ifndef ILUSIA_H_
#define ILUSIA_H_

struct ils_obj;
struct ils_control;

struct ils_config {
    char *title;
};

extern struct ils_obj *ils_ini(const struct ils_config);
extern struct ils_obj *ils_def_obj(const char *);
extern void ils_inc_obj(struct ils_obj *, struct ils_obj *);
extern struct ils_control *ils_def_control(struct ils_obj *, char *);

extern void ils_def_key(struct ils_control *, int);
extern void ils_def_input_proc(struct ils_control *,
        void (*)(struct ils_control *));
extern void ils_def_obj_control(struct ils_obj *, struct ils_control *);

extern void ils_start(struct ils_obj *, struct ils_obj *);
extern void ils_term(struct ils_obj *);
extern void ils_term_all(struct ils_obj *);

#endif /* ILUSIA_H_ */
