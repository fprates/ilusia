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

struct fac_lista *pool = NULL;

struct ils_control {
    char *name;
    struct fac_lista *keys;
    void (*input_proc)(struct ils_control *);
};

struct s_key {
    int code;
};

void ils_ini_controls()
{
    pool = fac_ini_lista();
    printf("i: pool de controles inicializado.\n");
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

void ils_term_controls(void)
{
    struct ils_control *control;
    struct fac_iterador *it = fac_ini_iterador(pool);

    /*
     * remove controles individuais
     */
    while (fac_existe_prox(it)) {
        control = fac_proximo(it);
        fac_rm_lista(control->keys);

        printf("i: controle %s finalizado.\n", control->name);
        free(control);
    }

    fac_rm_iterador(it);
    fac_rm_lista(pool);
}
