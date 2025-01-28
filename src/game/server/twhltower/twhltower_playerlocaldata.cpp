//==============================================================================
//
// Purpose: TWHL Tower: Source local player data
//
//==============================================================================

#include "cbase.h"

#include "twhltower_playerlocaldata.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_SIMPLE_DATADESC(CTwhlTowerPlayerLocalData)
	DEFINE_FIELD(m_bIsMousePitchOverriden,		FIELD_BOOLEAN),
	DEFINE_FIELD(m_bIsForwardSpeedOverriden,	FIELD_BOOLEAN),
	DEFINE_FIELD(m_bIsSideSpeedOverriden,		FIELD_BOOLEAN),
	DEFINE_FIELD(m_bIsBackSpeedOverriden,		FIELD_BOOLEAN),
	DEFINE_FIELD(m_flMousePitchOverride,		FIELD_FLOAT),
	DEFINE_FIELD(m_flForwardSpeedOverride,		FIELD_FLOAT),
	DEFINE_FIELD(m_flSideSpeedOverride,			FIELD_FLOAT),
	DEFINE_FIELD(m_flBackSpeedOverride,			FIELD_FLOAT),
END_DATADESC()

BEGIN_SEND_TABLE_NOBASE(CTwhlTowerPlayerLocalData, DT_TwhlLocal)
	SendPropBool(SENDINFO(m_bIsMousePitchOverriden)),
	SendPropBool(SENDINFO(m_bIsForwardSpeedOverriden)),
	SendPropBool(SENDINFO(m_bIsSideSpeedOverriden)),
	SendPropBool(SENDINFO(m_bIsBackSpeedOverriden)),
	SendPropFloat(SENDINFO(m_flMousePitchOverride)),
	SendPropFloat(SENDINFO(m_flForwardSpeedOverride)),
	SendPropFloat(SENDINFO(m_flSideSpeedOverride)),
	SendPropFloat(SENDINFO(m_flBackSpeedOverride)),
END_SEND_TABLE()

//------------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------------
CTwhlTowerPlayerLocalData::CTwhlTowerPlayerLocalData()
{
	m_bIsMousePitchOverriden	= false;
	m_bIsForwardSpeedOverriden	= false;
	m_bIsSideSpeedOverriden		= false;
	m_bIsBackSpeedOverriden		= false;
	m_flMousePitchOverride		= 0.0f;
	m_flForwardSpeedOverride	= 0.0f;
	m_flSideSpeedOverride		= 0.0f;
	m_flBackSpeedOverride		= 0.0f;
}

