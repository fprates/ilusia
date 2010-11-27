/*
 * game.c
 *
 *  Created on: 22/11/2010
 *      Author: francisco.prates
 */

#include <stdio.h>
#include <faclib.h>
#include <curses.h>
#include "ilusia.h"

void ils_start(struct ils_obj *game, struct ils_obj *cen)
{
    int c;
	struct ils_obj *obj_;
	struct ils_evento evento;
    WINDOW *win = initscr();
	struct fac_iterador *it = ils_ret_complex_objs(cen);

	noecho();
    cbreak();
    for (;;) {
    	c = getch();
        if (c == 27)
            break;

        fac_rst_iterador(it);
        while (fac_existe_prox(it)) {
            obj_ = ils_ret_obj_from_complex(fac_proximo(it));
            evento.evcode = ils_ret_key_event(obj_, c);

            if (evento.evcode == -1)
            	continue;

            evento.obj = obj_;
            evento.cen = cen;
            evento.key_code = c;

            ils_send_event(obj_, &evento);
        }
    }

    fac_rm_iterador(it);
    endwin();
    win = NULL;

}
