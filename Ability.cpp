// Ability.cpp: implementation of the CAbility class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ability.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CAbility::CAbility()
{
  m_wStr = 0;
  m_wDex = 0;
  m_wCon = 0;
  m_wWis = 0;
  m_wInt = 0;
  m_wCha = 0;
  m_wEndu = 0;
  m_wMovp = 0;
  m_wMor = 0;
  m_wLuck = 0;
  m_wWs = 0;
  m_wPs = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CAbilityDivide::CAbilityDivide()
{
  m_wBefore = 0;
  m_wAfter = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CAbilityLimit::CAbilityLimit()
{
  m_wClass = 0;
  m_wStep = 0;
  m_wMaxTactic = 0;
}

ABILITYLIMIT  g_mgrLimit;
ABILITYDIVIDE g_mgrDivide;
