#include "stdafx.h"
#include <ddraw.h>
#include <Io.h>
#include "Hangul.h"
#include "dragon.h"
#include "SmallMenuSet.h"
#include "Menu.h"
#include "SmallMenu.h"
#include "Hong_Sub.h"
#include "Hong_Sprite.h"
#include "stdio.h"
#include "directsound.h"
#include "directinput.h"
#include "Char.h"
#include "Effect.h"
#include "map.h"
#include "tool.h"
#include "MenuSet.h"
#include "convert565to555.h"
#include "Math.h"
#include "MenuNetwork.h"
#include "GameProc.h"
#include "CharDataTable.h"
#include "object.h"
#include "Item.h"
#include <direct.h>
#include "linecommand.h"
#include "word.h"
#include "kh_cpp.h"



//奄沙竺舛 
//適掘什 
#define LOW			200
#define C_GENERAL	300
#define HIGH		400
#define SUPERIOR	500
#define C_GREAT		600
#define CRUSADER	700

//送穣 
#define J_GENERAL	200
#define PRECISE		300
#define GOOD		400
#define TRUST		500
#define HONORABLE	600
#define J_GREAT		700

//敗呪 
void	Func_Division(void);	//葵雌渋拭魚虞 Division 痕呪税 葵聖 痕井馬食 旋遂徹 是廃... 
//痕呪 
int		Class_Division;				//適掘什 誤失帖 葵雌渋拭魚献 姥歳聖是廃 痕呪 	
int		Job_Division;				//送穣 誤失帖 葵雌渋拭魚献 姥歳聖是廃 痕呪 	
int		QUEST_GIVE;
int		QUEST_COUNT;

int		quest_num[QUEST_MAX] ; 
//SCharacterData.nCharacterData[SPELL]
void IncreaseFamous( int class_plus, int job_plus )
{
	SCharacterData.class_famous += class_plus;
	//
	// 適傾什 誤失戚 雌渋梅聖 凶 嬢恐 坦軒亜 赤陥檎...
	//

	SCharacterData.job_famous += job_plus;
	Func_Division();
	//
	// 送穣 誤失戚 雌渋梅聖凶...
	//
}

void DecreaseFamous( int class_plus, int job_plus )
{
	SCharacterData.class_famous -= class_plus;
	if( SCharacterData.class_famous < 0 ) SCharacterData.class_famous = 0;
	//dik馬喰梅聖 凶, 坦軒背醤拝 採歳戚 赤陥檎 食奄拭...
	//
	
	SCharacterData.job_famous -= job_plus;
	if( SCharacterData.job_famous < 0 ) SCharacterData.job_famous = 0;
	Func_Division();
}

void Func_Division(){
	//適掘什 誤失帖 
	if(SCharacterData.class_famous > 0 && SCharacterData.class_famous < LOW)					Class_Division = 1;
	else if(SCharacterData.class_famous > LOW && SCharacterData.class_famous < C_GENERAL)			Class_Division = 2;
	else if(SCharacterData.class_famous > C_GENERAL && SCharacterData.class_famous < HIGH)		Class_Division = 3;
	else if(SCharacterData.class_famous > HIGH && SCharacterData.class_famous < SUPERIOR)		Class_Division = 4;
	else if(SCharacterData.class_famous > SUPERIOR && SCharacterData.class_famous < C_GREAT)	Class_Division = 5;
	else if(SCharacterData.class_famous > C_GREAT && SCharacterData.class_famous < CRUSADER)	Class_Division = 6;
	//送穣 誤失帖 
	if(SCharacterData.job_famous > 0 && SCharacterData.job_famous < J_GENERAL)					Job_Division = 1;
	else if(SCharacterData.job_famous > J_GENERAL && SCharacterData.job_famous < PRECISE)			Job_Division = 2;
	else if(SCharacterData.job_famous > PRECISE && SCharacterData.job_famous < GOOD)			Job_Division = 3;
	else if(SCharacterData.job_famous > GOOD && SCharacterData.job_famous < TRUST)				Job_Division = 4;
	else if(SCharacterData.job_famous > TRUST && SCharacterData.job_famous < HONORABLE)			Job_Division = 5;
	else if(SCharacterData.job_famous > HONORABLE && SCharacterData.job_famous < J_GREAT)		Job_Division = 6;
	
}


QuestTable  quest_text[QUEST_MAX];
// 源益企稽 締什闘 砺戚鷺聖 災君 紳陥. 
void LoadQuestTable( char *filename,char* filename2, char *filename_bin)
{
	
	char text[1024];
	char text1[1024];
	FILE *fp;
	
	if(fp = Fopen( filename, "rt" ))
	{
		int count = 0;
		while( fgets( text, 1024, fp) != NULL )
		{
			if( text[0] == '#' || text[0] == ';' ) continue;
			quest_text[count].LoadTable( text );
			count++;
		}
		fclose( fp );
		count = 0;
		fp = Fopen( filename2, "rt" );	
		while( fgets( text1, 1024, fp) != NULL )
		{
			if( text1[0] == '#' || text1[0] == ';' ) continue;
			quest_text[count].LoadPicture( text1 );
			count++;
		}
		fclose( fp );
				
		
		if( Fopen ( filename_bin, "wb" ))
		{
			char* tt = (char*)quest_text;
			char crc = 0;
			while(fgets( text, 1024, fp) != NULL){
				crc += *tt;
				tt++;
			}
			fwrite( &crc, 1, 1, fp);
			fwrite( quest_text, sizeof(QuestTable), QUEST_MAX, fp);
			fwrite( &crc, 1, 1, fp);
			fclose(fp);
		}
	}
	else
	{
		
		if( fp = Fopen( filename_bin, "rb" ))
		{
			char* tt = (char*) quest_text;
			char crc = 0,crc1,crc2;
			fread( &crc1, 1, 1, fp);
			fread( quest_text, sizeof(QuestTable), QUEST_MAX, fp );
			fread( &crc2, 1, 1, fp);
			fclose(fp );

			for( int i=0; i<QUEST_MAX; i++ )
			{
				quest_text[i].SaveTable();
			}
		}
		else return;
	}
	return ;
	
}




// 締什闘 砺戚鷺戚櫛 隔神亀 鞠澗汽 砺戚鷺戚 舛軒亜 鞠嬢赤走 省焼辞 
// 魚稽 幻級嬢兜精 督析聖 災君紳陥. 
void QuestTable::LoadPicture( char *text )	{
	
	char *token = strtok(text,",\t\n");
	if( !token ) return;
	picnum = atoi(token);
		
	token= strtok(NULL, ",\t\n") ;
	if( !token ) return;
	strcpy( picture, token );
}



void QuestTable::SaveTable()
{
	return;
	FILE *fp;
	fp = fopen( "quest.txt", "at+" );
	if( !fp ) return;

	fprintf( fp, "%d	%d	%s	%d	%s	", num, code, name, place, ch );
	int i;
	for( i=0; i<QUEST_IF_COUNT; i++ )
	{
		fprintf(fp, "%d	", quest_if[i] );
	}
	for( i=0; i<QUEST_TAKE_COUNT; i++ )
	{
		fprintf(fp, "%d	", quest_take[i] );		
	}
	fprintf( fp, "\n" );
	fclose( fp );

	fp = fopen( "questpic.txt", "at+" );
	if( !fp ) return;

	fprintf( fp, "%d	%s\n", picnum, picture );
	fclose( fp );
	return;

}

void QuestTable::LoadTable( char *text )	// 庚切伸拭辞 汽戚展研 歳軒
{
	char *token = strtok(text,",\t\n");
	if( !token ) return;
	num = atoi( token );

	token= strtok(NULL, ",\t\n") ;
	if( !token ) return;
	code = atoi(token);
	
	token = strtok(NULL,",\t\n");
	if( !token ) return;
	strcpy( name, token );
	
	token= strtok(NULL, ",\t\n") ;
	if( !token ) return;
	place = atoi(token);
	
	token = strtok(NULL,",\t\n");
	if( !token ) return;
	strcpy( ch, token );
	
	int i;
	for(i = 0 ; i < QUEST_IF_COUNT; i++){
		token= strtok(NULL, ",\t\n") ;
		if( !token ) return;
		quest_if[i] = atoi(token);
	}

	for(i = 0 ; i < QUEST_TAKE_COUNT; i++){
		token= strtok(NULL, ",\t\n") ;
		if( !token ) return;
		quest_take[i] = atoi(token);
	}
	
	///////////////////////////
	// 牌鯉 蓄亜
}

//////////////////////////////////////////
// 坦製 獣拙 採牽奄
void kh_InitMenu()
{
	//LoadQuestTable("./quest/quest.txt");
}

int end_quest[200];

void SetEndQuest(int num )
{
	//for( int i=0; i<3; i++ )
		end_quest[num] = 1;
		return ;
}

void SetStartQuest(int num){
	QUEST_GIVE = rand()%4;
	quest_num[QUEST_COUNT++] = QUEST_GIVE;
	
	if(QUEST_COUNT > 200 ) QUEST_COUNT = 200;
	return;
}
//if( end_quest[quest_num] ) --> 失因廃 締什闘..

extern ItemAttr itemMallAttr[MAX_BUY_ITEM*3*5];
extern int itemMallMax;

void kh_KeyProc()
{
	if(g_aCurrentKeys[DIK_R] & 0x80)
	{
		itemMallMax = 0;
		for (int i = 0; i < 30; ++i)
		{
			itemMallAttr[i].item_no = 10275 + i;
			++itemMallMax;
		}
		CallSmallMenu(MN_ITEM_MALL);		// 締什闘 五敢
	}
	if(g_aCurrentKeys[DIK_M] & 0x80)
	{
		if(!SMenu[MN_CHARACTER_COLOR_CHANGE].key){
			SMenu[MN_CHARACTER_COLOR_CHANGE].key = 1;		// 締什闘 五敢
		}
		else {
			SMenu[MN_CHARACTER_COLOR_CHANGE].key = 0;
		}
	}// けしさずけい 
	//FAME
	
	if(g_aCurrentKeys[DIK_B] & 0x80)
	{
		
		//SetStartQuest(1);
		//CallSmallMenu(MN_FAME);		// 締什闘 五敢
		SendReqQuestIndex( 1 );
	}

	if(g_aCurrentKeys[DIK_X] & 0x80 )
	{	
		kein_kh_key_on = true;
	}
}


void FreeQuest()
{
	for( int i=0; i<QUEST_MAX;i++ )
		if( quest_text[i].sq.img ) FreeSpriteOnlyOne( &quest_text[i].sq );
}

void QestDataFree( )
{
	memset( quest_num, 0, sizeof( int ) *QUEST_MAX );
	memset( end_quest, 0, sizeof( int ) *QUEST_MAX );
	QUEST_COUNT = 0;
}








//////////////////////////////////////1010 kkh 蓄亜 //////////////////////////////////////
void CharacterWorkingWater( LPCHARACTER ch )
{
	//1010 kkh 蓄亜 
	static bool FIRSTONWATER = TRUE; // 1010 kkh 弘 廃腰幻 域紗 啄澗 益顕 是背 
	if( TileMap[ch->x / TILE_SIZE][ch->y / TILE_SIZE].attr_river == 1) {// 幻鉦拭 辞赤澗 員戚 弘紗失昔 員戚虞檎.. 
		
		static unsigned int tempCHtileX = 0,tempCHtileY = 0;
		if( tempCHtileX  > ch->x / TILE_SIZE || tempCHtileX < ch->x / TILE_SIZE \
			|| tempCHtileY > ch->y / TILE_SIZE || tempCHtileY < ch->y / TILE_SIZE){
			{ //
				if(abs((long)tempCHtileX - (ch->x / TILE_SIZE))+abs((long)tempCHtileY - (ch->y / TILE_SIZE)) >= 1){
					switch(ch->nCurrentAction){
					case ACTION_BASIC_WALK			:{
							InsertMagic(ch, ch, 490, 0, 0,0,0,0);
							InsertMagic(ch, ch, 491, 0, 0,0,0,0);
							PlayListAutoSounds( rand()%2+1148, 0, 0,0 );
							break;
													 }
					case ACTION_RUN					:{
							InsertMagic(ch, ch, 492, 0, 0,0,0,0);
							InsertMagic(ch, ch, 493, 0, 0,0,0,0);
							PlayListAutoSounds( rand()%2+1145, 0, 0,0 );
							break;
													 }
	 				
					}
					tempCHtileX = ch->x / TILE_SIZE;  // 薄仙 疎妊研 奄常 獣佃 兜澗陥.
					tempCHtileY = ch->y / TILE_SIZE;  // 陥製拭 級嬢 尽聖凶 搾嘘 馬奄 是背
				}
				
			}	
				//FIRSTONWATER = TRUE;
		}
	}
	
	//////////////////////////////////////1010 kkh 蓄亜 魁 //////////////////////////////////////
}
						   



