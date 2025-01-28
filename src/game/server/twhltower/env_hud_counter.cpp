//=============================================================================
//
// Purpose: HUD counter entity
//
//=============================================================================

#include "cbase.h"
#include "env_hud_counter.h"

LINK_ENTITY_TO_CLASS(env_hud_counter, CEnvHudCounter);

BEGIN_DATADESC(CEnvHudCounter)

	DEFINE_KEYFIELD(m_iszTitle, FIELD_STRING, "title"),
	DEFINE_KEYFIELD(m_nMaxCount, FIELD_INTEGER, "maxcount"),
	DEFINE_KEYFIELD(m_nValue, FIELD_INTEGER, "value"),

	DEFINE_FIELD(m_bIsShown, FIELD_BOOLEAN),

	DEFINE_INPUTFUNC(FIELD_VOID, "ShowCounter", InputShowCounter),
	DEFINE_INPUTFUNC(FIELD_VOID, "HideCounter", InputHideCounter),
	DEFINE_INPUTFUNC(FIELD_INTEGER, "SetValue", InputSetValue),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CEnvHudCounter::CEnvHudCounter() :
	m_iszTitle(),
	m_nMaxCount(0),
	m_nValue(0),
	m_bIsShown(false),
	BaseClass()
{
}


//-----------------------------------------------------------------------------
// Purpose: Called on entity spawn
//-----------------------------------------------------------------------------
void CEnvHudCounter::Spawn()
{
	BaseClass::Spawn();
}


//-----------------------------------------------------------------------------
// Purpose: Show counter on HUD
//-----------------------------------------------------------------------------
void CEnvHudCounter::ShowCounter()
{
	m_bIsShown = true;

	CBroadcastRecipientFilter filter;
	filter.MakeReliable();

	UserMessageBegin(filter, "HudCounter");
		WRITE_BYTE(0);
		WRITE_LONG(m_nMaxCount),
		WRITE_LONG(m_nValue),
		WRITE_STRING(STRING(m_iszTitle)),
	MessageEnd();
}


//-----------------------------------------------------------------------------
// Purpose: Hide counter from HUD
//-----------------------------------------------------------------------------
void CEnvHudCounter::HideCounter()
{
	m_bIsShown = false;

	CBroadcastRecipientFilter filter;
	filter.MakeReliable();

	UserMessageBegin(filter, "HudCounter");
		WRITE_BYTE(1);
	MessageEnd();
}


//-----------------------------------------------------------------------------
// Purpose: Get whether or not this counter is currently shown
// Output:  True if counter is shown, false if it is hidden
//-----------------------------------------------------------------------------
bool CEnvHudCounter::GetShowState()
{
	return m_bIsShown;
}


//-----------------------------------------------------------------------------
// Purpose: Set HUD counter value
//-----------------------------------------------------------------------------
void CEnvHudCounter::SetValue(int iValue)
{
	m_nValue = iValue;

	CBroadcastRecipientFilter filter;
	filter.MakeReliable();

	UserMessageBegin(filter, "HudCounter");
		WRITE_BYTE(2);
		WRITE_LONG(m_nValue);
	MessageEnd();
}


//-----------------------------------------------------------------------------
// Purpose: Show counter on HUD
//-----------------------------------------------------------------------------
void CEnvHudCounter::InputShowCounter(inputdata_t& inputdata)
{
	ShowCounter();
}


//-----------------------------------------------------------------------------
// Purpose: Hide counter from HUD
//-----------------------------------------------------------------------------
void CEnvHudCounter::InputHideCounter(inputdata_t& inputdata)
{
	HideCounter();
}


//-----------------------------------------------------------------------------
// Purpose: Set HUD counter value
//-----------------------------------------------------------------------------
void CEnvHudCounter::InputSetValue(inputdata_t& inputdata)
{
	SetValue(inputdata.value.Int());
}

