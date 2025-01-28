//==============================================================================
//
// Purpose: TWHL Tower: Source local player data
//
//==============================================================================

#include "cbase.h"

#include "c_twhltower_playerlocaldata.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_RECV_TABLE_NOBASE(C_TwhlTowerPlayerLocalData, DT_TwhlLocal)
	RecvPropInt(RECVINFO(m_iCollectedDiscs)),
	RecvPropInt(RECVINFO(m_iTotalDiscs)),
	RecvPropBool(RECVINFO(m_bFoundCurrentFloorDisc)),
	RecvPropInt(RECVINFO(m_iMenuDvdCounterState)),
	RecvPropBool(RECVINFO(m_bIsMousePitchOverriden)),
	RecvPropBool(RECVINFO(m_bIsForwardSpeedOverriden)),
	RecvPropBool(RECVINFO(m_bIsSideSpeedOverriden)),
	RecvPropBool(RECVINFO(m_bIsBackSpeedOverriden)),
	RecvPropFloat(RECVINFO(m_flMousePitchOverride)),
	RecvPropFloat(RECVINFO(m_flForwardSpeedOverride)),
	RecvPropFloat(RECVINFO(m_flSideSpeedOverride)),
	RecvPropFloat(RECVINFO(m_flBackSpeedOverride)),
END_RECV_TABLE()

//------------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------------
C_TwhlTowerPlayerLocalData::C_TwhlTowerPlayerLocalData() :
	m_iCollectedDiscs(0),
	m_iTotalDiscs(0),
	m_bFoundCurrentFloorDisc(false),
	m_bIsMousePitchOverriden(false),
	m_bIsForwardSpeedOverriden(false),
	m_bIsSideSpeedOverriden(false),
	m_bIsBackSpeedOverriden(false),
	m_flMousePitchOverride(0.0f),
	m_flForwardSpeedOverride(0.0f),
	m_flSideSpeedOverride(0.0f),
	m_flBackSpeedOverride(0.0f)
{
}

