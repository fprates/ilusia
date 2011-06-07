/*
 * texture.h
 *
 *  Created on: 06/06/2011
 *      Author: francisco.prates
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../ilusia.h"

extern struct ils_texture *_def_texture(char *id);

extern int _load_texture(
        struct ils_texture *texture, unsigned int comp, void *data);

extern void _set_texture(
        struct ils_texture *texture, unsigned int w, unsigned int h);

#endif /* TEXTURE_H_ */
