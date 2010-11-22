/*
 * ilusia.h
 *
 *  Created on: 18/11/2010
 *      Author: francisco.prates
 */

#ifndef ILUSIA_H_
#define ILUSIA_H_

struct ils_obj;

struct ils_config {
    char *title;
};

extern struct ils_obj *ils_ini(const struct ils_config);
extern struct ils_obj *ils_def(const char *);
extern void ils_inc(struct ils_obj *, struct ils_obj *);
extern void ils_start(struct ils_obj *);
extern void ils_term(struct ils_obj *);
extern void ils_term_all(struct ils_obj *);

#endif /* ILUSIA_H_ */
