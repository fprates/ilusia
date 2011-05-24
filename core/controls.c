/*
 * controls.c
 *
 *  Created on: 22/11/2010
 *      Author: francisco.prates
 */

#include <stdlib.h>
#include <stdio.h>
#include <faclib.h>
#include "../ilusia.h"
#include "object.h"

struct fac_lista *pool;

struct ils_control {
    char *name;
    struct fac_lista *keys;
    void (*input_proc)(struct ils_evento);
};

struct ils_key {
    int code;
    int evcode;
    enum ils_keypress mode;
    int active;
    struct ils_timer *timer;
};

void ils_ini_controls()
{
    pool = fac_ini_lista();
}

struct ils_control *ils_def_control(char *name)
{
    struct ils_control *control = malloc(sizeof(*control));
    control->name = name;
    control->keys = fac_ini_lista();
    fac_inc_item(pool, control);

    printf("i: controle %s inicializado.\n", name);
    return control;
}

struct ils_key *ils_def_key(struct ils_control *control, int evcode, int key, enum ils_keypress mode)
{
    struct ils_key *key_ = malloc(sizeof(*key_));

    key_->code = key;
    key_->evcode = evcode;
    key_->mode = mode;
    key_->active = 0;
    key_->timer = NULL;

    fac_inc_item(control->keys, key_);

    return key_;
}

int ils_ret_event_code(struct ils_key *key)
{
    return key->evcode;
}

void ils_def_key_timer(struct ils_key *key, unsigned int time)
{
    key->timer = ils_def_timer("key_timer", time);
}

struct ils_timer *ils_ret_key_timer(struct ils_key *key)
{
    return key->timer;
}

void ils_def_input_proc(struct ils_control *control,
		void (*input_proc)(struct ils_evento))
{
    control->input_proc = input_proc;
}

struct ils_key *ils_ret_key_event(struct ils_obj *obj, struct ils_key_press *key_press)
{
	struct fac_iterador *it;
    struct ils_key *key = NULL;
	struct ils_control *control = ils_ret_obj_control(obj);

	if (control == NULL)
	    return NULL;

	it = fac_ini_iterador(control->keys);

	while (fac_existe_prox(it)) {
		key = fac_proximo(it);

		if ((key->code != key_press->code) && (key->active == 0)) {
		    key = NULL;
			continue;
		}

		switch (key->mode) {
		case ILS_CONTINUOUS:
		    if ((key->code == key_press->code) && (key_press->pressed != -1))
		        key->active = key_press->pressed;

		    break;

		default:
		    break;
		}

		break;
	}

	fac_rm_iterador(it);

	return key;
}

void ils_send_event(struct ils_obj *obj, struct ils_evento *evento)
{
	struct ils_control *control = ils_ret_obj_control(obj);
	control->input_proc(*evento);
}

static void term_control(struct ils_control *control)
{
	struct fac_iterador *it;
	struct ils_key *key;

	if (control == NULL)
		return;

	it = fac_ini_iterador(control->keys);
	while (fac_existe_prox(it)) {
	    key = fac_proximo(it);
	    ils_term_timer(key->timer);

		free(key);
	}

	fac_rm_iterador(it);
	fac_rm_lista(control->keys);

	printf("i: controle %s finalizado.\n", control->name);
	free(control);
}

void ils_term_controls(void)
{
    struct fac_iterador *it = fac_ini_iterador(pool);

    while (fac_existe_prox(it))
        term_control(fac_proximo(it));

    fac_rm_iterador(it);
    fac_rm_lista(pool);
}
