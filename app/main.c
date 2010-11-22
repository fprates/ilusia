/*
 * main.c
 *
 *  Created on: 12/11/2010
 *      Author: francisco.prates
 */

#include <stdio.h>
#include "ilusia.h"

int main(void)
{
    struct ils_obj *game;
    struct ils_obj *cenario;
    struct ils_obj *joao;
    struct ils_obj *cao;
    struct ils_obj *lata;
    struct ils_config config;

    config.title = "Exemplo";
    game = ils_ini(config);

    cenario = ils_def("cenario");
    joao = ils_def("joao");
    cao = ils_def("cao");
    lata = ils_def("lata");

    ils_inc(joao, cenario);
    ils_inc(cao, cenario);
    ils_inc(lata, cenario);
    ils_inc(cenario, game);

    ils_start(game);
    ils_term_all(game);

    return 0;
}
