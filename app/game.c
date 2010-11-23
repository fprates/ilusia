/*
 * game.c
 *
 *  Created on: 22/11/2010
 *      Author: francisco.prates
 */

#include <stdio.h>
//#include <termios.h>
#include <faclib.h>
#include "ilusia.h"

void ils_start(struct ils_obj *game, struct ils_obj *obj)
{
//    int c;
//    struct ils_obj *obj;
//    struct termios term_orig;
//    struct termios term_dest;
//    struct fac_iterador *it;
//
//    tcgetattr(0,&term_orig);
//
//    term_dest = term_orig;
//    term_dest.c_lflag&=~ICANON; //ignora ENTER
//    term_dest.c_lflag&=~ECHO; //não ecoa
//
//    tcsetattr(0,TCSANOW,&term_dest);
//
//    for (;;) {
//        c = getchar();
//
//        if (c == 27)
//            break;
//
//        it = ils_ret_objs(game);
//        while (fac_existe_prox(it)) {
//            obj = fac_proximo(it);
//        }
//
//        fac_rm_iterador(it);
//    }
//
//    tcsetattr(0,TCSANOW,&term_orig);
}
