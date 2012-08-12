/*
 * ilusia.c
 *
 *  Created on: 11/04/2011
 *      Author: francisco.prates
 */

#include <faclib.h>
#include "object.h"
#include "controls.h"
#include "../ilusia.h"
#include "../texture/texture.h"
#include "../devices/timer.h"
#include "../texture/text.h"

void ils_ini(struct ils_config *config)
{
    _ini_objects();
    _ini_controls();
    _ini_textures();
    _ini_timers();
    _ini_fonts();

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
    _term_objects();
    _term_controls();
    _term_textures();
    _term_timers();
    _term_fonts();
}
