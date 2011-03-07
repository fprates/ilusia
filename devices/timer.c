/*
 * timer.c
 *
 *  Created on: 06/03/2011
 *      Author: francisco.prates
 */

#include "../ilusia.h"

// propriedades do timer
struct s_timer {
    unsigned int tmp_max;
    unsigned int tmp_ini;
    unsigned int pausa;
};

struct ils_obj *ils_def_timer(char *name, unsigned int milisseg)
{
    struct ils_obj *obj = ils_def_obj(name);
    struct s_timer *timer_ = malloc(sizeof(*timer_));

    ils_def_obj_espec(obj, timer_);
    timer_->pausa = 1;
    timer_->tmp_ini = 0;
    timer_->tmp_max = milisseg;

    return obj;
}

/**
 * verifica se o temporizador atingiu o limite
 */
unsigned char ils_ret_timer_mrk(struct ils_obj *timer)
{
    unsigned int tmp_atual;
    struct s_timer *timer_ =  ils_ret_obj_espec(timer);
    struct ils_sdl *sdl = ils_ret_sdl_fncs();

    if (timer_->pausa)
        return 0;

    tmp_atual = sdl->SDL_GetTicks() - timer_->tmp_ini;
    if (tmp_atual >= timer_->tmp_max)
        return 1;

    return 0;
}

/**
 * verifica se o temporizador esta pausado
 */
unsigned char ils_is_timer_stopped(struct ils_obj *timer)
{
    struct s_timer *timer_ = ils_ret_obj_espec(timer);
    return timer_->pausa;
}

/**
 * prossegue com o temporizador
 */
void ils_start_timer(struct ils_obj *timer)
{
    struct s_timer *timer_ = ils_ret_obj_espec(timer);
    struct ils_sdl *sdl = ils_ret_sdl_fncs();

    timer_->pausa = 0;
    timer_->tmp_ini = sdl->SDL_GetTicks();
    printf("i: timer \"%s\" has been started.\n", ils_ret_obj_name(timer));
}

/**
 * interrompe o temporizador
 */
void ils_stop_timer(struct ils_obj *timer)
{
    struct s_timer *timer_ = ils_ret_obj_espec(timer);

    timer_->pausa = 1;
    printf("i: timer \"%s\" has been stopped.\n", ils_ret_obj_name(timer));
}

/**
 * reinicia contagem
 */
void ils_reset_timer_cnt(struct ils_obj *timer)
{
    struct s_timer *timer_ = ils_ret_obj_espec(timer);
    struct ils_sdl *sdl = ils_ret_sdl_fncs();

    timer_->tmp_ini = sdl->SDL_GetTicks();
    printf("i: timer \"%s\" has been reseted.\n", ils_ret_obj_name(timer));
}

/*
 * destrói temporizador
 */
void ils_term_timer(struct ils_obj *timer)
{
    struct s_timer *timer_ = ils_ret_obj_espec(timer);

    if (timer_ == NULL)
        return;

    free(timer_);
    ils_term(timer);
}
