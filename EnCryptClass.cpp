// EnCryptClass.cpp: implementation of the CEnCryptClass class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "EnCryptClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEnCryptClass::CEnCryptClass(void) :m_nMAX_Cnt_Mark(4)
{
	//make encrypt key
	m_nIdx_PM = 0;		//PackMemory char size
	m_pPMTarget = NULL; m_pMark = NULL;


	m_pMark = new char[m_nMAX_Cnt_Mark + 1];	
	ZeroMemory(m_pMark, m_nMAX_Cnt_Mark + 1);

	for (m_nIdx_PM = 0; m_nIdx_PM < m_nMAX_Cnt_Mark; ++m_nIdx_PM)
	{
		m_pMark[m_nIdx_PM] = (char)((rand() % 63) + 1);
	}
}

CEnCryptClass::~CEnCryptClass(void)
{
	SAFE_DELETE_ARRAY(m_pMark);
}