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

/*
 * Retorna objetos complexos de outro objeto
 */
extern struct fac_iterador *_ret_complex_objs(struct ils_obj *obj);

/*
 * Extrai objeto de um objeto complexo
 */
extern struct ils_obj *_ret_obj_from_complex(
        struct ils_complex_obj *complex);

#endif /* OBJECT_H_ */
