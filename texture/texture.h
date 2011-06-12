/*
 * texture.h
 *
 *  Created on: 06/06/2011
 *      Author: francisco.prates
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../ilusia.h"

enum e_tpcolor {
    ILS_RGB,
    ILS_LUMINANCE,
    ILS_BGRA
};

extern void _ini_textures(void);

extern void _term_textures(void);

extern struct ils_texture *_def_texture(char *id);

extern int _load_texture(
        struct ils_texture *texture, enum e_tpcolor tpcolor, void *data);

extern void _set_texture(
        struct ils_texture *texture, unsigned int w, unsigned int h);

/*
 * Carrega imagens cadastradas
 */
extern void _def_img(void);

#endif /* TEXTURE_H_ */
