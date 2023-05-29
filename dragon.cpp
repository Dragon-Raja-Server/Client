#include "stdafx.h"
#include <Stdio.h>
#include <Process.h>
#include <Direct.h>
#include "resource.h"
#include "dragon.h"
#include "math.h"

#include "gameproc.h"
#include "network.h"
#include "map.h"
#include "char.h"
//#include "FPS.h"
#include "object.h"

#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Hong_Sub.h"

#include "Hangul.h"
//#include "Menudef.h"
#include "Menu.h"
#include "MenuStartSet.h"
#include "DirectSound.h"
#include "Effect.h"
#include "LineCommand.h"
#include "Tool.h"
#include "ItemTable.h"
#include "Midi.h"
#include "MouseCursor.h"
#include "SkillTool.h"
#include "Skill.h"
#include "House.h"
#include "CharDataTable.h"
#include "Item.h"
#include "SmallMenu.h"
#include "Counseler.h"
#include "NPC_Pattern.h"
#include "Music.h"
//1206 zhh
#include "CReadArgument.h"
#include "FrameMgr.h"
#include "LottoMgr.h"//soto-030501
//< CSD-CN-031215
#include "SecuritySystem.h"
//> CSD-CN-031215
extern SMENU    SMenu[ MAX_MENU];		// startmenu.cpp
extern NPC_INFO g_infNpc[MAX_CHARACTER_SPRITE_]; // CSD-030419
extern ITEM_INFO g_itemref[MAX_ITEM_MUTANT_KIND]; // CSD-030419
extern int ListenCommand;		// network.cpp
extern void DoQuickmemoryByKeyInput(const int iInputKey,bool bIsDikCheck);

///////////////////////////////////////////////////////////////////////////////
// Global Variables:
HWND				g_hwndMain;
HINSTANCE			g_hInstance;
	
GAMEINFO			g_GameInfo;
	
TCHAR				g_szTitle[ MAX_LOADSTRING ];
TCHAR				g_szWindowClass[ MAX_LOADSTRING ];
TCHAR				g_szCWD[ _MAX_PATH ];
TCHAR				g_szInfoFile[ _MAX_PATH ];
	
BOOL				g_bIsActive = FALSE;
//////////// 0309 lkh Ãß°¡ //////////////
BOOL				g_bCommandMode = CM_COMMAND;
	
DIRECTDRAWINFO		g_DirectDrawInfo = {0,};
CHARACTERLIST		g_CharacterList;

	
POINT				g_pointMouse, g_pointFuzz;
long				g_pointMouseX, g_pointMouseY;
int					g_nLButtonState, g_nRButtonState;
int					g_nLDButtonState, g_nRDButtonState;
int					g_nOldLButtonState, g_nOldRButtonState;
int					g_nSensitivity = 0;		// ¸¶¿ì½º°¨µµ¸¦ SettingÇÑ´Ù. 
int					LButtonDownIng, RButtonDownIng;

DWORD				g_CurrentTime;			// ¼­¹ö¿¡¼­ ¾Ë·ÁÁØ ÇöÀç ½Ã°£.
DWORD				g_ClientTime;			// GameProc()¿¡¼­ °è¼ÓÀûÀ¸·Î Check ÇÑ´Ù. 
DWORD				g_ServerClientTimeGap;

DWORD				g_curr_time;			//  ¼­¹ö¿¡¼­º¸³»ÁÖ´Â ÇöÀçÀÇ ½Ã°£. ÃÊ´ÜÀ§..
DWORD				g_packet_recv_send_checktime;
DWORD				g_Current_Server_Time; // finito 060707 holds the current server time

///////////////// 0311 lkh Ãß°¡ /////////////////
int					g_Operator_Function;	//¿î¿µÀÚ¿ë È¿°ú ±¸ÇöÀ» À§ÇÑ ½ºÀ§Äª º¯¼ö



char IIDD[MAX_PATH]= {0,};
char PPWW[MAX_PATH]= {0,};


int CheckSumError;	
	
// ------------------- khs 0804
char			   *g_DestBackBuf, *g_OldDestBackBuf;
int					Mox, Moy;		// ¸¶¿ì½ºÀÇ MapÀÇ Àý´ëÁÂÇ¥.
int					Mapx, Mapy;		// ÇöÀçº¸¿©Áö´Â ¸ÊÀÇ ½ÃÀÛÀý´ëÁÂÇ¥.
int					MapTx, MapTy;	// ÇöÀçº¸¿©Áö´Â ¸ÊÀÇ ½ÃÀÛÀý´ëÁÂÇ¥.
LPCHARACTER         Hero = NULL;			// ÁÖÀÎ°øÀÇ Point..
//------------------------------
int					QuitFlag = false;

DWORD  g_RecvBytes, g_SendBytes;

bool				MouseCursorOnOff = true;
DWORD				g_Inside;

int					NowLoading;


	
///////////////////////////////////////////////////////////////////////////////
// Function prototypes.
ATOM				MyRegisterClass( HINSTANCE );
BOOL				InitInstance( HINSTANCE, int );
BOOL				InitApplication( HINSTANCE, LPSTR );
void				ExitApplication( void );
void				ParseCommandLine( LPSTR );
	
void				SetCurrentWorkingDirectory( void );
char*				GetCurrentWorkingDirectory( void );
void				SetInfoFile( void );
char*				GetInfoFile( void );
char*				EatFrontWhiteChar( char* );
char*				EatRearWhiteChar( char* );
BOOL				ShowErrorMessage( char* lpszMessage );
	
BOOL				LoadGameInfo( void );
	
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime); // CSD-CN-031215
	
/////////////// 0128 ÀÌ±ÔÈÆ /////////////////	
bool  g_Volume_Off=0;
	
/////////////// 0126 ÀÌ±ÔÈÆ /////////////////	
short int	g_Current_Volume=100;	//ÇöÀçÀÇ »ç¿îµå º¼·ý(0~100»çÀÌ°ª)
short int	g_To_Volume		=100;	//º¯È¯µÇ¾î¾ß ÇÏ´Â (ÃÖÁ¾ÀûÀ¸·Î) ¸ñÇ¥ »ç¿îµå º¼·ý
short int	g_Change_Volume	=0;		//º¼·ý ¼öÄ¡ º¯È¯Ä¡(-+ º¯È­°ª->Fade Out/In)
	
extern void InitMoveVariablesSetting( void );
extern void		ItemRead();	
extern void PutStartLodingImg( );		// 0127 YGI
			
#include "Path.h"
#include "SDL2Render.h"
#include "EnCryptMgr.h"
			
char nOldVal[MAX_PATH];
			
			
int recvAttackResultCount;
FILE *keyplayfp;
FILE *keykeyplayfp;
			
			
bool g_bIsChristmas = false;
			
///////////////////////////////////////////////////////////////////////////////
// Functions.
			
//lsw		
extern char explain_tactics[13][20];
extern char FightTeam[5][13];
extern char NationName[8][20];			// 010227 YGI
// thai2 YGI
extern char	subFarmType[20][25];
extern char	subMineType[20][25];
extern char	subHubType[20][25];

//________________________ // 010904 LTS
extern bool InitNation();
extern void CloseNation();
extern void InitHorseAniTable();		// LTS HORSERIDER
//________________________ // 010904 LTS
			
			
static int FirstCheck = 0;
static char s_szCheckCode[] = "87021595-E214-4061-8E40-98BD9AAED844"; // CSD-CN-031215

HWND g_hMultiDlg = NULL;
//CSecuritySystem g_sysSecurity; // CSD-CN-031215
UINT g_idSafeTimer = 0; // CSD-CN-031215
			
//---------------------------------------------------------------------------------------------
//  °ÔÀÓÀ»	½ÇÇàÇÏ±âÀü È­ÀÏÀÌ¸§ÀÇ º¯µ¿ÀÌ¶óµç°¡ ÀÚµ¿ÆÐÄ¡·Î ¸øÇÏ°í ÇÁ·Î±×·¥ÀûÀ¸·Î
//  Ã³¸®ÇØ¾ß ÇÏ´Â °ÍÀ» ¿©±â¼­ Ã³¸®ÇÑ´Ù. 
//---------------------------------------------------------------------------------------------
void BeforeExe( LPSTR lpCmdLine ) // thai2 YGI
{	//< CSD-030324			

	int i,j;	
	int c = 0;	

	if( !FirstCheck)
	{		
		int ii;
		for(ii=0;ii<13; ii++)
		{
			char *temp = lan->OutputMessage(9,ii+451);
			if( strlen( temp ) >= 20 ) JustMsg( "1 : %d", strlen( temp ) );
			sprintf(explain_tactics[ii],"%s",lan->OutputMessage(9,ii+451));
		}
		
		for(ii=0;ii<5; ii++)
		{
			char *temp = lan->OutputMessage(9,ii+441);
			if( strlen( temp ) >= 13 ) JustMsg( "2: %d", strlen( temp ) );
			
			sprintf(FightTeam[ii],"%s",lan->OutputMessage(9,ii+441));
		}
		
		for(ii=0;ii<8; ii++)
		{
			char *temp = lan->OutputMessage(9,ii+421);
			if( strlen( temp ) >= 20 ) JustMsg( "3: %d", strlen( temp ) );
			sprintf(NationName[ii],"%s",lan->OutputMessage(9,ii+421));
		}
		// 010314 KHS
		for (ii = 0; ii < MAX_CHARACTER_SPRITE_; ++ii)
		{	//< CSD-030419
			char* temp = lan->OutputMessage(4, ii + 201);
			if (strlen(temp) >= 40) 
			{
				JustMsg("4: %d", strlen(temp));
			}

			sprintf(g_infNpc[ii].szName, "%s", lan->OutputMessage(4, ii + 201));
		}	//> CSD-030419
		
		for(ii=0;ii<20; ii++)
		{
			char *temp = lan->OutputMessage(4,ii+401);
			if( strlen( temp ) >= 25 ) JustMsg( "5: %d", strlen( temp ) );
			sprintf(subFarmType[ii],"%s",lan->OutputMessage(4,ii+401));
		}
		
		
		for(ii=0;ii<8; ii++)
		{
			char *temp = lan->OutputMessage(4,ii+421);
			if( strlen( temp ) >= 25 ) JustMsg( "6: %d", strlen( temp ) );
			sprintf(subMineType[ii],"%s",lan->OutputMessage(4,ii+421));
		}
		
		
		for(ii=0;ii<11; ii++)
		{
			char *temp = lan->OutputMessage(4,ii+121);
			if( strlen( temp ) >= 25 ) JustMsg( "7: %d", strlen( temp ) );
			sprintf(subHubType[ii],"%s",lan->OutputMessage(4,ii+121));
		}
		
		for(int j=0;j<MAX_LINE_COMMAND_; j++)//010216 lsw ¶óÀÎ Ä¿¸àµå / µÚ¿¡ ¸í·É¾îµé  501 ¹øºÎÅÍ ½ÃÀÛ
		{
			char *temp = lan->OutputMessage(9,j+501);
			if( strlen( temp ) >= 35 ) JustMsg( "8: %d, %d", strlen( temp ), j );
			sprintf(LC[j].reserved,"%s" ,lan->OutputMessage(9,j+501));
		}
		FirstCheck = 1;
	}

	g_pBill = new CClientBill(lpCmdLine);
	
#ifdef _DEBUG//ÀÌºÎºÐÀº µð¹ö±ë ¿ëÀ¸·Î ¾î¶² ÀÎÀÚ°ªÀÌ ³Ñ¾î ¿Ô´ÂÁö º¸¿©ÁÝ´Ï´Ù. ¹°·Ð ¹èÆ÷ÇÒ¶§´Â »©¾ß ÇÕ´Ï´Ù
	g_pBill->DisplayAllCommand();
#endif

#ifdef _MAPOBJECT_DIRECTORY_CHANGE_

	_mkdir("./object/0" );
	_mkdir("./object/2" );
	_mkdir("./object/3" );
	_mkdir("./object/4" );
	_mkdir("./object/5" );
	_mkdir("./object/6" );
	_mkdir("./object/7" );

	c = ReturnFileNumber("./object/Sung_Tile_00/" );
	c += ReturnFileNumber("./object/K_Sung2/" );
	c += ReturnFileNumber("./object/Ma-In/" );
	c += ReturnFileNumber("./object/Source/" );
	c += ReturnFileNumber("./object/Ice-w01/" );
	c += ReturnFileNumber("./object/Firedun1/" );
	c += ReturnFileNumber("./object/ManDun1/" );

	if( c )
	{
		JustMsg(lan->OutputMessage(6,1)   );//010215 lsw
		JustMsg( lan->OutputMessage(6,2)  );//010215 lsw
	}

	Dir2DirCopy("./object/Sung_Tile_00/",	"./object/2/" );
	Dir2DirCopy("./object/K_Sung2/",		"./object/7/" );
	Dir2DirCopy("./object/Ma-In/",			"./object/0/" );
	Dir2DirCopy("./object/Source/",			"./object/6/" );
	Dir2DirCopy("./object/Ice-w01/",		"./object/3/" );
	Dir2DirCopy("./object/Firedun1/",		"./object/4/" );
	Dir2DirCopy("./object/ManDun1/",		"./object/5/" );

	if( c )
	{
		JustMsg( lan->OutputMessage(6,3) );//010215 lsw
	}
				
	FILE *fp = Fopen( "./object/7/0277.csp", "rb" );
	if( fp )	
	{			
		fclose(fp);			
		JustMsg( lan->OutputMessage(6,4)  );//010215 lsw
		for( i = 277 ; i <= 798 ; i ++)
		{						
			char temp[ FILENAME_MAX];
			char temp1[ FILENAME_MAX];
													
			for( j = 0 ; j < 10 ; j ++)
			{										
				if( j == 0 )
				{								
					sprintf( temp, "./object/7/%04d.csp", i );
					sprintf( temp1, "./object/7/%04d.csp", i+7000 );
				}								
				else								
				{										
					sprintf( temp, "./object/7/%04d%02d.csp", i,j );
					sprintf( temp1, "./object/7/%04d%02d.csp", i+7000,j );
				}									
				rename( temp, temp1 );
			}									
		}											
	}			
#endif
}			

extern int LoadHackingToolName();
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	/*
	::Sleep(1000);//¿ÀÅäÆÐÄ¡ÀÇ ÆÐÄ¡ ¶§¹®¿¡ 1ÃÊ Á¤µµÀÇ µô·¹ÀÌ¸¦ ÁÝ´Ï´Ù.	
#ifdef _DEBUG1
	if (g_sysSecurity.GetDragonHwnd() != NULL)
	{   //< PowerZ-030116
		MessageBox(NULL, "Can Not Run Program", "Run Error", NULL);
        return FALSE;
	}	//> PowerZ-030116
#endif
	*/

	MSG		msg = {0,};
#ifdef _DEBUG1	// 031110 YGI
	EXCEPTION_POINTERS* pException = NULL;//020508 lsw	
	__try	//020514 lsw
	{
#endif

	g_CrashLog.Init(OUTPUT_JUST_FILE,"ClientCrash");//¸ðµå ¼ÂÆÃ ±â·Ï µÉ ÆÄÀÏÀÇ °íÀ¯ ÀÌ¸§ÀÔ´Ï´Ù.
	g_DBGLog.Init(OUTPUT_JUST_FILE,"CustomerService");//¸ðµå ¼ÂÆÃ ±â·Ï µÉ ÆÄÀÏÀÇ °íÀ¯ ÀÌ¸§ÀÔ´Ï´Ù.
	
	CrackMgr.SetInitClock();//030319 lsw
	CrackMgr.InitCrackMgr();
	
	if(!CrackMgr.StartCrackThread())
	{
		MessageBox(NULL,"Can Not Run Program. Call to A/S Center","Init Error",NULL);
		return FALSE;
	}
	
	if(!LoadLanguageText())
	{
		MessageBox(NULL,"Loading Language Pack Error","LanguagePack",NULL);
		return FALSE;
	}

	/*if(!LoadHackingToolName()) 
	{
		g_DBGLog.Log(LOG_LV1,"Can't Load Hacking Tool Check File");
		return false;
	}*/
	//< CSD-CN031215

	/*
	FILE *fpAP = Fopen("DragonRaja.b", "rb" );
	
	if (fpAP != NULL)
	{	
		fclose(fpAP);
		Sleep(300);
		remove("DragonRaja.exe");
		Sleep(300);
		rename("DragonRaja.b", "DragonRaja.exe");
		MessageBox( NULL, lan->OutputMessage(6,6), "DragonRaja", MB_OK );//010215 lsw
		ExitApplication(EA_BEFORE_EXE2);
		return FALSE;
	}
	//> CSD-CN031215
	*/

	if ( !InitApplication( hInstance, lpCmdLine ) )
	{
		g_DBGLog.Log(LOG_LV1,"Application Init Failed");
	 	return	FALSE;
	}
	if ( !InitInstance( hInstance, nCmdShow ) )
	{	
		g_DBGLog.Log(LOG_LV1,"Instance Init Failed");
	 	return	FALSE;
	}
	
	//BeforeExe(lpCmdLine);
#ifndef _SDL2
	if ( !InitDirectDraw( g_hwndMain, &g_DirectDrawInfo ) )
	{	
		g_DBGLog.Log(LOG_LV1,"DDraw Init Failed");
	 	return	FALSE;
	}	
#else
	if (!InitSDL())
	{
		MessageBox(NULL, "SDL", "SDL Failed to Init!", NULL);
		g_DBGLog.Log(LOG_LV1, "InitSDL() Init Failed");
		ExitApplication(EA_NORMAL);	//051013_KCH memory leak Á¦°Å
		return	FALSE;
	}
	SDL_SetWindowTitle(g_pl11l->GetSDLWindow(), "DragonRaja - Rogério");

#endif
	if ( InitDirectInput( g_hwndMain, g_hInstance, g_bIsActive ) != DI_OK )
	{		
		g_DBGLog.Log(LOG_LV1,"DInput Init Failed");
	 	return	FALSE;
	}
	if ( !EWndMgr.InitEditWnd())//021001 lsw
	{
		g_DBGLog.Log(LOG_LV1,"EditWnd Init Failed");
		return	FALSE;
	}

	InitSpriteTransTable( g_DirectDrawInfo.lpDirectDrawSurfacePrimary );

	g_DestBackBuf = GetSurfacePointer( g_DirectDrawInfo.lpDirectDrawSurfaceBack );
	EraseScreen( &g_DirectDrawInfo, (WORD)RGB( 0xff, 0xff, 0xff ) );
	PutStartLodingImg( );
	FlipScreen( &g_DirectDrawInfo );
	
	LoadSoundList();
//	RenameSoundFile(); finito - not used or needed
	
	if( SysInfo.effect )
	if ( !InitSfx( g_hwndMain ) )
	{		
	 	return	FALSE;
	}	

	LoadConditionTable();
	
	if (!LoadEffect())
	{
		MessageBox(NULL,"Loading Effect Error","Effect Data", NULL);
		return FALSE; 
	}

	if (!LoadMagicTable2())
	{
		MessageBox(NULL,"Loading Magic Error","Magic Data", NULL);
		return FALSE; 
	}
	////////////////////// SoundUp LKH Ãß°¡ ////////////////////////
	LoadBGESoundTable();		//¹è°æ È¿°ú ÀÌÆåÆ®À½ Å×ÀÌºí ÀÐ¾î ¿À±â

	LoadSmallMapBack();

	ViewTipsLoad( 0 );
	 	
	HangulOutputArea( 0, SCREEN_WIDTH-1, 0, SCREEN_HEIGHT-1 );
	LoadHangulEnglishFont( "./data/han.fnt", "./data/eng.fnt" );
	MakergbTable();		
	LoadLevelExpTable();
	LoadCursorAni();	
	LoadMouseCursor( "Cursor.Spr" );

	//010928 lsw
	LoadChatImage();

	// finito 12/07/07
	LoadChatColours();

	if(!LoadItemTableData())//021111 lsw
	{
		g_DBGLog.Log(LOG_LV1,"ItemTableData Init Failed");
		return FALSE;
	}

/*	if (checkbeta=='1')
	{	//< CSD-031030
		if (!g_mgrLimit.Load("./data_b5/AbilityLimit.bin"))
		{
			if (!g_mgrLimit.Load("./data/AbilityLimit.bin"))
			{
				MessageBox(NULL,"Loading LimitData Error","Limit Data", NULL);
				return FALSE; 
			}
		}
	}	//> CSD-031030
	else
	{*/
	if (!g_mgrLimit.Load("./data/AbilityLimit.bin"))
	{
		MessageBox(NULL,"Loading LimitData Error","Limit Data", NULL);
		return FALSE; 
	}
	//}
	//> CSD-021015
	if (!InitNation()) 
	{
        MessageBox(NULL,"Loading NationData Error","Nation Data",NULL);
		return FALSE; 
	}
	//________________________________________________________________ // 010904 LTS

#ifdef USE_PROFILER	// 031013 kyo
	g_ProFileMgr.Init();
	g_ProFileMgr.AddCounter("GameProc", "GameProc" );
	g_ProFileMgr.AddCounter("GameProc", "Protocol" );
	g_ProFileMgr.AddCounter("GameProc", "Input" );
#endif 

#ifndef LIGHT_VERSION_
	InitHorseAniTable();		// LTS HORSERIDER
#endif

MAIN_MENU_:

	LoadMenuData( 0 );
	LoadMenuData( 4 );
	StartMenuSetting();      //°ÔÀÓÀ» ½ÃÀÛÇÏ±â Àü¿¡ ¸Þ´º ±¸Á¶Ã¼µéÀ» ÃÊ±âÈ­  GamePorc() Àü¿¡ È£ÃâMenuSetting();      //°ÔÀÓÀ» ½ÃÀÛÇÏ±â Àü¿¡ ¸Þ´º ±¸Á¶Ã¼µéÀ» ÃÊ±âÈ­  GamePorc() Àü¿¡ È£Ãâ
	
//	SetVolumeMusic(::GetPrivateProfileInt("options","music",20,g_szInfoFile)); finito - not used or needed
//	PlayBackMusic( g_hwndMain, 0 ); finito - not used or needed
	
	CursorNo( 1 );

	struct tm *today;
	time_t lTime;
	time( &lTime );
	today = localtime( &lTime );
	//Log( "Packet¹ÞÀ½.txt", "%02d:%02d  %d", today->tm_min, today->tm_sec, addlen );
	if( today->tm_mon == 11 && ( today->tm_mday >= 1 ||  today->tm_mday <= 31  ))  
	{
		g_bIsChristmas = true;
	}

	// 0811 NPC KHS
	LoadNPCAccessTable();
	g_FrameMgr.InitTime();
	while( TRUE )
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0 )){break;}
			
			// soto-030602 ÀÚ½Ä ´ëÈ­»óÀÚ¿¡. TabÅ°¸¦ ¸ÔÀÌÀÚ.
			if(g_hMultiDlg)
			{
				if ( IsDialogMessage(g_hMultiDlg, &msg) == FALSE )
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
#ifdef ALT_TAB_BLOCK
//#ifndef _DEBUG
			if (g_DirectDrawInfo.lpDirectDraw==NULL)		// ¿¢Æ¼ºê´Â µÇ¾úÁö¸¸ ¼­ÆäÀÌ½º°¡ ¾ø´Ù	// LTS 020725
			{
				Sleep(1000); // Finito added Sleep. Stops movement bug and other stuff
				//SetFocus2(HWND_GAME_MAIN);//021001 lsw
				//ShowWindow(g_hwndMain,SW_RESTORE);
				//UpdateWindow(g_hwndMain);
			}
//#endif
#endif
			if( !StartMenuProc( &g_DirectDrawInfo ))
			{
				PostMessage(g_hwndMain, WM_CLOSE, 0, 0);
				goto END_;
			}

			if (QuitFlag == SWD_QUIT || QuitFlag == SWD_LOGON) 
			{
				goto END_;
			}
			if( SMenu[MN_MAININTERFACE].bActive == TRUE ) 
			{
				goto START_GAME_;
			}
		}
	}
	goto END_;		
					
START_GAME_:		
					
	MenuSetting();	
	SendAllReady();	
	// ÆÁº¸±â Àú·¦Àº ±âº»À¸·Î º¸ÀÌ°Ô ÇÑ´Ù.  0818 khs
	// 010205 KHS
	if( SCharacterData.nLevel < 8 )  ViewTipToggle = 1;
					
	double		StartTime, Duration;
	StartTime = timeGetTime( );
	ListenCommand = CMD_NONE;
	while( 1 )			
	{						
		Duration = timeGetTime( ) - StartTime;	// 5ÃÊ¸¦ ±â´Ù¸°´Ù. 
		if ( Duration > 1000 )
		{			
			break;	
		}			
					
		if ( ProtocolProc( &connections ) < 0 )	break;
	}				
					
	InitBytesRoutine();	// 
	YouCanHeroActionProc =0; YouCanViewTipsCheck  = 0;

	CLottoMgr::Create();//soto-030501
	g_FrameMgr.InitTime();


	while( TRUE )
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE ))//PM_NOREMOVE
		{
			if (!GetMessage(&msg, NULL, 0, 0 )){break;}
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
		else
		{

#ifdef ALT_TAB_BLOCK
//#ifndef _DEBUG
			if (g_DirectDrawInfo.lpDirectDraw==NULL)		// ¿¢Æ¼ºê´Â µÇ¾úÁö¸¸ ¼­ÆäÀÌ½º°¡ ¾ø´Ù	// LTS 020725
			{
				 Sleep(1000); // Finito added Sleep. Stops movement bug and other stuff
				//SetFocus(HWND_GAME_MAIN);//021001 lsw
				//ShowWindow(g_hwndMain,SW_RESTORE);
				//UpdateWindow(g_hwndMain);
			}
//#endif
#endif
			if (!GameProc(&g_DirectDrawInfo))
			{
				PostMessage(g_hwndMain, WM_CLOSE, 0, 0);
				goto END_;
			}

			if (QuitFlag == SWD_QUIT || QuitFlag == SWD_LOGON) 
			{
				goto GOTO_MAIN_MENU_;			//0201 YGI
			}
		}
	}		


	goto END_;
			
GOTO_MAIN_MENU_:
	ReStartMenu( );
	pMusic->Stop();
	Release( &connections );				// Release()È£Ãâ..
	FreeTOI();
	DestroyRoofHeader( &g_RoofHeader );			
	DestroyEventList();							
	FreeMapSource();							
	DestroyItemList();
	FreeCharacterSpriteDataAll( 0, MAX_CHARACTER_SPRITE_ );	
	DestroyCharacterListExceptHero();

	CLottoMgr::Destroy();//soto-030501

	goto MAIN_MENU_;

#ifdef _DEBUG1	// 031110 YGI
	}
	__except(pException = GetExceptionInformation())//020508 lsw
	{
		::DumpException( pException, "Exception Raised on WinMain()");		
	}
#endif

END_:
	CloseNation(); // 010904 LTS	//¸ÞÀÎ¸Þ´º¿¡ À§Ä¡ÇÏ°Ô µÇ¸é ClosenationÀÌ °è¼ÓÈ£ÃâµÈ´Ù.. 
									//g_pNationÀÌ °è¼Ó Áö¿öÁø´Ù. 
	{
		char optiondata[15];
		::sprintf(optiondata,"%d",system_info.music);
		::WritePrivateProfileString("options","music",optiondata,g_szInfoFile);
		::sprintf(optiondata,"%d",system_info.sound);
		::WritePrivateProfileString("options","sound",optiondata,g_szInfoFile);
		::sprintf(optiondata,"%d",system_info.gore);
		::WritePrivateProfileString("options","gore",optiondata,g_szInfoFile); // finito 050707 save gore option setting
	}

	ExitApplication(EA_NORMAL);

	return msg.wParam;
}

ATOM
MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX	wcex;

	wcex.cbSize			= sizeof( WNDCLASSEX );
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= ( WNDPROC )WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon( hInstance, ( LPCTSTR )IDI_DRAGON );
	wcex.hCursor		= NULL;// LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground	= ( HBRUSH )( COLOR_WINDOW + 1 );
	
	//acer7
#ifdef _DEBUG // finito 060507
	if( SysInfo.enable_menu ) 
	{
		if( SysInfo.notdead )
		{
			wcex.lpszMenuName	= ( LPCSTR )IDC_DRAGON;
		}
		else
		{
			wcex.lpszMenuName	= ( LPCSTR )IDR_GM;
		}
	}
	else
#endif
	{
		wcex.lpszMenuName	= NULL; 
	}

	wcex.lpszClassName	= g_szWindowClass;
	wcex.hIconSm		= LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_SMALL );

	return	RegisterClassEx( &wcex );
}

BOOL
InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	HWND		hWnd;
	int			cx, cy;
	int         nStyle = 0;

	g_hInstance = hInstance;

    cx = SCREEN_WIDTH + GetSystemMetrics( SM_CXSIZEFRAME ) * 2;
    cy = SCREEN_HEIGHT + GetSystemMetrics( SM_CYSIZEFRAME ) * 2 + GetSystemMetrics( SM_CYMENUSIZE );


	switch( SysInfo.dx )
	{
		case 0 :	nStyle = WS_OVERLAPPEDWINDOW;	break;
		case 1 :	nStyle = WS_POPUP;		break;
	}

//	if( GetSysInfo( SI_GAME_MAKE_MODE ) == 0 )
//		if(FindWindow("DRAGONRAJA_CLASS","DragonRaja Online")){ return FALSE; }
// finito 19/08/07 commented 2 lines above

	hWnd = CreateWindow( g_szWindowClass,
						 g_szTitle,
						 nStyle,
						 0,0,
						 cx,
						 cy,
						 NULL,
						 NULL,
						 hInstance,
						 NULL );
	if ( !hWnd )
	{
		return	FALSE;
	}
	
	g_hwndMain = hWnd;
	
	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	return	TRUE;
}	

BOOL
InitApplication( HINSTANCE hInstance, LPSTR lpCmdLine )
{	
	ModeSetting( g_hwndMain, hInstance);
	if( SysInfo.result == 999 )	return FALSE; 
	
	MakeSizeofSOU2();	
	
	
	LoadString( hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
	LoadString( hInstance, IDC_DRAGONRAJA_CLASS, g_szWindowClass, MAX_LOADSTRING );
	MyRegisterClass( hInstance );
	
//	ParseCommandLine( lpCmdLine );
	srand( ::timeGetTime() );
	
	SetCurrentWorkingDirectory();
	SetInfoFile();
	
	if ( !LoadGameInfo( ) )
	{
		MessageBox(NULL,"LoadGameInfo() failed.","Debug",NULL);
		return	FALSE;
	}

	pMusic->Stop();

	CoInitialize( NULL );
	if ( !::Initialize() )
	{
		return	FALSE;
	}
	g_pointMouseX = SCREEN_WIDTH / 2;
	g_pointMouseY = SCREEN_HEIGHT / 2;
	g_nLButtonState = g_nRButtonState = g_nOldLButtonState = g_nOldRButtonState = STATE_BUTTON_RELEASED;
	g_nSensitivity = 0;
	
	return	TRUE;
}	

void ExitApplication( const eExitAppType eEAType )
{
	CrackMgr.StopCrackThread();

	SendLogOut();
	GameEndFree();

	::Release( &connections );
	
	EndLoadCharSpriteDataThread();
	DestroyCharacterList( &g_CharacterList );
	CleanupDirectDraw( &g_DirectDrawInfo );
	StopBackMusic( g_hwndMain );
	FreeSfx();
	CoUninitialize( );
	FreeAllOfMenu();		// 0927 YGI
	DeleteAllSkillData();
	pMusic->Stop();
	if(EA_NORMAL != eEAType)
	{
		g_DBGLog.Log(LOG_LV1,"ExitApplication Call(%d)",eEAType);
	}

	if( CheckSumError ) 
	{
		JustMsg( "Found corrupted or tampered files while loading. \nExiting program. [%d]", CheckSumError);
	}

	FreeItemTable();

	delete g_pBill;
	delete lan;

	exit(0);
}
	
void ParseCommandLine( LPSTR lpCmdLine )
{	
	while ( lpCmdLine[ 0 ] == '-' || lpCmdLine[ 0 ] == '/' )
	{
		lpCmdLine++;
	
		switch ( *lpCmdLine++ )
		{
		case	'f':
		case	'F':
			g_DirectDrawInfo.bFullscreen = FALSE; // LTS TEST
			break;
		}
	
		lpCmdLine = EatFrontWhiteChar( lpCmdLine );
	}
}
void SetCurrentWorkingDirectory( void )
{	
	char*	ptr;
	
	GetModuleFileName( g_hInstance, g_szCWD, sizeof( g_szCWD ) );
	ptr = strrchr( g_szCWD, '\\' );
	*ptr = 0;
}

char*
GetCurrentWorkingDirectory( void )
{	
	return	g_szCWD;
}	

void SetInfoFile( void )
{	
	wsprintf( g_szInfoFile, "%s/dragon.ini", g_szCWD );
}	
	
char*
GetInfoFile( void )
{	
	return	g_szInfoFile;
}	
	
char*
EatFrontWhiteChar( char* pStr )
{	
	char*	szWhite = " \t\n\r";
	
    if ( pStr )
    {
		while ( *pStr )
		{
			if ( strchr( szWhite, *pStr ) )
			{
				pStr++;
			}
			else
			{
				break;
			}
		}	
    }
        
    return  pStr;    
}	
	
char*
EatRearWhiteChar( char* pStr )
{	
	char*	szWhite = " \t\n\r";
	char*	pRear;
	
	pRear = pStr + strlen( pStr ) - 1;
	if ( pRear )
	{
		while ( pStr <= pRear )
		{
			if ( strchr( szWhite, *pRear ) )
			{
				*pRear-- = 0;
			}
			else
			{
				break;
			}
		}
	}
	
	return	pStr;
}	
	
BOOL
ShowErrorMessage( char* lpszMessage )
{	
	MessageBox( g_hwndMain, lpszMessage, "Dragon Raja Online", MB_OK );
	return	FALSE;
}	
	
	
	
	

														
///////////////////////////////////////////////////////////////////////////////
// window procedure										
																
void MouseProcess( UINT message, WPARAM wParam, LPARAM lParam)
{	
	int x, y;

	switch( message )										
	{														
	case WM_LBUTTONUP     :	g_nLButtonState = STATE_BUTTON_RELEASED; LButtonDownIng= 0;	tool_MyHouseLBU( wParam, lParam ); ReleaseCapture();				break;
	case WM_LBUTTONDOWN   :	YouCanHeroActionProc = 1; YouCanViewTipsCheck  = 1;g_nLButtonState = STATE_BUTTON_PRESSED;  LButtonDownIng= 1;	tool_MyHouseLBD( wParam, lParam ); SetCapture( g_hwndMain);		break;
	case WM_RBUTTONUP	  :	g_nRButtonState = STATE_BUTTON_RELEASED; RButtonDownIng= 0;	ReleaseCapture();				break;
	case WM_RBUTTONDOWN   :	g_nRButtonState = STATE_BUTTON_PRESSED;	 RButtonDownIng= 1;	SetCapture( g_hwndMain);  		break;
	case WM_LBUTTONDBLCLK :	g_nLDButtonState = STATE_BUTTON_DOUBLECLICK;	SetCapture( g_hwndMain);		break;
	case WM_RBUTTONDBLCLK :	g_nRDButtonState = STATE_BUTTON_DOUBLECLICK;	SetCapture( g_hwndMain);		break;
	}												
							
	x = LOWORD( lParam );
	y = HIWORD( lParam );

/*#ifdef _DEBUG
	y += ::GetSystemMetrics(SM_CYMENUSIZE);
#endif*/
	
	if(LButtonDownIng==1 )								
	{															
		g_DragMouse.ex=x+Mapx;	//¸¶¿ì½ºÀÇ È­¸éÁÂÇ¥¿¡ ¸ÊÀÇ Àý´ë ÁÂÇ¥¸¦ ´õÇÔ
		g_DragMouse.ey=y+Mapy;	
	}


	AdjustSkillMouseCursor( &x, &y );
		
	
	// ÇöÀç ³ë°¡´Ù ±â¼úÀ» ÇÏ´Â ÁßÀÌ¸é   ¸¶¿ì½º´Â ±× ¹Ú½º¾È¿¡ ÀÖ¾î¾ß ÇÑ´Ù. 
	g_pointMouseY = g_pointMouse.y = y;
	g_pointMouseX = g_pointMouse.x = x;
	Mox =  Mapx + g_pointMouseX;		Moy =  Mapy + g_pointMouseY;
	
	/*int		SkillNo;
int		SkillStatus;
Spr	   *SkillIcon;
int		SkillItemNo;			// ±â¼ú¿¡ »ç¿ëµÉ  ItemÀÇ ¹øÈ£.
POS		SkillItemPOS;			// ±â¼ú¿¡ »ç¿ëµÉ  ItemÀÇ À§Ä¡°ª.



bool	SkillMouseDontMoveFlag;	//	³ë°¡´Ù¸¦ ÇØ¾ß ÇÏ´Â ±â¼úÀº ÇÑ¹ø ±×°÷À» ¼±ÅÃÇÏ¸é   ¿À¸¥ÂÊ¸¶¿ì½º·Î Ãë¼ÒÇÏµç°¡ NogadaCount°¡ Max°¡ µÉ¶§±îÁö ±â´Ù·Á¾ß ÇÑ´Ù. 
DWORD	SkillRetryTime;
int		SkillDontMoveSx,SkillDontMoveSy,SkillDontMoveEx,SkillDontMoveEy;
bool	YouCanNogadaFlag;
*/

}															

//1206 zhh
//#include "Language.h" //010215 lsw ÁÖ¼®Ã³¸®.


// 10505 KHS  //010605 lms
bool chinese_input;
void CheckChineseInput( HWND hwnd, LPARAM lParam )
{
	if( ( lParam & 0x00ff ) == 0x0004 )
	{	
		HRESULT hResult;

		chinese_input = true;

		if( g_DirectDrawInfo.lpClipper )
		{
			g_DirectDrawInfo.lpClipper->Release();	
			g_DirectDrawInfo.lpClipper = NULL;
		}

		hResult = g_DirectDrawInfo.lpDirectDraw->CreateClipper( 0, &g_DirectDrawInfo.lpClipper, NULL );
		if( hResult == DD_OK ) 
			g_DirectDrawInfo.lpClipper->SetHWnd( 0, g_hwndMain );
	}	
	else
	{	
		chinese_input = false;

		if( g_DirectDrawInfo.lpClipper )
		{
			g_DirectDrawInfo.lpClipper->Release();	
			g_DirectDrawInfo.lpClipper = NULL;
		}
	}	
}
														
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC	hDC;
    MINMAXINFO* pMinMax = NULL;
	
	if (EWndMgr.CheckSysKey( message, wParam ) == 0)
	{	//021001 lsw
		return 0;
	}
	
	switch (message)							
	{
	case WM_INPUTLANGCHANGE:
		{
			CheckChineseInput( hWnd, lParam );
			break;
		}
	case WM_COMMAND:
		{
			SystemMenuProcess ( wParam );	
			break;
		}
	case WM_INITMENU:
		{	//soto 030307
		#ifdef _DEBUG
			MainMenuSelected(wParam,lParam);
		#endif
			break;
		}	//soto End.
	case WM_CREATE:
		{
			SetCursor( NULL );
			SetTimer( hWnd, 13, 1000, NULL);
			break;
		}					
	case WM_TIMER:
		{
			if (wParam == 13)
			{
				g_curr_time++;
				g_packet_recv_send_checktime++;
				//DecMallItemTime(); //removed by rogerio at 2023-05-29 to test others things
			}
			if (wParam == 14)
			{
				g_Current_Server_Time++; // finito 050707
			}

			break;
		}
	case WM_ACTIVATE:		
	case WM_ACTIVATEAPP:			
		{								
			switch (LOWORD(wParam))
			{							
			case WA_CLICKACTIVE:
				{
					g_bIsActive = TRUE;
					g_FrameMgr.InitTime();
					
					if (g_DirectDrawInfo.lpDirectDraw == NULL)
					{
						if (!InitDirectDraw(g_hwndMain, &g_DirectDrawInfo))
						{
							return 0;
						}
						
						EraseScreen(&g_DirectDrawInfo, RGB(0,0,0));
					}
					
					break;
				}
			case WA_INACTIVE:
				{
					g_bIsActive = FALSE;
					g_FrameMgr.InitTime();

					if (g_DirectDrawInfo.lpDirectDraw != NULL)
					{
						if (SysInfo.dx == 0)
						{
							CleanupDirectDraw(&g_DirectDrawInfo);
						}
						else
						{
							g_DirectDrawInfo.lpDirectDrawSurfacePrimary->Release( );
							g_DirectDrawInfo.lpDirectDrawSurfacePrimary = NULL;
							g_DirectDrawInfo.lpDirectDraw = NULL;
						}
					}

					break;
				}
			case WA_ACTIVE:
				{
					g_bIsActive = TRUE;

					if (g_DirectDrawInfo.lpDirectDraw == NULL)
					{
						if (!InitDirectDraw(g_hwndMain, &g_DirectDrawInfo))
						{
							return 0;
						}

						EraseScreen(&g_DirectDrawInfo, RGB(0,0,0));
						SetCursor( NULL );
					}

					break;
				}
			}
			
			SetAcquire(g_bIsActive);
			CheckingAccelator_Sub1();
			break;	
		}								
	case WM_GETMINMAXINFO:
		{								
			pMinMax = (MINMAXINFO*)lParam;
			pMinMax->ptMinTrackSize.x = SCREEN_WIDTH + GetSystemMetrics(SM_CXSIZEFRAME) * 2;
			pMinMax->ptMinTrackSize.y = SCREEN_HEIGHT + GetSystemMetrics(SM_CYSIZEFRAME) * 2 + GetSystemMetrics(SM_CYMENU);
			pMinMax->ptMaxTrackSize.x = pMinMax->ptMinTrackSize.x;
			pMinMax->ptMaxTrackSize.y = pMinMax->ptMinTrackSize.y;
			break;					
		}								
	case WM_MOVE:							
	case WM_SIZE:						
		{						
			if (g_bIsActive)
			{
				if (g_DirectDrawInfo.bFullscreen)
				{						
					SetRect(&g_DirectDrawInfo.rectPrimarySurface, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
				}									
				else									
				{								
					GetClientRect(hWnd, &g_DirectDrawInfo.rectPrimarySurface);
					ClientToScreen(hWnd, (LPPOINT)&g_DirectDrawInfo.rectPrimarySurface);
					ClientToScreen(hWnd, (LPPOINT)&g_DirectDrawInfo.rectPrimarySurface + 1);
				}									
			}

			break;										
		}													
	case WM_PAINT:								
		{	
			if (g_bIsActive)
			{
				hDC = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
			}

			break;
		}
	case WM_KEYDOWN:
		{		
			DoSomeThingWithKey( wParam, lParam );		// tool.cpp
			CheckCharacterFrameOrder( wParam, lParam );	// tool.cpp
			
			switch (wParam)
			{
			case VK_RETURN:		
				{
					if (!IsChatBoxActive())		// 0601 YGI
					{
						SetChatMode(CM_MESSAGE);
					}

					break;
				}
			}

			break;
		}
	case WM_CHAR:
		{
			switch (wParam)
			{
			case VK_RETURN:		
			case VK_TAB:		
				{
					return 0;
				}
			}

			break;
		}
	case WM_KEYUP://021008 lsw
		{
			const int nVirtKey = (int)wParam;	// virtual-key code 
			LPARAM lKeyData = lParam;			// key data
			
			switch (nVirtKey)
			{
			case VK_F1:
			case VK_F2:
			case VK_F3:
			case VK_F4:
			case VK_F5:
			case VK_F6:
			case VK_F7:
			case VK_F8:
				{
					::DoQuickmemoryByKeyInput(nVirtKey,false);
					return 0;
				}
			}

			break;
		}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		{		
			MouseProcess( message, wParam, lParam);
			break;	
		}	
	case WM_MOUSEMOVE:
		{
			if (SysInfo.dx == 0)
			{
				SetCursor( NULL );
			}
			MouseProcess( message, wParam, lParam);
			break;	
		}
	/*case MM_MCINOTIFY:			// MCI ÅëÁö ¸Þ½ÃÁö¸¦ ¹Þ´Â´Ù.
		{
			if (wParam == MCI_NOTIFY_SUCCESSFUL)    // ¿¬ÁÖ°¡ ³¡³µÀ»¶§.
			{
				PlayBackMusic(g_hwndMain, CurMidi);
			}
			else if (CurMidi >= 0)
			{
				PlayBackMusic(g_hwndMain, CurMidi);
			}
			
			break;
		}
	finito not used or needed */
	case WM_ERASEBKGND: // finito stops windows erasing background which caused graphics to flicker white
		break;
	case WM_DESTROY:
		{	
			PostQuitMessage( 0 );	
			break;					
		}								
	}		
	
	return DefWindowProc( hWnd, message, wParam, lParam );
}

/*
void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{	//< CSD-CN-031215
	if (nTimerid == g_idSafeTimer)
	{ 	//< PowerZ-030116
		HWND hSafeWnd = g_sysSecurity.GetSafeWindowHandle();
		
		if (!IsWindow(hSafeWnd))
		{ 
			KillTimer(hWnd, g_idSafeTimer);
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
	}	//> PowerZ-030116
}	//> CSD-CN-031215
*/

BOOL LoadGameInfo( void )
{
	//GetPrivateProfileString( "network", "host1", "", g_GameInfo.proxy_ip[0], sizeof( g_GameInfo.proxy_ip[0] ), g_szInfoFile );
	sprintf( g_GameInfo.proxy_ip[0], "127.0.0.1"); // Add by rogerio
	//sprintf( g_GameInfo.proxy_ip[0], "127.0.0.1"); // Add by rogerio
	if ( g_GameInfo.proxy_ip[0][0]== 0 )
	{
		return	FALSE;
	}
	//GetPrivateProfileString( "network", "host2", "", g_GameInfo.proxy_ip[1], sizeof( g_GameInfo.proxy_ip[1] ), g_szInfoFile );
	sprintf( g_GameInfo.proxy_ip[1], "127.0.0.1"); // Add by rogerio
	//sprintf( g_GameInfo.proxy_ip[1], "127.0.0.1"); // Add by rogerio
	if ( g_GameInfo.proxy_ip[1][0]== 0 )
	{
		return	FALSE;
	}
	g_GameInfo.version = GetPrivateProfileInt( "network", "ver", 0, g_szInfoFile );
	if ( g_GameInfo.version == 0 )
	{
		return	FALSE;
	}
	else
	{
//	char temp[ MAX_PATH];
//	sprintf( temp, "%d", 400 );
//	WritePrivateProfileString( "network", "ver", temp, "dragon.ini" );
	}

	// 010711 YGI
	g_SelectServerIndex = GetPrivateProfileInt( "network", "ServerSet", 0, g_szInfoFile );

	//< kjy-040325 
	// ÀÏ½º ¼±ÅÃ ¹æÁö ÇÃ·¡±×¸¦ dragon.iniÆÄÀÏ·ÎºÎÅÍ ÀÔ·Â¹Þ´Â´Ù. 
	char szKeySelectYilse[50];
	memset( szKeySelectYilse, '\0', 50 );
	sprintf( szKeySelectYilse, "name%d_prevent_select_yilse", g_SelectServerIndex+1 );
	g_GameInfo.preventSelectYilse = GetPrivateProfileInt( "network", szKeySelectYilse, 0, g_szInfoFile );
	//> kjy-040325 


	//< CSD-040127
#ifdef KOREA_LOCALIZING_
	g_GameInfo.proxy_port = 9004;
#else
	g_GameInfo.proxy_port = 9004;
#endif
	g_GameInfo.agent_port = 9001;
	//> CSD-040127
	return	TRUE;
}	
	
int LoadingGameData()
{	// eLoadingGameDataStep ¸¦ »ç¿ë
	static int s_iLoadingPos = LGDS_FIRST;
	
	switch (s_iLoadingPos)
	{
	case LGDS_FIRST:
		{
			LoadAttackRangeTable(0);
			LoadAttackRangeTable(1);
#ifdef _DEBUG // finito 060507
			if (SysInfo.notconectserver)
			{
				s_iLoadingPos = LGDS_STEP1;
				return s_iLoadingPos;
			}
#endif
			DWORD StartTime = ::timeGetTime();
			ListenCommand = CMD_NONE;
			
			while (true)
			{	
				DWORD Duration = ::timeGetTime( ) - StartTime;
				
				if (Duration > WAIT_TIME_RESPONSE_DURATION) // 030930 kyo
				{
					s_iLoadingPos = LGDS_FIRST;
					return -1;								
				}
				
				if (HandleRunning(&connections) <= 0)
				{																	
					Release(&connections);
					s_iLoadingPos = LGDS_FIRST;
					return -2;											
				}																		
				
				if (ListenCommand == CONNECT_REFUSE)
				{
					s_iLoadingPos = LGDS_FIRST;
					return -3;
				}
				
				if (ListenCommand == CMD_CONNECT_INFO)
				{
					s_iLoadingPos = LGDS_STEP1;	
					return s_iLoadingPos;
				}
			}

			break;
		}
	case LGDS_STEP1:
		{
			HandleRunning(&connections);
			g_Volume_Off = 1; // ·ÎµùÀÌ ³¡³¯ ¶§±îÁö ¹è°æÀ½¾Ç ÀÌ¿ÜÀÇ ¼Ò¸® ¾È³ª¿À°Ô
#ifdef _DEBUG // finito 060507		
			if (SysInfo.notconectserver)
			{
				if (!BuildCharacterList(&g_CharacterList))
				{
					JustMsg(lan->OutputMessage(6, 8)); //010215 lsw
				}
			}
#endif
			s_iLoadingPos = LGDS_STEP2;
			break;
		}
	case LGDS_STEP2:
		{
			MapBuild(&g_Map, MapName);	// map
			
			if (HandleRunning(&connections) <= 0)
			{
				Release(&connections);
				return -2;
			}

			s_iLoadingPos = LGDS_STEP3;
			break;
		}
	case LGDS_STEP3 :
		{
			LoadTOI(MapName);
			HandleRunning(&connections );
			BuildRoofHeader(&g_RoofHeader, MapName);					// rof
			HandleRunning(&connections);
			BuildEventList(MapName);									// ent
			HandleRunning(&connections);
			
			ReqTacSkillExp();	// ÀüÃ¼ ÅÃÆ½ ¼öÄ¡ °¡Á®¿À±â
			HandleRunning(&connections);
			AllReqSkillExp();	// ÀüÃ¼ ½ºÅ³ °æÇèÄ¡ °¡Á®¿À±â
			HandleRunning(&connections);
			s_iLoadingPos = LGDS_STEP4;
			break;
		}
	case LGDS_STEP4:		
		{
			LoadMapSource2( MapName );									// sou		
			HandleRunning( &connections);
			s_iLoadingPos = LGDS_STEP5;
			break;
		}
	case LGDS_STEP5: 
		{
			LoadCharSpriteData( "./char/000000.spr",  &CharBuf[0],  &CharBufLength[0], &CharSpr[0], LD_CHAR_SPRITE_ALL_LOAD);
			HandleRunning(&connections);
			s_iLoadingPos= LGDS_STEP6;
			break;
		}
	case LGDS_STEP6: 
		{
			LoadCharSpriteData( "./char/001000.spr",  &CharBuf[1],  &CharBufLength[1], &CharSpr[1], LD_CHAR_SPRITE_ALL_LOAD);
			LoadHeroClothAccessoryData( 104, 107 );
			
			char tempfilename[FILENAME_MAX];
			const int preloadcharsprite[21] = {29,30,37,38,46,72,79,1005,1006,1017,1018,1019,1020,1027,1028,1041,1060,1061,1075,1083,1092 };
						
			for (int i = 0; i < 21; ++i)
			{
				int tt = preloadcharsprite[i];
				sprintf(tempfilename, "./char/%06d.spr", tt);
				LoadCharSpriteData(tempfilename, &CharAccessoryBuf[tt/1000][tt%1000], &CharAccessoryBufLength[ tt/1000][tt%1000], &CharAccessorySpr[tt/1000][tt%1000], LD_CHAR_SPRITE_ALL_LOAD);
				HandleRunning(&connections);
			}
			
			HandleRunning(&connections);
			s_iLoadingPos = LGDS_STEP7;	
			break;
		}
	case LGDS_STEP7:
		{
			LoadClothOrderData();	
			HandleRunning(&connections);
			s_iLoadingPos = LGDS_STEP8;
			break;
		}
	case LGDS_STEP8 : 
		{
			LoadBrightData();			
			HandleRunning(&connections);
			s_iLoadingPos = LGDS_STEP9;
			break;
		}
	case LGDS_STEP9: 	
		{
			StartSetView(Hero);
			HandleRunning(&connections);
			s_iLoadingPos = LGDS_STEP10;
			break;
		}
	case LGDS_STEP10: 
		{
			LoadMenuData(1);
			HandleRunning(&connections);
			s_iLoadingPos = LGDS_STEP11;
			break;
		}
	case LGDS_STEP11: 
		{
			LoadMenuData(2);
			HandleRunning(&connections);
			s_iLoadingPos = LGDS_STEP12;
			break;
		}
	case LGDS_STEP12:
		{
			LoadHongSkillTable();
			HandleRunning(&connections);
			LoadMenuData(5);
			HandleRunning(&connections);

			if (CurOpenHouse >= 0)
			{
				RecvRoofOpen(CurOpenHouse);
			}

			InitHpUpDown();
			HandleRunning(&connections);
			s_iLoadingPos = LGDS_FIRST;
			return LGDS_END; // ÀÌ°Í¸¸ ¿¹¿Ü Ã³¸®
		}
	}
	
	return s_iLoadingPos;
}