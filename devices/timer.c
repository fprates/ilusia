/*
 * timer.c
 *
 *  Created on: 06/03/2011
 *      Author: francisco.prates
 */

#include "../ilusia.h"
#include <faclib.h>

// propriedades do timer
struct ils_timer {
    char *name;
    unsigned int tmp_max;
    unsigned int tmp_ini;
    unsigned int pausa;
};

static struct fac_lista *pool;

void ils_ini_timers(void)
{
    pool = fac_ini_lista();
}

struct ils_timer *ils_def_timer(char *name, unsigned int milisseg)
{
    struct ils_timer *timer = malloc(sizeof(*timer));

    timer->name = name;
    timer->pausa = 1;
    timer->tmp_ini = 0;
    timer->tmp_max = milisseg;

    fac_inc_item(pool, timer);

    printf("i: timer %s gerado.\n", name);

    return timer;
}

/**
 * verifica se o temporizador atingiu o limite
 */
unsigned char ils_ret_timer_mrk(struct ils_timer *timer)
{
    unsigned int tmp_atual;
    struct ils_sdl *sdl = ils_ret_sdl_fncs();

    if (timer->pausa)
        return 0;

    tmp_atual = sdl->SDL_GetTicks() - timer->tmp_ini;
    if (tmp_atual >= timer->tmp_max)
        return 1;

    return 0;
}

/**
 * verifica se o temporizador esta pausado
 */
unsigned char ils_is_timer_stopped(struct ils_timer *timer)
{
    return timer->pausa;
}

/**
 * prossegue com o temporizador
 */
void ils_start_timer(struct ils_timer *timer)
{
    struct ils_sdl *sdl = ils_ret_sdl_fncs();

    timer->pausa = 0;
    timer->tmp_ini = sdl->SDL_GetTicks();
    printf("i: timer \"%s\" has been started.\n", timer->name);
}

/**
 * interrompe o temporizador
 */
void ils_stop_timer(struct ils_timer *timer)
{
    timer->pausa = 1;
    printf("i: timer \"%s\" has been stopped.\n", timer->name);
}

/**
 * reinicia contagem
 */
void ils_reset_timer_cnt(struct ils_timer *timer)
{
    struct ils_sdl *sdl = ils_ret_sdl_fncs();

    timer->tmp_ini = sdl->SDL_GetTicks();
}

/*
 * destrói temporizador
 */
void ils_term_timer(struct ils_timer *timer)
{
    if (timer == NULL)
        return;

    free(timer);
}

void ils_term_timers(void)
{
    struct fac_iterador *it = fac_ini_iterador(pool);

    while (fac_existe_prox(it))
        ils_term_timer(fac_proximo(it));

    fac_rm_iterador(it);
    fac_rm_lista(pool);

}
