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
#include "../core/controls.h"
#include "../devices/sdl.h"
#include "../texture/text.h"
#include "../texture/texture.h"
#include <sys/stat.h>
#include <sys/types.h>

struct {
    struct ils_config *config;
    int ret;
} system_;

enum e_line_type {
    UNDEF,
    PROPERTY,
    COMMAND
};

struct s_command {
    struct ils_obj *game;
    struct ils_config *config;
    enum e_line_type type;
    struct fac_lista *parms;
};

static void inc_param(char *arg, struct s_command *command)
{
    char *parg;

    parg = malloc(strlen(arg)+1);
    strcpy(parg, &arg[0]);

    fac_inc_item(command->parms, parg);
}

static void parse(struct s_command *command, char *line)
{
    char arg[255];
    unsigned int c = 0;
    unsigned char k = 0;

    command->parms = fac_ini_lista();
    command->type = UNDEF;

    for (;;) {
        switch (*(line+c)) {
        case '\0':
        case '\n':
            arg[k++] = 0;
            inc_param(&arg[0], command);

            return;

        case '=':
            if (command->type == COMMAND) {
                arg[k++] = *(line+c);
                break;
            }

            arg[k] = 0;
            command->type = PROPERTY;
            k = 0;

            inc_param(&arg[0], command);

            break;

        case ' ':
            if (command->type == PROPERTY) {
                arg[k++] = *(line+c);
                break;
            }

            command->type = COMMAND;
            arg[k] = 0;
            k = 0;

            inc_param(&arg[0], command);

            break;

        default:
            arg[k++] = *(line+c);

            break;
        }

        c++;
    }
}

static void exec_pipeline(struct s_command *command)
{
    char *parameter;
    struct ils_obj *orig;
    struct ils_obj *dest;
    struct fac_iterador *it = fac_ini_iterador(command->parms);

    while (fac_existe_prox(it)) {
        parameter = fac_proximo(it);

        switch (command->type) {
        case PROPERTY:
            if (strcmp(parameter, "title") == 0) {
                command->config->title = fac_proximo(it);
                break;
            }

            break;

        case COMMAND:
            if (strcmp(parameter, "def_obj") == 0) {
                ils_def_obj(fac_proximo(it));
                break;
            }

            if (strcmp(parameter, "def_game") == 0) {
                command->game = ils_def_obj(fac_proximo(it));
                break;
            }

            if (strcmp(parameter, "inc_obj") == 0) {
                orig = ils_ret_obj_by_name(fac_proximo(it));
                dest = ils_ret_obj_by_name(fac_proximo(it));
                ils_inc_obj(orig, dest);
                break;
            }

            break;

        default:
            break;
        }
    }

    fac_rm_iterador(it);
    fac_rm_lista(command->parms);
}

static void execute(struct s_command *command, char *line)
{
    parse(command, line);
    exec_pipeline(command);
}

struct ils_obj *ils_load(char *filename, struct ils_config *config)
{
    FILE *fd;
    struct stat fd_stat;
    char *buffer;
    char line[255];
    unsigned char k;
    unsigned int c = 0;
    struct s_command command;

    if (filename == NULL)
        return NULL;

    command.game = NULL;

    fd = fopen(filename, "rb");
    if (fd == NULL)
        return NULL;

    fstat(fileno(fd), &fd_stat);

    buffer = malloc(fd_stat.st_size);

    if (fread(buffer, sizeof(buffer), fd_stat.st_size, fd) == 0)
        goto load_term;

    while (c < fd_stat.st_size) {
        for (k = 0; k < 255; k++) {
            line[k] = *(buffer+(c++));
            if (line[k] == '\0' || line[k] == '\n')
                break;
        }

        execute(&command, &line[0]);
    }

load_term:
    fclose(fd);

    free(buffer);

    return command.game;
}

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
    struct ils_key *key;
	struct ils_obj *obj_;
	struct ils_timer *timer;
	struct ils_evento evento;
	struct fac_iterador *it;

	if (game == NULL) {
	    printf("e: objeto de jogo não inicializado.\n");
	    return;
	}

	if (cen == NULL) {
	    printf("e: cenário não inicializado.\n");
	    return;
	}

	it = _ret_complex_objs(cen);

	system_.config = &config;

    if (_ini_devices(system_.config) < 0) {
        printf("e: erro na inicialização dos dispositivos.\n");
        return;
    }

    _def_img();
    _def_fonts();

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
            obj_ = _ret_obj_from_complex(fac_proximo(it));

            switch (ils_ret_event_source(obj_)) {
            case ILS_KEY:
                key = _ret_key_event(obj_, &key_press);
                break;

            case ILS_BOT:
            	key = _ret_bot_event(cen, obj_);
            	break;
            }

            _push_state();
            _call_output_proc(cen, obj_);
            _pop_state();

            if (config.global_proc != NULL)
                config.global_proc(cen, obj_);

            if (key == NULL)
            	continue;

            evento.evcode = ils_ret_event_code(key);
            evento.obj = obj_;
            evento.cen = cen;
            evento.key_code = key_press.code;

            timer = ils_ret_key_timer(key);

            if (timer != NULL) {
                if (ils_is_timer_stopped(timer)) {
                    ils_start_timer(timer);
                    continue;
                }

                if (!ils_ret_timer_mrk(timer))
                    continue;
                else
                    ils_reset_timer_cnt(timer);
            }

            ils_send_event(obj_, &evento);
        }

        _output_commit();
    }

    fac_rm_iterador(it);

    _term_devices();
}
