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
#include "controls.h"

struct ils_obj {
	const char *name;
	struct fac_lista *objs;
	struct ils_control *control;
	void (*proc_output)(struct ils_view);
    void *espec;
};

struct s_game {
	struct ils_config *config;
	struct fac_lista *controls;
};

struct ils_complex_obj {
	struct ils_obj *obj;
	struct ils_pos pos;
};

struct ils_obj *ils_ini(struct ils_config config)
{
    struct ils_obj *game = ils_def_obj(config.title);
    struct s_game *espec = malloc(sizeof(*espec));

    espec->config = &config;
    espec->controls = fac_ini_lista();

    game->espec = espec;

    return game;
}

struct ils_obj *ils_def_obj(const char *name)
{
	struct ils_obj *obj = malloc(sizeof(*obj));
	obj->name = name;
	obj->objs = fac_ini_lista();
	obj->espec = NULL;
	obj->control = NULL;

	printf("i: objeto %s gerado.\n", name);
	return obj;
}


void ils_inc_obj(struct ils_obj *orig, struct ils_obj *dest)
{
	struct ils_complex_obj *obj = malloc(sizeof(*obj));

	obj->obj = orig;
	fac_inc_item(dest->objs, obj);
}

struct fac_iterador *ils_ret_complex_objs(struct ils_obj *obj)
{
	return fac_ini_iterador(obj->objs);
}

struct ils_obj *ils_ret_obj_from_complex(struct ils_complex_obj *complex)
{
	return complex->obj;
}

const char *ils_ret_name(struct ils_obj *obj)
{
	return obj->name;
}

void ils_def_obj_control(struct ils_obj *obj, struct ils_control *control)
{
    obj->control = control;
}

struct ils_control *ils_ret_obj_control(struct ils_obj *obj)
{
	return obj->control;
}

void ils_def_pos(struct ils_obj *obj, struct ils_obj *cen, float x, float y, float z)
{
	struct ils_complex_obj *obj_;
	struct fac_iterador *it = fac_ini_iterador(cen->objs);

	while (fac_existe_prox(it)) {
		obj_ = fac_proximo(it);

		obj_->pos.x = x;
		obj_->pos.y = y;
		obj_->pos.z = z;
	}

	fac_rm_iterador(it);
}

void ils_def_relat_pos(struct ils_obj *dest, struct ils_obj *orig,
		float x, float y, float z)
{
	struct ils_complex_obj *obj;
	struct fac_iterador *it = fac_ini_iterador(dest->objs);

	while (fac_existe_prox(it)) {
		obj = fac_proximo(it);

		if (obj->obj != orig)
			continue;

		obj->pos.x += x;
		obj->pos.y += y;
		obj->pos.z += z;
		break;
	}

	fac_rm_iterador(it);
}

struct ils_pos ils_ret_obj_pos(struct ils_obj *dest, struct ils_obj *orig)
{
	struct ils_complex_obj *obj;
	struct ils_pos pos;
	struct fac_iterador *it = fac_ini_iterador(dest->objs);

	while (fac_existe_prox(it)) {
		obj = fac_proximo(it);

		if (obj->obj != orig)
			continue;

		pos = obj->pos;
		break;
	}

	fac_rm_iterador(it);
	return pos;
}

void ils_def_output_proc(struct ils_obj *obj,
		void (*proc_output)(struct ils_view view))
{
	obj->proc_output = proc_output;
}

void _call_output_proc(struct ils_obj *cen, struct ils_obj *obj)
{
	struct ils_view view;

	view.cen = cen;
	view.obj = obj;
	obj->proc_output(view);
}

void _ins_control(struct ils_obj *game, struct ils_control *control)
{
	struct s_game *espec = game->espec;
	fac_inc_item(espec->controls, control);
}

void ils_term(struct ils_obj *obj)
{
    if (obj == NULL)
    	return;

    fac_rm_lista(obj->objs);
    free(obj);
}

static void term_controls(struct ils_obj *game)
{
	if (game->espec == NULL)
		return;

	struct s_game *espec = game->espec;
    struct fac_iterador *it = fac_ini_iterador(espec->controls);

    /*
     * remove controles individuais
     */
    while (fac_existe_prox(it))
        _term_control(fac_proximo(it));

    fac_rm_iterador(it);
    fac_rm_lista(espec->controls);
}

void ils_term_all(struct ils_obj *obj)
{
	struct ils_complex_obj *obj_;
	struct fac_iterador *it;

	if (obj == NULL)
		return;

	it = fac_ini_iterador(obj->objs);

	while (fac_existe_prox(it)) {
		obj_ = fac_proximo(it);
		ils_term_all(obj_->obj);
		free(obj_);
	}

	fac_rm_iterador(it);
	fac_rm_lista(obj->objs);
	term_controls(obj);

	printf("i: objeto %s finalizado.\n", obj->name);
	free(obj);
}
