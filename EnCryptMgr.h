#if !defined(AFX_CEnCryptMgr_H__F753DF0D_1705_42E8_AAB7_69852C663F96__INCLUDED_)
#define AFX_CEnCryptMgr_H__F753DF0D_1705_42E8_AAB7_69852C663F96__INCLUDED_

#pragma once
#include "directdraw.h" //DIRECTDRAWINFO;
#include "dragon.h"		// GAMEINFO;
#include "mytypes.h"	//CHARACTER;

#include "EnCryptClass.h" // g_pPack
#include "FrameMgr.h"	  // g_frameMgr

class CEnCryptMgr: public Singleton<CEnCryptMgr>
{
public:
	explicit CEnCryptMgr(void);
	virtual ~CEnCryptMgr(void);

private:

	//time 
	DWORD				m_dwCurrentTime;
	DWORD				m_dwClientTime;	// GameProc()에서 계속적으로 Check 한다. 
	DWORD				m_dwServerClientTimeGap;

	DWORD				m_dwcurr_time;  //  서버에서보내주는 현재의 시간. 초단위..
	DWORD				m_dwpacket_recv_send_checktime;

#define _MAX_ID_LENGTH 40	//060617_kch china의 tom weblogin작업시 pw가 암호화 되어서 오는데, 40byte 정도.
#define _1_SEC_from_ms 1000;
	//user id
	char m_IIDD[MAX_PATH];				//user id
	char m_PPWW[MAX_PATH];
	char m_sId[_MAX_ID_LENGTH];			//채팅창에서, 입력된 글자를 조합시 이용.
	char m_sPassword[_MAX_ID_LENGTH];

	//window active flag
	BOOL				m_bIsActive;

	//window handle
	HWND				m_hwndMain;
	DIRECTDRAWINFO		m_DxDraw;

#ifdef _SDL2
#include "SDL2Render.h"
	SDL_Window* m_SDLhwndMain;
	SDL_Renderer* m_SDLRenderer;
	SDL_Texture* m_SDLTexture;
	char* m_SDLPixels;
#endif

	//game char info
	GAMEINFO			m_GameInfo;
	LPCHARACTER			m_pHero;

	DWORD				m_dwAttackLimitTime;

	int					m_ct;

	DWORD				m_dwCurTime_1Sec;
	WORD				m_wTimeSpan;
	DWORD				m_dwRecv_ExpectTime;
public:	

#ifdef _SDL2
	SDL_Window* GetSDLWindow() { 
		return m_SDLhwndMain; 
	}
	
	void SetSDLWindow(SDL_Window* SDLWindow) { 
		m_SDLhwndMain = SDLWindow; 
	}

	SDL_Renderer* GetSDLRenderer() { 
		return m_SDLRenderer; 
	}

	void SetSDLRenderer(SDL_Renderer* SDLRenderer) { 
		m_SDLRenderer = SDLRenderer; 
	}

	SDL_Texture* GetSDLTexture() { 
		return m_SDLTexture; 
	}
	
	void SetSDLTexture(SDL_Texture* SDLTexture) { 
		m_SDLTexture = SDLTexture; 
	}

	char* GetSDLPixels() { 
		return m_SDLPixels; 
	}
	void SetSDLPixels(char* pixels) { 
		m_SDLPixels = pixels; 
	}
#endif

	DIRECTDRAWINFO& GetDxDrawRef() { return m_DxDraw; }
	GAMEINFO& GetGameInfoRef() { return m_GameInfo; }

	LPCHARACTER		GetHeroPtr() { return m_pHero; }
	void			SetHeroPtr(const LPCHARACTER lpHero) { m_pHero = lpHero; }
};

#define g_pl11l	CEnCryptMgr::Pointer()

#endif
