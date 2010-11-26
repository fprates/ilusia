/*
 * ilusia.c
 *
 *  Created on: 18/11/2010
 *      Author: francisco.prates
 */

#include <stdlib.h>
#include <stdio.h>
#include <faclib.h>
#include "ilusia.h"
#include "object.h"
#include "controls.h"

struct ils_obj {
	const char *name;
	struct fac_lista *objs;
	struct ils_control *control;
    void *espec;
};

struct s_game {
	struct ils_config *config;
	struct fac_lista *controls;
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

	printf("i: objeto %s gerado.\n", name);
	return obj;
}


void ils_inc_obj(struct ils_obj *orig, struct ils_obj *dest)
{
	fac_inc_item(dest->objs, orig);
}

struct fac_iterador *ils_ret_objs(struct ils_obj *obj)
{
	return fac_ini_iterador(obj->objs);
}

const char *ils_ret_name(struct ils_obj *obj)
{
	return obj->name;
}

void ils_def_obj_control(struct ils_obj *obj, struct ils_control *control)
{
    obj->control = control;
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
	struct ils_obj *obj_;
	struct fac_iterador *it;

	if (obj == NULL)
		return;

	it = fac_ini_iterador(obj->objs);

	while (fac_existe_prox(it)) {
		obj_ = fac_proximo(it);
		ils_term_all(obj_);
	}

	fac_rm_iterador(it);
	fac_rm_lista(obj->objs);
	term_controls(obj);

	printf("i: objeto %s finalizado.\n", obj->name);
	free(obj);
}
