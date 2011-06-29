/*
 * ilusia.h
 *
 *  Created on: 18/11/2010
 *      Author: francisco.prates
 */

#ifndef ILUSIA_H_
#define ILUSIA_H_

#include "render.h"

enum ils_signal {ILS_TERM};

enum ils_keypress {
    ILS_CONTINUOUS,
    ILS_RELEASED,
    ILS_PRESSED,
    ILS_PRESSED_RELEASED
};

enum ils_e_texture {
	DONT_INVERT, INVERT
};

enum ils_event_source {
	ILS_KEY, ILS_BOT
};

struct ils_obj;
struct ils_complex_obj;
struct ils_control;
struct ils_timer;
struct ils_font;
struct ils_key;
struct ils_texture;

struct ils_video {
    unsigned int w, h;
    unsigned int bpp;
};

struct ils_pos {
    float x, y, z;
    float sw, sh, sd;
    float dw, dh, dd;
};

struct ils_persp {
    float fovy;
    float aspec;
    float zprox;
    float zdist;
};

struct ils_camera {
    float obsx, obsy, obsz;
    float objx, objy, objz;
    float vupx, vupy, vupz;
};

struct ils_config {
    char *title;
    struct ils_video video;
    struct ils_pos view;
    struct ils_persp persp;
    struct ils_camera camera;
    void (*global_proc)(struct ils_obj *cen, struct ils_obj *obj);
};

struct ils_view {
    struct ils_obj *cen;
    struct ils_obj *obj;
};

struct ils_evento {
    struct ils_obj *obj;
    struct ils_obj *cen;
    int key_code;
    int evcode;
};

struct ils_key_press {
    SDLKey code;
    char pressed;
};

struct ils_cor {
    float r, g, b, a;
};

/*
 * Inicializa módulos da biblioteca
 */
extern void ils_ini(struct ils_config *config);

/*
 * Gera instância de um objeto do jogo
 */
extern struct ils_obj *ils_def_obj(char *obj);

/*
 * Retorna objeto pelo nome
 */
extern struct ils_obj *ils_ret_obj_by_name(char *name);

/*
 * Atribui dados customizados à um objeto
 */
extern void ils_def_obj_espec(struct ils_obj *obj, void *espec);

/*
 * Retorna dados customizados de um objeto
 */
extern void *ils_ret_obj_espec(struct ils_obj *obj);

/*
 * Retorna nome do objeto
 */
extern char *ils_ret_obj_name(struct ils_obj *obj);

/*
 * Atribui um objeto à outro
 */
extern void ils_inc_obj(struct ils_obj *orig, struct ils_obj *dest);

/*
 * Define posição absoluta de um objeto no cenário
 */
extern void ils_def_pos(struct ils_obj *obj, struct ils_obj *cen,
		float x, float y, float z);

/*
 * Define escala absoluta do objeto no cenário
 */
extern void ils_def_scale(struct ils_obj *obj, struct ils_obj *cen,
        float w, float h, float d);

/*
 * Define dimensão do objeto
 */
extern void ils_def_dim(struct ils_obj *obj, float w, float h, float d);

/*
 * Define posição relativa de um objeto no cenário
 */
extern void ils_def_relat_pos(struct ils_obj *obj, struct ils_obj *cen,
		float x, float y, float z);

/*
 * Retorna posição do objeto no cenário
 */
extern struct ils_pos ils_ret_obj_pos(
        struct ils_obj *obj, struct ils_obj *cen);

/*
 * Habilita objeto para o cenário
 */
extern void ils_set_obj_enabled(
        struct ils_obj *orig, struct ils_obj *dest, unsigned char enabled);


/*
 * Retorna se objeto está habilitado para cenário
 */
extern unsigned char ils_is_obj_enabled(struct ils_obj *orig, struct ils_obj *dest);

/*
 * Define rotina de processamento visual de um objeto
 */
extern void ils_def_output_proc(struct ils_obj *, void (*)(struct ils_view));

/*
 * Gera instância de um controle
 */
extern struct ils_control *ils_def_control(char *control_name);

/*
 * Adiciona tecla à um evento no controle
 */
extern struct ils_key *ils_def_key(struct ils_control *, int, int,
        enum ils_keypress);

/*
 * Define rotina de processamento do bot à um controle
 */
extern void ils_def_bot_proc(struct ils_control *control,
		unsigned int (*bot_proc)(struct ils_obj *cen, struct ils_obj *obj));

/*
 * Retorna código do evento
 */
extern int ils_ret_event_code(struct ils_key *);

/*
 * Define tempo de repetição para uma tecla
 */
extern void ils_def_key_timer(struct ils_key *, unsigned int);

/*
 * Retorna timer de uma tecla
 */
extern struct ils_timer *ils_ret_key_timer(struct ils_key *);

/*
 * Define rotina de processamento de enrada para um controle
 */
extern void ils_def_input_proc(struct ils_control *control,
		void (*input_proc)(struct ils_evento));

/*
 * Associa um controle à um objeto
 */
extern void ils_def_obj_control(struct ils_obj *obj,
		struct ils_control *control, enum ils_event_source event_source);

/*
 * Retorna o controle de um objeto
 */
extern struct ils_control *ils_ret_obj_control(struct ils_obj *obj);

/*
 * Retorna a fonte do evento de um objeto
 */
extern enum ils_event_source ils_ret_event_source(struct ils_obj *obj);

/*
 * Define um status para um objeto
 */
extern void ils_def_obj_status(struct ils_obj *obj, unsigned long);

/*
 * Verifica status de um objeto
 */
extern unsigned char ils_test_obj_status(
        struct ils_obj *obj, unsigned int status);

/*
 * Ajuste status de objeto
 */
extern void ils_set_obj_status(struct ils_obj *obj, unsigned int status);

/*
 * Limpa status de um objeto
 */
extern void ils_reset_obj_status(struct ils_obj *obj, unsigned int status);

/*
 * Envia evento para fila
 */
extern void ils_send_event(struct ils_obj *, struct ils_evento *);

/*
 * Gera um temporizador
 */
extern struct ils_timer *ils_def_timer(char *name, unsigned int tempo);

/*
 * Verifica se o temporizador atingiu o limite
 */
extern unsigned char ils_ret_timer_mrk(struct ils_timer *timer);

/*
 * Verifica se o temporizador está parado
 */
extern unsigned char ils_is_timer_stopped(struct ils_timer *timer);

/*
 * Inicia contagem do temporizador
 */
extern void ils_start_timer(struct ils_timer *timer);

/*
 * Interrompe contagem do temporizador
 */
extern void ils_stop_timer(struct ils_timer *timer);

/*
 * Reinicia contagem do temorizador
 */
extern void ils_reset_timer_cnt(struct ils_timer *timer);

/*
 * Libera aloção de um temporizador
 */
extern void ils_term_timer(struct ils_timer *timer);

/*
 * Retorna ponteiros para funções SDL
 */
extern struct ils_sdl *ils_ret_sdl_fncs(void);

/*
 * Retorna ponteiros para funções GL
 */
extern struct ils_gl *ils_ret_gl_fncs(void);

/*
 * Gera objetos à partir de arquivo
 */
extern struct ils_obj *ils_load(char *filename, struct ils_config *config);

/*
 * Insere sinal na fila
 */
extern void ils_def_signal(int signal);

/*
 * Inicializa processamento dos objetos
 */
extern void ils_start(
		struct ils_obj *game, struct ils_obj *cen, struct ils_config config);

/*
 * Libera alocação dos objetos do jogo
 */
extern void ils_term(struct ils_obj *game);

/*
 * Ajuste texto de um objeto de texto
 */
extern void ils_set_text(
        struct ils_obj *obj, char *text, struct ils_font *font);

/*
 * Cadastra fonte para posterior utilização
 */
extern struct ils_font *ils_def_font(
        char *id, char *fontname, unsigned int size, struct ils_cor *cor);

/*
 * Gera um objeto de texto
 */
extern struct ils_obj *ils_def_text_obj(char *name);

/*
 * Cadastra textura para posterior atualização
 */
extern struct ils_texture *ils_texture_inc(char *name, char *path);

/*
 * Renderiza textura
 */
extern void ils_show_texture(struct ils_texture *texture, struct ils_pos *pos,
		unsigned int mode);

#endif /* ILUSIA_H_ */
