#pragma once
#ifndef	__SDL2RENDER_H__
#define	__SDL2RENDER_H__

#include <SDL.h>
#include <SDL_ttf.h>

#ifdef __cplusplus
extern "C"
{
#endif
	extern SDL_Window* m_SDLhwndMain;
	extern SDL_Renderer* m_SDLRenderer;
	extern SDL_Texture* m_SDLTexture;
	extern char* m_SDLPixels;
	//#include <wtypes.h>

	SDL_Window* GetSDLWindow();
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
	extern void SetSDLRenderer(SDL_Renderer* renderer);
	extern void SetSDLWindow(SDL_Window* SDLWindow);
	extern void SetSDLTexture(SDL_Texture* SDLTexture);	

	extern SDL_Renderer* GetSDLRenderer();
	extern SDL_Texture* GetSDLTexture();
	extern char* GetSDLPixels();
	extern void				SetSDLPixels(char* pixels);
	extern SDL_Window* GetSDLWindow();
#ifdef __cplusplus
}
#endif

#endif //__SDL2RENDER_H__

