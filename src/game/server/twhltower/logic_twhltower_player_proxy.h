//=============================================================================
//
// Purpose: Proxy for TWHL Tower: Source player inputs and outputs
//
//=============================================================================

#ifndef LOGIC_TWHLTOWER_PLAYER_PROXY_H
#define LOGIC_TWHLTOWER_PLAYER_PROXY_H

#include "entityoutput.h"
#include "hl2/hl2_player.h"

class CLogicTwhlTowerPlayerProxy : public CLogicPlayerProxy
{
public:
	DECLARE_CLASS(CLogicTwhlTowerPlayerProxy, CLogicPlayerProxy);

protected:
	COutputEHANDLE	m_OnGotPotentialUseTarget;
	COutputEHANDLE	m_OnLostPotentialUseTarget;
	COutputEHANDLE	m_OnLookAtPotentialUseTarget;
	COutputEHANDLE	m_OnEndLookAtPotentialUseTarget;

private:
	DECLARE_DATADESC();
};

#endif // !LOGIC_TWHLTOWER_PLAYER_PROXY_H
