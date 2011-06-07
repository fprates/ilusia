/*
 * controls.h
 *
 *  Created on: 06/06/2011
 *      Author: francisco.prates
 */

#ifndef CONTROLS_H_
#define CONTROLS_H_

extern void _ini_controls(void);

extern void _term_controls(void);

/*
 * Retorna evento de um controle
 */
extern struct ils_key *_ret_key_event(struct ils_obj *obj,
        struct ils_key_press *key_press);

/*
 * Retorna evento de um bot
 */
extern struct ils_key *_ret_bot_event(struct ils_obj *cen,
        struct ils_obj *obj);

#endif /* CONTROLS_H_ */
