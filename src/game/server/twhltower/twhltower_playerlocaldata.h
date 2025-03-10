//==============================================================================
//
// Purpose: TWHL Tower: Source local player data
//
//==============================================================================

#ifndef TWHLTOWER_PLAYERLOCALDATA_H
#define TWHLTOWER_PLAYERLOCALDATA_H

#include "networkvar.h"
#include "../shared/twhltower/menu_dvd_counter_state.h"

class CTwhlTowerPlayerLocalData
{
public:
	DECLARE_SIMPLE_DATADESC();
	DECLARE_CLASS_NOBASE(CTwhlTowerPlayerLocalData);
	DECLARE_EMBEDDED_NETWORKVAR();

	CTwhlTowerPlayerLocalData();

	// DVD counter
	CNetworkVar(int, m_iCollectedDiscs);
	CNetworkVar(int, m_iTotalDiscs);
	CNetworkVar(bool, m_bFoundCurrentFloorDisc);
	CNetworkVar(MenuDvdCounterState_e, m_iMenuDvdCounterState);

	// Input overrides
	CNetworkVar(bool,	m_bIsMousePitchOverriden);
	CNetworkVar(bool,	m_bIsForwardSpeedOverriden);
	CNetworkVar(bool,	m_bIsSideSpeedOverriden);
	CNetworkVar(bool,	m_bIsBackSpeedOverriden);
	CNetworkVar(float,	m_flMousePitchOverride);
	CNetworkVar(float,	m_flForwardSpeedOverride);
	CNetworkVar(float,	m_flSideSpeedOverride);
	CNetworkVar(float,	m_flBackSpeedOverride);
};

EXTERN_SEND_TABLE(DT_TwhlLocal);

#endif // !TWHLTOWER_PLAYERLOCALDATA_H
