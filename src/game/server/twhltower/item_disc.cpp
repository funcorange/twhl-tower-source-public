//==============================================================================
//
// Purpose: TWHL Tower: Source Half-Life 2 DVD collectible
//
//==============================================================================

#include "cbase.h"
#include "item_disc.h"

#include <igameevents.h>

#include "twhltower_player.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define DVD_MODEL_NAME "models/props_tower/dvd.mdl"
#define DVD_PICKUP_SOUND "Player.PickupWeapon"

LINK_ENTITY_TO_CLASS(item_disc, CItemDisc);

BEGIN_DATADESC(CItemDisc)
	DEFINE_KEYFIELD(m_iszMessage, FIELD_STRING, "message"),
END_DATADESC()

//------------------------------------------------------------------------------
// Purpose: Called on entity spawn
//------------------------------------------------------------------------------
void CItemDisc::Spawn()
{
	Precache();

	SetModel(DVD_MODEL_NAME);

	BaseClass::Spawn();
}


//------------------------------------------------------------------------------
// Purpose: Precache entity resources
//------------------------------------------------------------------------------
void CItemDisc::Precache()
{
	BaseClass::Precache();

	PrecacheModel(DVD_MODEL_NAME);
	PrecacheScriptSound(DVD_PICKUP_SOUND);
}


//------------------------------------------------------------------------------
// Purpose: Called when player touches this item
//------------------------------------------------------------------------------
bool CItemDisc::MyTouch(CBasePlayer* pPlayer)
{
	CPASAttenuationFilter filter(pPlayer, DVD_PICKUP_SOUND);
	EmitSound(filter, pPlayer->entindex(), DVD_PICKUP_SOUND);

	CTwhlTower_Player* pTwhlTowerPlayer = dynamic_cast<CTwhlTower_Player*>(pPlayer);
	if (!pTwhlTowerPlayer)
		return true;

	pTwhlTowerPlayer->IncrementCollectedDiscsCount(STRING(m_iszMessage));
	pTwhlTowerPlayer->SetCurrentFloorDiscFound(true);

	IGameEvent* pEvent = gameeventmanager->CreateEvent("disc_pickup");
	if (pEvent)
	{
		pEvent->SetInt("userid", pPlayer->GetUserID());
		gameeventmanager->FireEvent(pEvent);
	}

	return true;
}
