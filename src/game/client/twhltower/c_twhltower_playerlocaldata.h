//==============================================================================
//
// Purpose: TWHL Tower: Source local player data
//
//==============================================================================

#ifndef TWHLTOWER_PLAYERLOCALDATA_H
#define TWHLTOWER_PLAYERLOCALDATA_H

#include "dt_recv.h"
#include "../shared/twhltower/menu_dvd_counter_state.h"

class C_TwhlTowerPlayerLocalData
{
public:
	DECLARE_CLASS_NOBASE(C_TwhlTowerPlayerLocalData);
	DECLARE_EMBEDDED_NETWORKVAR();

	C_TwhlTowerPlayerLocalData();

	// DVD counter
	int m_iCollectedDiscs;
	int m_iTotalDiscs;
	bool m_bFoundCurrentFloorDisc;
	MenuDvdCounterState_e m_iMenuDvdCounterState;

	// Input overrides
	bool	m_bIsMousePitchOverriden;
	bool	m_bIsForwardSpeedOverriden;
	bool	m_bIsSideSpeedOverriden;
	bool	m_bIsBackSpeedOverriden;
	float	m_flMousePitchOverride;
	float	m_flForwardSpeedOverride;
	float	m_flSideSpeedOverride;
	float	m_flBackSpeedOverride;
};

EXTERN_RECV_TABLE(DT_TwhlLocal);

#endif // !TWHLTOWER_PLAYERLOCALDATA_H
