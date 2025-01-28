//==============================================================================
//
// Purpose: TWHL Tower: Source local player data
//
//==============================================================================

#ifndef TWHLTOWER_PLAYERLOCALDATA_H
#define TWHLTOWER_PLAYERLOCALDATA_H

#include "dt_recv.h"

class C_TwhlTowerPlayerLocalData
{
public:
	DECLARE_CLASS_NOBASE(C_TwhlTowerPlayerLocalData);
	DECLARE_EMBEDDED_NETWORKVAR();

	C_TwhlTowerPlayerLocalData();

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
