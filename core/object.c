/*
 * ilusia.c
 *
 *  Created on: 18/11/2010
 *      Author: francisco.prates
 */

#include <stdlib.h>
#include <stdio.h>
#include <faclib.h>
#include "../ilusia.h"
#include "object.h"

struct ils_obj {
	char *name;
	struct fac_lista *objs;
	enum ils_event_source event_source;
	struct ils_control *control;
	void (*proc_output)(struct ils_view);
    void *espec;
    unsigned long status;
    struct ils_pos pos;
};

struct s_game {
	struct ils_config *config;
};

struct ils_complex_obj {
	struct ils_obj *obj;
	struct ils_pos pos;
	unsigned int status;
};

static struct fac_lista *pool;

void _ini_objects(void)
{
	pool = fac_ini_lista();
}

struct ils_obj *ils_def_obj(char *name)
{
	struct ils_obj *obj = malloc(sizeof(*obj));
	obj->name = name;
	obj->objs = fac_ini_lista();
	obj->espec = NULL;
	obj->control = NULL;
	obj->proc_output = NULL;
	obj->pos.dw = 0;
	obj->pos.dh = 0;
	obj->pos.dd = 0;

	fac_inc_item(pool, obj);

	printf("i: objeto %s gerado.\n", name);
	return obj;
}

struct ils_obj *ils_ret_obj_by_name(char *name)
{
    struct ils_obj *obj = NULL;
    struct fac_iterador *it = fac_ini_iterador(pool);

    while (fac_existe_prox(it)) {
        obj = fac_proximo(it);

        if (strcmp(obj->name, name) == 0)
            break;

        obj = NULL;
    }

    fac_rm_iterador(it);

    return obj;
}

void ils_def_obj_espec(struct ils_obj *obj, void *espec)
{
    obj->espec = espec;
}

void *ils_ret_obj_espec(struct ils_obj *obj)
{
    return obj->espec;
}

char *ils_ret_obj_name(struct ils_obj *obj)
{
    return obj->name;
}

void ils_inc_obj(struct ils_obj *orig, struct ils_obj *dest)
{
	struct ils_complex_obj *obj = malloc(sizeof(*obj));

	obj->obj = orig;
	obj->pos.sw = 1;
	obj->pos.sh = 1;
	obj->pos.sd = 1;
	obj->pos.dw = 0;
	obj->pos.dh = 0;
	obj->pos.dd = 0;
	obj->pos.x = 0;
	obj->pos.y = 0;
	obj->pos.z = 0;
	obj->status = 1;

	fac_inc_item(dest->objs, obj);
	printf("i: incluindo %s em %s.\n", orig->name, dest->name);
}

static struct ils_complex_obj *ret_obj_from_cen(struct ils_obj *obj, struct ils_obj *cen)
{
    struct ils_complex_obj *obj_ = NULL;
    struct fac_iterador *it = fac_ini_iterador(cen->objs);

    while (fac_existe_prox(it)) {
        obj_ = fac_proximo(it);

        if (obj_->obj == obj)
            break;

        obj_ = NULL;
    }

    fac_rm_iterador(it);

    return obj_;
}

void ils_set_obj_enabled(
        struct ils_obj *orig, struct ils_obj *dest, unsigned char enabled)
{
    struct ils_complex_obj *obj = ret_obj_from_cen(orig, dest);

    obj->status = enabled;
}

unsigned char ils_is_obj_enabled(struct ils_obj *orig, struct ils_obj *dest)
{
    struct ils_complex_obj *obj = ret_obj_from_cen(orig, dest);

    return obj->status;
}

struct fac_iterador *_ret_complex_objs(struct ils_obj *obj)
{
	return fac_ini_iterador(obj->objs);
}

struct ils_obj *_ret_obj_from_complex(struct ils_complex_obj *complex)
{
	return complex->obj;
}

void ils_def_obj_control(struct ils_obj *obj, struct ils_control *control,
		enum ils_event_source event_source)
{
	obj->event_source = event_source;
    obj->control = control;
}

struct ils_control *ils_ret_obj_control(struct ils_obj *obj)
{
	return obj->control;
}

void ils_def_pos(struct ils_obj *orig, struct ils_obj *dest,
        float x, float y, float z)
{
	struct ils_complex_obj *obj = ret_obj_from_cen(orig, dest);

	if (obj == NULL)
	    return;

    obj->pos.x = x;
    obj->pos.y = y;
    obj->pos.z = z;
}

void ils_def_dim(struct ils_obj *obj, float w, float h, float d)
{
    obj->pos.dw = w;
    obj->pos.dh = h;
    obj->pos.dd = d;
}

void ils_def_scale(struct ils_obj *obj, struct ils_obj *cen,
        float w, float h, float d)
{
    struct ils_complex_obj *obj_ = ret_obj_from_cen(obj, cen);

    if (obj_ == NULL)
        return;

    obj_->pos.sw = w;
    obj_->pos.sh = h;
    obj_->pos.sd = d;
}

void ils_def_relat_pos(struct ils_obj *obj, struct ils_obj *cen,
		float x, float y, float z)
{
    struct ils_complex_obj *obj_ = ret_obj_from_cen(obj, cen);

    if (obj_ == NULL)
        return;

    obj_->pos.x += x;
    obj_->pos.y += y;
    obj_->pos.z += z;
}

struct ils_pos ils_ret_obj_pos(struct ils_obj *orig, struct ils_obj *dest)
{
    struct ils_pos pos;
	struct ils_complex_obj *obj = ret_obj_from_cen(orig, dest);

	if (obj == NULL)
	    return (struct ils_pos){0};

	pos = orig->pos;
	pos.x = obj->pos.x;
	pos.y = obj->pos.y;
	pos.z = obj->pos.z;
	pos.sw = obj->pos.sw;
	pos.sh = obj->pos.sh;
	pos.sd = obj->pos.sd;
	return pos;
}

void ils_def_output_proc(struct ils_obj *obj,
		void (*proc_output)(struct ils_view view))
{
	obj->proc_output = proc_output;
}

enum ils_event_source ils_ret_event_source(struct ils_obj *obj)
{
	return obj->event_source;
}


void ils_def_obj_status(struct ils_obj *obj, unsigned long status)
{
    obj->status = status;
}

unsigned char ils_test_obj_status(struct ils_obj *obj, unsigned int status)
{
    unsigned int set = 1 << status;

    return (obj->status & set)? 1 : 0;
}

void ils_set_obj_status(struct ils_obj *obj, unsigned int status)
{
    unsigned int set = 1 << status;

    obj->status |= set;
}

void ils_reset_obj_status(struct ils_obj *obj, unsigned int status)
{
    unsigned int set = 1 << status;

    obj->status &= ~set;
}

void _call_output_proc(struct ils_obj *cen, struct ils_obj *obj)
{
	struct ils_view view;

	if (obj->proc_output == NULL)
		return;

	view.cen = cen;
	view.obj = obj;
	obj->proc_output(view);
}

void _term_objects(void)
{
	struct ils_obj *obj;
	struct fac_iterador *iti;
	struct fac_iterador *it = fac_ini_iterador(pool);

	while (fac_existe_prox(it)) {
		obj = fac_proximo(it);
		iti = fac_ini_iterador(obj->objs);
		while (fac_existe_prox(iti))
			free(fac_proximo(iti));

		fac_rm_iterador(iti);
		fac_rm_lista(obj->objs);

		printf("i: objeto %s removido.\n", obj->name);

		free(obj);
	}

	fac_rm_iterador(it);
	fac_rm_lista(pool);
}
