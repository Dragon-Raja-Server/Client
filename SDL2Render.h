#pragma once
#ifndef	__SDL2RENDER_H__
#define	__SDL2RENDER_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <minwindef.h>

#ifdef __cplusplus
extern "C"
{
#endif


	extern BOOL	InitSDL();
	extern void SDL_EraseScreen();
	extern char* SDL_GetSurfacePointer();
	extern void SDL_FlipScreen();
	extern void SDL_Hprint(const int x, const int y, char* destbuf, const char* szStr, SDL_Color textColor, int HangulAlign);
	extern void SDL_HprintBold(const int x, const int y, char* destbuf, const char* szStr, SDL_Color textColor, int HangulAlign);
	extern void SDL_Draw();
	extern void SDLKill();
	extern void SDL_Lock();
	extern void SDL_UnLock();

#ifdef __cplusplus
}
#endif

#endif //__SDL2RENDER_H__