/*
 * controls.c
 *
 *  Created on: 22/11/2010
 *      Author: francisco.prates
 */

#include <stdlib.h>
#include <stdio.h>
#include <faclib.h>
#include "ilusia.h"
#include "object.h"

struct fac_lista *global_pool;

struct s_pool_item {
    struct ils_obj * game;
    struct fac_lista *pool;
};

struct ils_control {
    char *name;
    struct fac_lista *keys;
    void (*input_proc)(struct ils_control *);
};

struct s_key {
    int code;
};

struct ils_control *ils_def_control(struct ils_obj *game, char *name)
{
    struct ils_control *control = malloc(sizeof(*control));
    control->name = name;
    control->keys = fac_ini_lista();
    _ins_control(game, control);

    printf("i: controle %s inicializado.\n", name);
    return control;
}

void ils_def_key(struct ils_control *control, int key)
{
    struct s_key *key_ = malloc(sizeof(*key_));
    key_->code = key;
    fac_inc_item(control->keys, key_);
}

void ils_def_input_proc(struct ils_control *control,
        void (*input_proc)(struct ils_control *))
{
    control->input_proc = input_proc;
}

void _term_control(struct ils_control *control)
{
	struct fac_iterador *it;

	if (control == NULL)
		return;

	it = fac_ini_iterador(control->keys);
	while (fac_existe_prox(it))
		free(fac_proximo(it));

	fac_rm_iterador(it);
	fac_rm_lista(control->keys);

	printf("i: controle %s finalizado.\n", control->name);
	free(control);
}
