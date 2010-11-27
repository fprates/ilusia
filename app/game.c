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
    WINDOW *win = initscr();
	struct fac_iterador *it = ils_ret_objs(cen);

    cbreak();
    for (;;) {
    	c = getch();
        if (c == 27)
            break;

        fac_rst_iterador(it);
        while (fac_existe_prox(it)) {
            obj_ = fac_proximo(it);
            if (ils_verif_key_event(obj_, c))
            	ils_send_event(obj_, c);
        }
    }

    fac_rm_iterador(it);
    endwin();
    win = NULL;

}
