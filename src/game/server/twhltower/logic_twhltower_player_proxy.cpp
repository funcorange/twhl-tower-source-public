//=============================================================================
//
// Purpose: Proxy for TWHL Tower: Source player inputs and outputs
//
//=============================================================================

#include "cbase.h"
#include "logic_twhltower_player_proxy.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS(logic_playerproxy, CLogicTwhlTowerPlayerProxy);

BEGIN_DATADESC(CLogicTwhlTowerPlayerProxy)
	DEFINE_OUTPUT(m_OnGotPotentialUseTarget, "OnGotPotentialUseTarget"),
	DEFINE_OUTPUT(m_OnLostPotentialUseTarget, "OnLostPotentialUseTarget"),
	DEFINE_OUTPUT(m_OnLookAtPotentialUseTarget, "OnLookAtPotentialUseTarget"),
	DEFINE_OUTPUT(
		m_OnEndLookAtPotentialUseTarget,
		"OnEndLookAtPotentialUseTarget"
	),
END_DATADESC()
