/*
 * sdl.c
 *
 *  Created on: 03/12/2010
 *      Author: francisco.prates
 */

#include <faclib.h>
#include "sdl.h"

static struct s_sdl {
    void *lib;
    void *ttf;
    void *img;
    void *glu;
    SDL_Surface *screen;
    struct ils_sdl fnc;
    struct ils_gl gl;
} sdl;

static int ini_video(struct ils_config *config, struct s_sdl *sdl_)
{
	struct ils_gl *gl = &sdl_->gl;

    sdl_->screen = sdl_->fnc.SDL_SetVideoMode(
            config->video.w,
            config->video.h,
            config->video.bpp,
            SDL_OPENGL
    );

    if (sdl_->screen == NULL)
        return -1;

    gl->glViewport(
    		config->view.x, config->view.y, config->view.dw, config->view.dh);

    gl->glMatrixMode(GL_PROJECTION);
    gl->glLoadIdentity();

    gl->gluPerspective(config->persp.fovy, config->persp.aspec,
    		config->persp.zprox, config->persp.zdist);

    gl->gluLookAt(config->camera.obsx, config->camera.obsy, config->camera.obsz,
    		config->camera.objx, config->camera.objy, config->camera.objz,
    		config->camera.vupx, config->camera.vupy, config->camera.vupz);

    if (sdl_->fnc.TTF_Init() < 0)
        return -1;

    return 0;
}

static int def_sdl_fnc(struct s_sdl *sdl_)
{
    sdl_->fnc.SDL_Init = fac_ret_proc_lib(sdl_->lib, "SDL_Init");

    if (sdl_->fnc.SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return -1;

    sdl_->fnc.SDL_FreeSurface = fac_ret_proc_lib(sdl_->lib, "SDL_FreeSurface");
    sdl_->fnc.SDL_GetTicks = fac_ret_proc_lib(sdl_->lib, "SDL_GetTicks");
    sdl_->fnc.SDL_GL_GetProcAddress = fac_ret_proc_lib(
            sdl_->lib, "SDL_GL_GetProcAddress");
    sdl_->fnc.SDL_GL_LoadLibrary = fac_ret_proc_lib(
            sdl_->lib, "SDL_GL_LoadLibrary");
    sdl_->fnc.SDL_GL_SetAttribute = fac_ret_proc_lib(
            sdl_->lib, "SDL_GL_SetAttribute");
    sdl_->fnc.SDL_GL_SwapBuffers = fac_ret_proc_lib(
            sdl_->lib, "SDL_GL_SwapBuffers");
    sdl_->fnc.SDL_PollEvent = fac_ret_proc_lib(sdl_->lib, "SDL_PollEvent");
    sdl_->fnc.SDL_SetVideoMode = fac_ret_proc_lib(sdl_->lib, "SDL_SetVideoMode");
    sdl_->fnc.SDL_WM_SetCaption = fac_ret_proc_lib(sdl_->lib, "SDL_WM_SetCaption");
    sdl_->fnc.SDL_LockSurface = fac_ret_proc_lib(sdl_->lib, "SDL_LockSurface");
    sdl_->fnc.SDL_UnlockSurface = fac_ret_proc_lib(sdl_->lib, "SDL_UnlockSurface");
    sdl_->fnc.SDL_CreateRGBSurface = fac_ret_proc_lib(
            sdl_->lib, "SDL_CreateRGBSurface");
    sdl_->fnc.SDL_BlitSurface = fac_ret_proc_lib(sdl_->lib, "SDL_UpperBlit");
    sdl_->fnc.SDL_Quit = fac_ret_proc_lib(sdl_->lib, "SDL_Quit");
    sdl_->fnc.SDL_GetTicks = fac_ret_proc_lib(sdl_->lib, "SDL_GetTicks");

    /*
     * Funções SDL TTF
     */
    sdl_->fnc.TTF_Init = fac_ret_proc_lib(sdl_->ttf, "TTF_Init");
    sdl_->fnc.TTF_OpenFont = fac_ret_proc_lib(sdl_->ttf, "TTF_OpenFont");
    sdl_->fnc.TTF_CloseFont = fac_ret_proc_lib(sdl_->ttf, "TTF_CloseFont");
    sdl_->fnc.TTF_RenderText_Blended = fac_ret_proc_lib(sdl_->ttf,
        "TTF_RenderText_Blended");

    /*
     * Funções SDL IMG
     */
    sdl_->fnc.IMG_Load = fac_ret_proc_lib(sdl.img, "IMG_Load");

    printf("i: funções SDL carregadas.\n");

    return 0;
}

static int def_gl_fnc(struct s_sdl *sdl_)
{
	struct ils_gl *gl = &sdl_->gl;
	struct ils_sdl *sdl = &sdl_->fnc;

    gl->glBegin = sdl->SDL_GL_GetProcAddress("glBegin");
    gl->glBindTexture = sdl->SDL_GL_GetProcAddress("glBindTexture");
    gl->glBlendFunc = sdl->SDL_GL_GetProcAddress("glBlendFunc");
    gl->glClear = sdl->SDL_GL_GetProcAddress("glClear");
    gl->glClearColor = sdl->SDL_GL_GetProcAddress("glClearColor");
    gl->glColor3f = sdl->SDL_GL_GetProcAddress("glColor3f");
    gl->glColor4f = sdl->SDL_GL_GetProcAddress("glColor4f");
    gl->glDeleteTextures = sdl->SDL_GL_GetProcAddress("glDeleteTextures");
    gl->glDisable = sdl->SDL_GL_GetProcAddress("glDisable");
    gl->glEnable = sdl->SDL_GL_GetProcAddress("glEnable");
    gl->glEnd = sdl->SDL_GL_GetProcAddress("glEnd");
    gl->glGenTextures = sdl->SDL_GL_GetProcAddress("glGenTextures");
    gl->glLoadIdentity = sdl->SDL_GL_GetProcAddress("glLoadIdentity");
    gl->glMatrixMode = sdl->SDL_GL_GetProcAddress("glMatrixMode");
    gl->glNormal3f = sdl->SDL_GL_GetProcAddress("glNormal3f");
    gl->glOrtho = sdl->SDL_GL_GetProcAddress("glOrtho");
    gl->glPixelStoref = sdl->SDL_GL_GetProcAddress("glPixelStoref");
    gl->glPixelStorei = sdl->SDL_GL_GetProcAddress("glPixelStorei");
    gl->glPopMatrix = sdl->SDL_GL_GetProcAddress("glPopMatrix");
    gl->glPushMatrix = sdl->SDL_GL_GetProcAddress("glPushMatrix");
    gl->glRotatef = sdl->SDL_GL_GetProcAddress("glRotatef");
    gl->glScalef = sdl->SDL_GL_GetProcAddress("glScalef");
    gl->glTranslatef = sdl->SDL_GL_GetProcAddress("glTranslatef");
    gl->glVertex2d = sdl->SDL_GL_GetProcAddress("glVertex2d");
    gl->glVertex2f = sdl->SDL_GL_GetProcAddress("glVertex2f");
    gl->glVertex3f = sdl->SDL_GL_GetProcAddress("glVertex3f");
    gl->glViewport = sdl->SDL_GL_GetProcAddress("glViewport");
    gl->glTexGeni = sdl->SDL_GL_GetProcAddress("glTexGeni");
    gl->glTexGenfv = sdl->SDL_GL_GetProcAddress("glTexGenfv");
    gl->glTexParameterf = sdl->SDL_GL_GetProcAddress("glTexParameterf");
    gl->glTexParameteri = sdl->SDL_GL_GetProcAddress("glTexParameteri");
    gl->glTexImage2D = sdl->SDL_GL_GetProcAddress("glTexImage2D");
    gl->glTexCoord2f = sdl->SDL_GL_GetProcAddress("glTexCoord2f");
    gl->glTexCoord3f = sdl->SDL_GL_GetProcAddress("glTexCoord3f");
    gl->glTexEnvi = sdl->SDL_GL_GetProcAddress("glTexEnvi");

    gl->gluLookAt = fac_ret_proc_lib(sdl_->glu, "gluLookAt");
    gl->gluOrtho2D = fac_ret_proc_lib(sdl_->glu, "gluOrtho2D");
    gl->gluPerspective = fac_ret_proc_lib(sdl_->glu, "gluPerspective");
    gl->gluBuild2DMipmaps = fac_ret_proc_lib(sdl_->glu, "gluBuild2DMipmaps");

    printf("i: funções OpenGL carregadas.\n");

    return 0;
}

static int sdl_config(char *gllibname, struct s_sdl *sdl_)
{
	struct ils_sdl *sdl = &sdl_->fnc;

	/* following on-line tutorial:
	 *
	 * SDL has portable support for loading OpenGL function pointers:
	 *   SDL_GL_LoadLibrary and SDL_GL_GetProcAddress.
	 *
	 * Note that SDL_SetVideoMode should be called after SDL_GL_LoadLibrary
	 *   and before SDL_GL_GetProcAddress.
	 */
	if (sdl->SDL_GL_LoadLibrary(gllibname) < 0)
		return -1;

	sdl->SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return 0;
}

int _ini_devices(struct ils_config *config)
{
    sdl.lib = fac_ld_lib(ILS_SDL);
    if (sdl.lib == NULL)
        return -1;

    sdl.ttf = fac_ld_lib(ILS_TTF);
    if (sdl.ttf == NULL)
        return -1;

    sdl.img = fac_ld_lib(ILS_IMG);
    if (sdl.img == NULL)
        return -1;

    sdl.glu = fac_ld_lib(ILS_GLU);
    if (sdl.glu == NULL)
    	return -1;

    if (def_sdl_fnc(&sdl) < 0)
        return -2;

    if (sdl_config(ILS_GL, &sdl) < 0)
    	return -3;

    if (def_gl_fnc(&sdl) < 0)
    	return -4;

    ini_video(config, &sdl);

    return 0;
}

void _term_devices(void)
{
    if ((sdl.lib == NULL) || (sdl.ttf == NULL) || (sdl.img == NULL))
        return;

    sdl.fnc.SDL_FreeSurface(sdl.screen);
    sdl.fnc.SDL_Quit();

    fac_lib_term(sdl.lib);
    fac_lib_term(sdl.ttf);
    fac_lib_term(sdl.img);
    fac_lib_term(sdl.glu);
}

struct ils_sdl *ils_ret_sdl_fncs(void)
{
    return &sdl.fnc;
}

struct ils_gl *ils_ret_gl_fncs(void)
{
	return &sdl.gl;
}
