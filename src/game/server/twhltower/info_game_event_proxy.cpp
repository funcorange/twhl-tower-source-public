//============================================================================
//
// Purpose: Entity for firing game instructor events
//
//============================================================================

#include "cbase.h"
#include "info_game_event_proxy.h"

#include "igameevents.h"
#include "saverestore_utlvector.h"

LINK_ENTITY_TO_CLASS(info_game_event_proxy, CInfoGameEventProxy);

BEGIN_DATADESC(CInfoGameEventProxy)
	
	DEFINE_KEYFIELD(m_iszEventName, FIELD_STRING, "event_name"),
	DEFINE_KEYFIELD(m_flRadius, FIELD_FLOAT, "range"),
	DEFINE_KEYFIELD(m_iszSubjectName, FIELD_STRING, "subject"),
	DEFINE_KEYFIELD(m_bDisabled, FIELD_BOOLEAN, "startdisabled"),

	DEFINE_UTLVECTOR(m_VisibleEntities, FIELD_EHANDLE),

	DEFINE_INPUTFUNC(FIELD_VOID, "GenerateGameEvent", InputGenerateEvent),
	DEFINE_INPUTFUNC(FIELD_VOID, "Enable", InputEnable),
	DEFINE_INPUTFUNC(FIELD_VOID, "Disable", InputDisable),

	DEFINE_THINKFUNC(AutoVisibilityThink),

END_DATADESC()

//----------------------------------------------------------------------------
// Purpose: Constructor
//----------------------------------------------------------------------------
CInfoGameEventProxy::CInfoGameEventProxy() :
	m_iszEventName(),
	m_flRadius(0.0F),
	m_iszSubjectName(),
	m_bDisabled(false),
	m_VisibleEntities(),
	BaseClass()
{
}


//----------------------------------------------------------------------------
// Purpose: Called on entity spawn
//----------------------------------------------------------------------------
void CInfoGameEventProxy::Spawn()
{
	Precache();
	BaseClass::Spawn();

	if (HasSpawnFlags(SF_GAMEEVENTPROXY_AUTOVISIBILITY))
	{
		SetThink(&CInfoGameEventProxy::AutoVisibilityThink);
		SetNextThink(gpGlobals->curtime);
	}
	else
	{
		SetThink(NULL);
	}
}


//----------------------------------------------------------------------------
// Purpose: Detect if a player can see this entity and fire event
//----------------------------------------------------------------------------
void CInfoGameEventProxy::AutoVisibilityThink()
{
	SetNextThink(gpGlobals->curtime + 0.1F);

	if (!IsEnabled())
		return;

	CBaseEntity* pEntity = NULL;
	while ((pEntity = gEntList.FindEntityByClassname(pEntity, "player")) != NULL)
	{
		if (!pEntity->IsPlayer())
			continue;

		if (!IsEntityVisible(pEntity))
		{
			if (IsEntityInVisibleSet(pEntity))
			{
				RemoveEntityFromVisibleSet(pEntity);
			}

			continue;
		}

		if (!IsEntityInVisibleSet(pEntity))
		{
			AddEntityToVisibleSet(pEntity);
			GenerateEvent(pEntity);
		}
	}
}


//----------------------------------------------------------------------------
// Purpose: Generate this entity's event
//----------------------------------------------------------------------------
void CInfoGameEventProxy::GenerateEvent(CBaseEntity* pActivator)
{
	if (m_iszEventName == NULL_STRING)
		return;

	IGameEvent* pEvent = gameeventmanager->CreateEvent(STRING(m_iszEventName));
	if (!pEvent)
		return;

	pEvent->SetInt("userid", (pActivator && pActivator->IsPlayer()) ? ToBasePlayer(pActivator)->GetUserID() : UTIL_GetLocalPlayer()->GetUserID());
	
	CBaseEntity* pSubjectEntity = gEntList.FindEntityByName(NULL, m_iszSubjectName);
	pEvent->SetInt("subject", pSubjectEntity ? pSubjectEntity->entindex() : entindex());

	DevMsg("CInfoGameEventProxy firing event %s\n", STRING(m_iszEventName));
	gameeventmanager->FireEvent(pEvent);
}


//----------------------------------------------------------------------------
// Purpose: Check if the given entity is within this entity's radius
//----------------------------------------------------------------------------
bool CInfoGameEventProxy::IsEntityInRadius(CBaseEntity* pEntity) const
{
	if (m_flRadius < 0.0F)
		return true;

	if (!pEntity)
		return false;

	Vector vecStart = pEntity->EyePosition();
	Vector vecEnd = GetAbsOrigin();

	if ((vecEnd - vecStart).Length() > m_flRadius)
		return false;

	return true;
}


//----------------------------------------------------------------------------
// Purpose: GenerateEvent input
//----------------------------------------------------------------------------
void CInfoGameEventProxy::InputGenerateEvent(inputdata_t& inputdata)
{
	if (!IsEnabled())
		return;

	CBaseEntity* pActivator = (inputdata.pActivator ? inputdata.pActivator : UTIL_GetLocalPlayer());

	if (IsEntityInRadius(pActivator))
		GenerateEvent(pActivator);
}


//----------------------------------------------------------------------------
// Purpose: Determine whether the specified entity can be seen by this entity
//----------------------------------------------------------------------------
bool CInfoGameEventProxy::IsEntityVisible(CBaseEntity* pEntity)
{
	if (!IsEntityInRadius(pEntity))
		return false;

	Vector vecStart = pEntity->EyePosition();
	Vector vecEnd = GetAbsOrigin();

	trace_t tr;
	CTraceFilterSimple filter(pEntity, COLLISION_GROUP_NONE);
	UTIL_TraceLine(vecStart, vecEnd, MASK_VISIBLE, &filter, &tr);

	if (tr.fraction < 1.0F)
		return false;

	return true;
}


//----------------------------------------------------------------------------
// Purpose: See if the specified entity is already visible by this entity
//----------------------------------------------------------------------------
bool CInfoGameEventProxy::IsEntityInVisibleSet(CBaseEntity* pEntity)
{
	for (CBaseEntity* pCurrent : m_VisibleEntities)
	{
		if (pCurrent == pEntity)
			return true;
	}

	return false;
}


//----------------------------------------------------------------------------
// Purpose: Remove the specified entity from the set of visible entities
//----------------------------------------------------------------------------
void CInfoGameEventProxy::RemoveEntityFromVisibleSet(CBaseEntity* pEntity)
{
	auto end = m_VisibleEntities.end();
	int n = 0;
	for (auto i = m_VisibleEntities.begin(); i != end; ++i)
	{
		CBaseEntity* pCurrent = *i;

		if (pCurrent == pEntity)
		{
			m_VisibleEntities.Remove(n);
			return;
		}

		++n;
	}
}


//----------------------------------------------------------------------------
// Purpose: Add the specified entity to the set of visible entities
//----------------------------------------------------------------------------
void CInfoGameEventProxy::AddEntityToVisibleSet(CBaseEntity* pEntity)
{
	m_VisibleEntities.AddToTail(pEntity);
}

