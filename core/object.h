/*
 * object.h
 *
 *  Created on: 22/11/2010
 *      Author: francisco.prates
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "../ilusia.h"

extern void _call_output_proc(struct ils_obj *, struct ils_obj *);

extern void _ini_objects(void);

extern void _term_objects(void);

#endif /* OBJECT_H_ */
