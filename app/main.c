/*
 * main.c
 *
 *  Created on: 12/11/2010
 *      Author: francisco.prates
 */

#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include "../lib/ilusia.h"

enum actions {TERM, LEFT, RIGHT};

struct ils_pos pos_;

static void output(struct ils_view view)
{

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

    case TERM:
        ils_def_signal(ILS_TERM);

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

    game = ils_def_obj("exemplo");
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

    ils_ini_controls();
    control1 = ils_def_control("player 1");
    ils_def_key(control1, LEFT, SDLK_a);
    ils_def_key(control1, RIGHT, SDLK_d);
    ils_def_key(control1, TERM, SDLK_ESCAPE);

    control2 = ils_def_control("player 2");
    ils_def_key(control2, LEFT, SDLK_LEFT);
    ils_def_key(control2, RIGHT, SDLK_RIGHT);
    ils_def_key(control2, TERM, SDLK_ESCAPE);

    ils_def_output_proc(joao, output);
    ils_def_output_proc(cao, output);
    ils_def_input_proc(control1, input);
    ils_def_input_proc(control2, input);

    ils_def_obj_control(joao, control1);
    ils_def_obj_control(cao, control2);

    config.title = "Teste";
    config.video.w = 800;
    config.video.h = 600;
    config.video.bpp = 24;

    ils_start(game, cenario, config);

    ils_term_all(game);
    ils_term_controls();

    printf("That's all, folks!\n");

    return 0;
}
