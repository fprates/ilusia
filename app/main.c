/*
 * main.c
 *
 *  Created on: 12/11/2010
 *      Author: francisco.prates
 */

#include <stdio.h>
#include <curses.h>
#include "ilusia.h"

static void input(struct ils_evento evento)
{
	struct ils_pos pos;

	switch(evento.key_code) {
	case 'A':
	case 'a':
		ils_def_relat_pos(evento.cen, evento.obj, -1, 0, 0);
		pos = ils_ret_obj_pos(evento.cen, evento.obj);
		delch();
		move(pos.y, pos.x);
		insch('X');

		break;

	case 'D':
	case 'd':
		ils_def_relat_pos(evento.cen, evento.obj, 1, 0, 0);
		pos = ils_ret_obj_pos(evento.cen, evento.obj);
		delch();
		move(pos.y, pos.x);
		insch('X');

		break;
	}
}

int main(void)
{
    struct ils_obj *game;
    struct ils_obj *cenario;
    struct ils_obj *joao;
    struct ils_obj *cao;
    struct ils_obj *lata;
    struct ils_control *control;
    struct ils_config config;

    config.title = "Exemplo";
    game = ils_ini(config);

    cenario = ils_def_obj("cenario");
    joao = ils_def_obj("joao");
    cao = ils_def_obj("cao");
    lata = ils_def_obj("lata");

    ils_inc_obj(joao, cenario);
    ils_inc_obj(cao, cenario);
    ils_inc_obj(lata, cenario);
    ils_inc_obj(cenario, game);

    ils_def_pos(joao, cenario, 1, 1, 0);
    ils_def_pos(cao, cenario, 20, 1, 0);
    ils_def_pos(lata, cenario, 40, 1, 0);

    control = ils_def_control(game, "player 1");
    ils_def_key(control, 'A');
    ils_def_key(control, 'a');
    ils_def_key(control, 'D');
    ils_def_key(control, 'd');
    ils_def_input_proc(control, input);
    ils_def_obj_control(joao, control);

    ils_start(game, cenario);
    ils_term_all(game);

    printf("That's all, folks!\n");

    return 0;
}
