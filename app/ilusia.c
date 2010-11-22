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
struct ils_obj {
	const char *name;
	struct fac_lista *objs;
    void *espec;
};

struct ils_obj *ils_ini(struct ils_config config)
{
    struct ils_obj *game = ils_def(config.title);
    game->espec = &config;

    return game;
}

struct ils_obj *ils_def(const char *name)
{
	struct ils_obj *obj = malloc(sizeof(*obj));
	obj->name = name;
	obj->objs = fac_ini_lista();

	printf("i: objeto %s gerado.\n", name);
	return obj;
}

void ils_inc(struct ils_obj *orig, struct ils_obj *dest)
{
	fac_inc_item(dest->objs, orig);
}

void ils_start(struct ils_obj *game)
{

}

void ils_term(struct ils_obj *obj)
{
    if (obj == NULL)
    	return;

    fac_rm_lista(obj->objs);
    free(obj);
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

	printf("i: objeto %s finalizado.\n", obj->name);
	free(obj);
}
