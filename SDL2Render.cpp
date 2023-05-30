#pragma once

#include "stdafx.h"
#include <Stdio.h>
#include "SDL2Render.h"
#include "MouseCursor.h"
#include "global.h"


HWND g_hwndMain;
HINSTANCE g_hInstance;

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

SDL_Window* m_SDLhwndMain;
SDL_Renderer* m_SDLRenderer;
SDL_Texture* m_SDLTexture;
char* m_SDLPixels;

//Functions
BOOL InitSDL()
{
#ifdef _SDL2
	//Init Blank Texture
	BlankTexture = NULL;

	//Setup Font Cache
	CachedFontsCount = 0;

	for (int i = 0; i < 128; i++)
	{
		CachedFonts[i].id = 0;
		CachedFonts[i].texture = NULL;
	}


	SDL_Window* SDLHWnd = GetSDLWindow();
	if (SDLHWnd != NULL) return TRUE;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return FALSE;
	}
	else
	{
		SDLHWnd = SDL_CreateWindowFrom(g_hwndMain);
		if (SDLHWnd == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return FALSE;
		}
	}

	SDL_ShowCursor(SDL_FALSE);

	SDL_Surface* screenSurface = SDL_GetWindowSurface(SDLHWnd);

	SDL_SetColorKey(screenSurface, SDL_TRUE, SDL_MapRGB(screenSurface->format, 0xFF, 0, 0xFF));

	SDL_Renderer* renderer = SDL_CreateRenderer(SDLHWnd, -1, SDL_RENDERER_ACCELERATED & SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);

	SDL_Rect dest;
	dest.h = SCREEN_HEIGHT;
	dest.w = SCREEN_WIDTH;
	dest.x = 0;
	dest.y = 0;

	SDL_RenderDrawRect(renderer, &dest);

	//SDL_SetWindowMinimumSize(SDLHWnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	//SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

	SDL_Texture* screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	//SDL_PIXELFORMAT_RGB565

	SetSDLRenderer(renderer);
	SetSDLWindow(SDLHWnd);
	SetSDLTexture(screen_texture);

	TTF_Init();
	gFont = TTF_OpenFont("./micross.ttf", 12);

#endif
	return TRUE;
}

void SDL_EraseScreen()
{
#ifdef _SDL2
	SDL_Renderer* screen_renderer = GetSDLRenderer();
	if (BlankTexture == NULL)
	{
		SDL_Surface* screenSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x0, 0x0, 0x0));
		BlankTexture = SDL_CreateTextureFromSurface(screen_renderer, screenSurface);
		SDL_FreeSurface(screenSurface);
	}
	SDL_RenderClear(screen_renderer);
	SDL_RenderCopy(screen_renderer, BlankTexture, NULL, NULL);
	SDL_RenderPresent(screen_renderer);
#endif
}

#ifdef  _SDL2
SDL_Window* GetSDLWindow() { return m_SDLhwndMain; }
void SetSDLWindow(SDL_Window* SDLWindow) { m_SDLhwndMain = SDLWindow; }
	
SDL_Renderer* GetSDLRenderer() { return m_SDLRenderer; }
void SetSDLRenderer(SDL_Renderer* SDLRenderer) { m_SDLRenderer = SDLRenderer; }
	
SDL_Texture* GetSDLTexture() { return m_SDLTexture; }
void SetSDLTexture(SDL_Texture* SDLTexture) { m_SDLTexture = SDLTexture; }
	
char* GetSDLPixels();
void SetSDLPixels(char* pixels);
	
char* GetSDLPixels() { return m_SDLPixels; }
void SetSDLPixels(char* pixels) { m_SDLPixels = pixels; }
#endif //  _SDL2


char* SDL_GetSurfacePointer()
{
#ifdef _SDL2
	if (GetSDLPixels() != NULL)
	{
		return GetSDLPixels();
	}
	else
	{
		char* pixels = (char*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 2);

		memset(pixels, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 2);

		SetSDLPixels(pixels);

		return (char*)pixels;
	}
#endif
	return 0;
}

void SDL_Lock()
{
	//SDL_Texture * screen_texture = g_pl11l->GetSDLTexture();
	//void *pixels;
	//int pitch;
	//SDL_LockTexture(screen_texture,NULL,&pixels,&pitch);
	//g_DestBackBuf = (char *)pixels;
}

void SDL_UnLock()
{
	//SDL_Texture * screen_texture = g_pl11l->GetSDLTexture();
	//SDL_UnlockTexture(screen_texture);
	//g_DestBackBuf = g_pl11l->GetSDLPixels();
}

void SDL_Draw()
{
	SDL_Renderer* screen_renderer = GetSDLRenderer();
	SDL_Texture* screen_texture = GetSDLTexture();

	SDL_RenderCopy(screen_renderer, BlankTexture, NULL, NULL);

	char* pixels = GetSDLPixels();
	SDL_UpdateTexture(screen_texture, NULL, pixels, SCREEN_WIDTH * 2);
	SDL_RenderCopy(screen_renderer, screen_texture, NULL, NULL);
}

void SDL_FlipScreen()
{
#ifdef _SDL2
	SDL_Event e;
	while (SDL_PollEvent(&e)) {

	}
	SDL_Renderer* screen_renderer = GetSDLRenderer();
	//SDL_Texture * screen_texture = g_pl11l->GetSDLTexture();

	char fps[50];

	//sprintf(fps, "FPS:[%d]", (g_FrameMgr.DoFrameDelay (fps) / 2));

	SDL_Color c = { 0, 0, 0 };
	SDL_HprintBold(99, 1, NULL, fps, c, TA_LEFT);
	SDL_HprintBold(101, 1, NULL, fps, c, TA_LEFT);
	SDL_HprintBold(100, 0, NULL, fps, c, TA_LEFT);
	SDL_HprintBold(100, 2, NULL, fps, c, TA_LEFT);

	SDL_Color c2 = { 0x0, 0xFF, 0x0 };
	SDL_HprintBold(100, 1, NULL, fps, c2, TA_LEFT);

	SDL_RenderPresent(screen_renderer);
#endif
}


int hashSDL2(char* szStr) {
	int h = 0;
	for (int i = 0; i < strlen(szStr); i++)
	{
		h = 31 * h + szStr[i];
	}
	return h;
}

int getFontFromCache(char* szStr, bool bold, SDL_Color textColor)
{
	int hashed = 0;
	if (bold)
	{
		hashed = hashSDL2(szStr) + 0xB01D + ((int)(textColor.r + textColor.b + textColor.g));
	}
	else
	{
		hashed = hashSDL2(szStr);
	}

	for (int i = 0; i < MAXFONTS; i++)
	{
		if (CachedFonts[i].id == hashed)
		{
			return i;
		}
	}
	return -1;
}

SDL_Texture* putFontIntoCache(char* szStr, bool bold, SDL_Color textColor)
{
	int hashed = hashSDL2(szStr) + 0xB01D + ((int)(textColor.r + textColor.b + textColor.g));

	if (bold)
	{
		TTF_SetFontStyle(gFont, TTF_STYLE_NORMAL); //TTF_STYLE_BOLD);
	}
	else {
		TTF_SetFontStyle(gFont, TTF_STYLE_NORMAL);
	}

	CachedFontsCount = ((CachedFontsCount + 1) % MAXFONTS);
	//if (CachedFontsCount >= MAXFONTS) {
	//	CachedFontsCount = 0;
	//}

	if (CachedFonts[CachedFontsCount].texture != NULL)
	{
		SDL_DestroyTexture(CachedFonts[CachedFontsCount].texture);
	}

	SDL_Renderer* screen_renderer = GetSDLRenderer();
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, szStr, textColor);
	SDL_Texture* _texture = SDL_CreateTextureFromSurface(screen_renderer, textSurface);

	CachedFonts[CachedFontsCount].id = hashed;
	CachedFonts[CachedFontsCount].texture = _texture;

	SDL_FreeSurface(textSurface);

	return CachedFonts[CachedFontsCount].texture;
}

SDL_Rect AlignText(int HangulAlign, int x, int y, int texW, int texH)
{
	//int w,h;
	//TTF_SizeText(gFont, szStr, &w, &h)
	SDL_Rect dstrect = { x, y, texW, texH };
	switch (HangulAlign)
	{
	case(TA_RIGHT):
	{
		dstrect.x = dstrect.x - texW;
		break;
	}
	case(TA_CENTER):
	{
		dstrect.x = dstrect.x - (texW / 2);
		break;
	}
	case(TA_LEFT):
	default:
	{
		//Do nothing.
	}
	}
	return dstrect;
}

void SDL_Hprint(const int x, const int y, char* destbuf, const char* szStr, SDL_Color textColor, int HangulAlign)
{
	int FFC = getFontFromCache((char*)szStr, 0, textColor);
	SDL_Texture* texture;
	if (FFC > -1)
	{
		texture = CachedFonts[FFC].texture;
	}
	else
	{
		texture = putFontIntoCache((char*)szStr, 0, textColor);
	}
	SDL_Renderer* screen_renderer = GetSDLRenderer();
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = AlignText(HangulAlign, x, y, texW, texH); //{ x, y, texW, texH };
	SDL_RenderCopy(screen_renderer, texture, NULL, &dstrect);
}

void SDL_HprintBold(const int x, const int y, char* destbuf, const char* szStr, SDL_Color textColor, int HangulAlign)
{
	int FFC = getFontFromCache((char*)szStr, 1, textColor);

	SDL_Texture* texture;
	if (FFC > -1)
	{
		texture = CachedFonts[FFC].texture;
	}
	else
	{
		texture = putFontIntoCache((char*)szStr, 1, textColor);
	}
	SDL_Renderer* screen_renderer = GetSDLRenderer();
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { x, y, texW, texH };
	SDL_RenderCopy(screen_renderer, texture, NULL, &dstrect);
}


void SDLKill()
{
	SDL_DestroyTexture(GetSDLTexture());

	TTF_CloseFont(gFont);

	SDL_DestroyRenderer(GetSDLRenderer());
	SDL_DestroyWindow(GetSDLWindow());

	TTF_Quit();
	SDL_Quit();
}

