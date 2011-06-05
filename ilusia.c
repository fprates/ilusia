/*
 * ilusia.c
 *
 *  Created on: 11/04/2011
 *      Author: francisco.prates
 */

#include "ilusia.h"
#include <faclib.h>

void ils_ini(struct ils_config *config)
{
	ils_ini_objects();
    ils_ini_controls();
    ils_ini_textures();
    ils_ini_timers();

    config->global_proc = NULL;
}

void ils_term(struct ils_obj *obj)
{
    ils_term_objects();
    ils_term_controls();
    ils_term_textures();
    ils_term_timers();
}
