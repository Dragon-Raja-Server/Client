#include "stdafx.h"
#include <Stdio.h>
#include "dragon.h"
#include "SDL2Render.h"

#include "MouseCursor.h"
BOOL InitSDL();
void SDL_EraseScreen();
void SDL_EndFont();
char* SDL_GetSurfacePointer();
void SDL_FlipScreen();
void SDL_Hprint(const int x, const int y, char* destbuf, const char* szStr, SDL_Color textColor, int HangulAlign);
void SDL_HprintBold(const int x, const int y, char* destbuf, const char* szStr, SDL_Color textColor, int HangulAlign);
void SDL_Draw();
void SDLKill();

TTF_Font* gFont;
SDL_Texture* BlankTexture;

struct CACHEDFONT {
	int id;
	SDL_Texture* texture;
}pCACHEDFONT;

const int MAXFONTS = 512;

CACHEDFONT CachedFonts[MAXFONTS];
int CachedFontsCount;

BOOL InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return false;
	}

	if (TTF_Init() < 0)
	{
		return false;
	}

	return true;
}