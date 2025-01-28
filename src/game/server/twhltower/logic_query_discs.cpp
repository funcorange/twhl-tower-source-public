//=============================================================================
//
// Purpose: Entity for querying player disc count
//
//=============================================================================

#include "cbase.h"
#include "logic_query_discs.h"

#include "ai_utils.h"
#include "baseentity.h"
#include "entityoutput.h"
#include "twhltower_player.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//=============================================================================
// 
// CLogicQueryDiscs
// 
//=============================================================================
LINK_ENTITY_TO_CLASS(logic_query_discs, CLogicQueryDiscs);

BEGIN_DATADESC(CLogicQueryDiscs)
	DEFINE_OUTPUT(m_OnQueryCollectedDiscs, "OnQueryCollectedDiscs"),
	DEFINE_OUTPUT(m_OnQueryTotalDiscs, "OnQueryTotalDiscs"),

	DEFINE_INPUTFUNC(
		FIELD_VOID,
		"QueryCollectedDiscs",
		InputQueryCollectedDiscs
	),
	DEFINE_INPUTFUNC(
		FIELD_VOID,
		"QueryTotalDiscs",
		InputQueryTotalDiscs
	),
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CLogicQueryDiscs::CLogicQueryDiscs() :
	m_OnQueryCollectedDiscs(),
	m_OnQueryTotalDiscs(),
	BaseClass()
{
}


//-----------------------------------------------------------------------------
// Purpose: Query player collected disc count
// Input  : inputdata - Input data
//-----------------------------------------------------------------------------
void CLogicQueryDiscs::InputQueryCollectedDiscs(inputdata_t& inputdata)
{
	CBasePlayer* pPlayer = AI_GetSinglePlayer();
	if (pPlayer == NULL)
		return;

	CTwhlTower_Player* pTwhlTowerPlayer =
		dynamic_cast<CTwhlTower_Player*>(pPlayer);
	if (pTwhlTowerPlayer == NULL)
		return;

	m_OnQueryCollectedDiscs.Set(
		pTwhlTowerPlayer->GetCollectedDiscCount(),
		inputdata.pActivator,
		inputdata.pCaller
	);
}


//-----------------------------------------------------------------------------
// Purpose: Query player total disc count
// Input  : inputdata - Input data
//-----------------------------------------------------------------------------
void CLogicQueryDiscs::InputQueryTotalDiscs(inputdata_t& inputdata)
{
	CBasePlayer* pPlayer = AI_GetSinglePlayer();
	if (pPlayer == NULL)
		return;

	CTwhlTower_Player* pTwhlTowerPlayer =
		dynamic_cast<CTwhlTower_Player*>(pPlayer);
	if (pTwhlTowerPlayer == NULL)
		return;

	m_OnQueryCollectedDiscs.Set(
		pTwhlTowerPlayer->GetTotalDiscCount(),
		inputdata.pActivator,
		inputdata.pCaller
	);
}

