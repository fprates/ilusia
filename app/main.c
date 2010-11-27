/*
 * main.c
 *
 *  Created on: 12/11/2010
 *      Author: francisco.prates
 */

#include <stdio.h>
#include <curses.h>
#include <string.h>
#include "../lib/ilusia.h"

enum actions {LEFT, RIGHT};
struct ils_pos pos_;

static void output(struct ils_view view)
{
	struct ils_pos pos = ils_ret_obj_pos(view.cen, view.obj);
	const char *name = ils_ret_name(view.obj);

	move(pos_.y, pos_.x);
	delch();
	move(pos.y, pos.x);

	if (strcmp(name, "joao") == 0)
		insch('J');
	else
		insch('C');
}

static void input(struct ils_evento evento)
{
	switch(evento.evcode) {
	case LEFT:
		pos_ = ils_ret_obj_pos(evento.cen, evento.obj);
		ils_def_relat_pos(evento.cen, evento.obj, -1, 0, 0);

		break;

	case RIGHT:
		pos_ = ils_ret_obj_pos(evento.cen, evento.obj);
		ils_def_relat_pos(evento.cen, evento.obj, 1, 0, 0);

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
    struct ils_control *control1;
    struct ils_control *control2;
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

    control1 = ils_def_control(game, "player 1");
    ils_def_key(control1, LEFT, 'A');
    ils_def_key(control1, LEFT, 'a');
    ils_def_key(control1, RIGHT, 'D');
    ils_def_key(control1, RIGHT, 'd');

    control2 = ils_def_control(game, "player 2");
    ils_def_key(control2, LEFT, 'J');
    ils_def_key(control2, LEFT, 'j');
    ils_def_key(control2, RIGHT, 'L');
    ils_def_key(control2, RIGHT, 'l');

    ils_def_output_proc(joao, output);
    ils_def_output_proc(cao, output);
    ils_def_input_proc(control1, input);
    ils_def_input_proc(control2, input);

    ils_def_obj_control(joao, control1);
    ils_def_obj_control(cao, control2);

    ils_start(game, cenario);
    ils_term_all(game);

    printf("That's all, folks!\n");

    return 0;
}
