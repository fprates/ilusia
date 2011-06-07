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
    ils_ini_fonts();

    config->video.w = 800;
    config->video.h = 600;
    config->video.bpp = 24;
    config->title = "";

    config->view.x = 0;
    config->view.y = 0;
    config->view.dw = 1200;
    config->view.dh = 780;

    config->persp.fovy = 60;
    config->persp.aspec = 1;
    config->persp.zprox = 1;
    config->persp.zdist = 50;

    config->camera.objx = 0;
    config->camera.objy = 0;
    config->camera.objz = 0;
    config->camera.obsx = 0;
    config->camera.obsy = 0;
    config->camera.obsz = 10;
    config->camera.vupx = 0;
    config->camera.vupy = 1;
    config->camera.vupz = 0;

    config->global_proc = NULL;
}

void ils_term(struct ils_obj *obj)
{
    ils_term_objects();
    ils_term_controls();
    ils_term_textures();
    ils_term_timers();
    ils_term_fonts();
}
