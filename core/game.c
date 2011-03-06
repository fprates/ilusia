/*
 * game.c
 *
 *  Created on: 22/11/2010
 *      Author: francisco.prates
 */

#include <stdio.h>
#include <faclib.h>
#include "object.h"
#include "../ilusia.h"
#include "../devices/sdl.h"

struct {
    struct ils_config *config;
    int ret;
} system_;

void ils_def_signal(int signal)
{
    switch (signal) {
    case ILS_TERM:
        system_.ret = 1;
        break;
    }
}

void ils_start(struct ils_obj *game, struct ils_obj *cen,
        struct ils_config config)
{
    struct ils_key_press key_press;
	struct ils_obj *obj_;
	struct ils_evento evento;
	struct fac_iterador *it = ils_ret_complex_objs(cen);

	system_.config = &config;

    if (_ini_devices(system_.config) < 0) {
        printf("e: erro na inicialização dos dispositivos.\n");
        return;
    }

    printf("i: aguardando eventos...\n");
    system_.ret = 0;

    while (!system_.ret) {
    	key_press = _ret_key_pressed();

    	_frame_start();

    	_push_state();
    	_call_output_proc(cen, cen);
    	_pop_state();

        fac_rst_iterador(it);
        while (fac_existe_prox(it)) {
            obj_ = ils_ret_obj_from_complex(fac_proximo(it));

            evento.evcode = ils_ret_key_event(obj_, &key_press);

            _push_state();
            _call_output_proc(cen, obj_);
            _pop_state();

            if (evento.evcode == -1)
            	continue;

            evento.obj = obj_;
            evento.cen = cen;
            evento.key_code = key_press.code;

            ils_send_event(obj_, &evento);
        }

        _output_commit();
    }

    fac_rm_iterador(it);

    _term_devices();

}
