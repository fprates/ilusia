/*
 * sdlin.c
 *
 *  Created on: 27/11/2010
 *      Author: francisco.prates
 */

#include "sdl.h"

struct ils_key_press _ret_key_pressed(void)
{
    SDL_Event event;
    struct ils_key_press key_press;
    struct ils_sdl *sdl = ils_ret_sdl_fncs();

    key_press.code = -1;
    key_press.pressed = -1;

    if (sdl->SDL_PollEvent(&event)) {
        key_press.code = event.key.keysym.sym;
        switch (event.type) {
        case SDL_KEYDOWN:
            key_press.pressed = 1;
            break;

        case SDL_KEYUP:
            key_press.pressed = 0;
            break;

        }
    }

    return key_press;
}

