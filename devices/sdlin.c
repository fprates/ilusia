/*
 * sdlin.c
 *
 *  Created on: 27/11/2010
 *      Author: francisco.prates
 */

#include "sdl.h"

int _ret_key_code(void)
{
    SDL_Event event;
    struct ils_sdl *sdl = ils_ret_sdl_fncs();

    while (sdl->SDL_PollEvent(&event)) {
        if (event.type != SDL_KEYDOWN)
            continue;

        return event.key.keysym.sym;
    }

    return 0;
}

