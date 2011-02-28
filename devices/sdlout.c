/*
 * sdlout.c
 *
 *  Created on: 27/11/2010
 *      Author: francisco.prates
 */

#include "sdl.h"

void _frame_start(void)
{

}

void _push_state(void)
{
	struct ils_gl *gl = ils_ret_gl_fncs();

	gl->glPushMatrix();
}

void _pop_state(void)
{
	struct ils_gl *gl = ils_ret_gl_fncs();

	gl->glPopMatrix();
}

void _output_commit(void)
{
	struct ils_sdl *sdl = ils_ret_sdl_fncs();

    sdl->SDL_GL_SwapBuffers();
}
