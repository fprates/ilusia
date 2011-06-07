/*
 * render.h
 *
 *  Created on: 27/02/2011
 *      Author: francisco.prates
 */

#ifndef RENDER_H_
#define RENDER_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_opengl.h>

/*
 * ponteiros para funcoes OpenGL.
 */
struct ils_gl {
    void APIENTRY (*glBegin)(GLenum);
    void APIENTRY (*glBlendFunc)(GLenum, GLenum);
    void APIENTRY (*glClear)(GLbitfield);
    void APIENTRY (*glClearColor)(GLclampf, GLclampf, GLclampf, GLclampf);
    void APIENTRY (*glColor3f)(GLfloat, GLfloat, GLfloat);
    void APIENTRY (*glColor4f)(GLfloat, GLfloat, GLfloat, GLfloat);
    void APIENTRY (*glDisable)(GLenum);
    void APIENTRY (*glEnable)(GLenum);
    void APIENTRY (*glEnd)(void);
    void APIENTRY (*glLoadIdentity)(void);
    void APIENTRY (*glMatrixMode)(GLenum);
    void APIENTRY (*glOrtho)(GLdouble, GLdouble, GLdouble,
    		GLdouble, GLdouble, GLdouble);
    void APIENTRY (*glPixelStoref)(GLenum, GLfloat);
    void APIENTRY (*glPixelStorei)(GLenum, GLint);
    void APIENTRY (*glPopMatrix)(void);
    void APIENTRY (*glPushMatrix)(void);
    void APIENTRY (*glRotatef)(GLfloat, GLfloat, GLfloat, GLfloat);
    void APIENTRY (*glScalef)(GLfloat, GLfloat, GLfloat);
    void APIENTRY (*glTexEnvi)(GLenum, GLenum, GLint);
    void APIENTRY (*glTranslatef)(GLfloat, GLfloat, GLfloat);
    void APIENTRY (*glVertex3f)(GLfloat, GLfloat, GLfloat);
    void APIENTRY (*glViewport)(GLint, GLint, GLsizei, GLsizei);
    void APIENTRY (*glGenTextures)(GLsizei, GLuint *);
    void APIENTRY (*glBindTexture)(GLenum, GLuint);
    void APIENTRY (*glTexParameterf)(GLenum, GLenum, GLfloat);
    void APIENTRY (*glTexParameteri)(GLenum, GLenum, GLint);
    void APIENTRY (*glTexGeni)(GLenum, GLenum, GLint);
    void APIENTRY (*glTexGenfv)(GLenum, GLenum, const GLfloat *);
    void APIENTRY (*glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei,
    		GLint, GLenum, GLenum, const GLvoid *);
    void APIENTRY (*glTexCoord2f)(GLfloat, GLfloat);
    void APIENTRY (*glDeleteTextures)(GLsizei, const GLuint *);
    void APIENTRY (*glNormal3f)(GLfloat, GLfloat, GLfloat);
    void APIENTRY (*glTexCoord3f)(GLfloat, GLfloat, GLfloat);
    void APIENTRY (*gluLookAt)(GLdouble, GLdouble, GLdouble,
    		GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
    void APIENTRY (*gluOrtho2D)(GLdouble, GLdouble, GLdouble, GLdouble);
    void APIENTRY (*gluPerspective)(GLdouble, GLdouble, GLdouble, GLdouble);
    void APIENTRY (*gluBuild2DMipmaps)(
            GLenum, GLint, GLsizei, GLsizei, GLenum, GLenum, const void *);
};

struct ils_sdl {
    void SDLCALL (*SDL_Delay)(unsigned int);
    void SDLCALL (*SDL_FreeSurface)(SDL_Surface *);
    unsigned int SDLCALL (*SDL_GetTicks)(void);
    void * SDLCALL (*SDL_GL_GetProcAddress)(const char*);
    int SDLCALL (*SDL_GL_LoadLibrary)(const char *);
    int SDLCALL (*SDL_GL_SetAttribute)(SDL_GLattr attr, int);
    void SDLCALL (*SDL_GL_SwapBuffers)(void);
    int SDLCALL (*SDL_Init)(unsigned int);
    int SDLCALL (*SDL_PollEvent)(SDL_Event *);
    void SDLCALL (*SDL_Quit)(void);
    SDL_Surface * SDLCALL (*SDL_SetVideoMode)(int, int, int, unsigned int);
    void SDLCALL (*SDL_WM_SetCaption)(const char *, const char *);
    void SDLCALL (*SDL_LockSurface)(SDL_Surface *);
    void SDLCALL (*SDL_UnlockSurface)(SDL_Surface *);
    SDL_Surface * SDLCALL (*SDL_CreateRGBSurface)(Uint32, int, int, int,
            Uint32, Uint32, Uint32, Uint32);
    int SDLCALL (*SDL_BlitSurface)(SDL_Surface *, SDL_Rect *, SDL_Surface *,
            SDL_Rect *);
    int SDLCALL (*TTF_Init)(void);
    TTF_Font * SDLCALL (*TTF_OpenFont)(const char *, int);
    SDL_Surface * SDLCALL (*TTF_RenderText_Blended)(TTF_Font *, const char *,
            SDL_Color);
    void SDLCALL (*TTF_CloseFont)(TTF_Font *);
    SDL_Surface * SDLCALL (*IMG_Load)(const char *);
};

#endif /* RENDER_H_ */
