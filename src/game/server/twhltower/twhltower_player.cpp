//==============================================================================
//
// Purpose: Player for TWHL Tower: Source
//
//==============================================================================

#include "cbase.h"
#include "twhltower_player.h"

#include "ammodef.h"
#include "igameevents.h"
#include "item_ammo.h"
#include "items.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define ITEM_GLOW_MIN_DOT 0.9659258

ConVar g_sv_total_disc_count("sv_total_disc_count", "18", FCVAR_CHEAT);
ConVar g_sv_item_glow_distance_max("sv_item_glow_distance_max", "1024", FCVAR_CHEAT);
ConVar g_sv_item_glow_distance_near("sv_item_glow_distance_near", "512", FCVAR_CHEAT);

//==============================================================================
//
// CTwhlTower_Player
//
//==============================================================================
#ifdef TWHLTOWER
LINK_ENTITY_TO_CLASS(player, CTwhlTower_Player);
#endif // TWHLTOWER

BEGIN_DATADESC(CTwhlTower_Player)
	DEFINE_INPUTFUNC(FIELD_FLOAT,	"SetMousePitchOverride",		InputSetMousePitchOverride),
	DEFINE_INPUTFUNC(FIELD_FLOAT,	"ClearMousePitchOverride",		InputClearMousePitchOverride),
	DEFINE_INPUTFUNC(FIELD_FLOAT,	"SetSideSpeedOverride",			InputSetSideSpeedOverride),
	DEFINE_INPUTFUNC(FIELD_FLOAT,	"ClearSideSpeedOverride",		InputClearSideSpeedOverride),
	DEFINE_INPUTFUNC(FIELD_FLOAT,	"SetBackSpeedOverride",			InputSetBackSpeedOverride),
	DEFINE_INPUTFUNC(FIELD_FLOAT,	"ClearBackSpeedOverride",		InputClearBackSpeedOverride),
	DEFINE_INPUTFUNC(FIELD_FLOAT,	"SetForwardSpeedOverride",		InputSetForwardSpeedOverride),
	DEFINE_INPUTFUNC(FIELD_FLOAT,	"ClearForwardSpeedOverride",	InputClearForwardSpeedOverride),
	DEFINE_INPUTFUNC(FIELD_FLOAT,	"SetExplosiveDamageScale",		InputSetExplosiveDamageScale),

	DEFINE_FIELD(m_iCollectedDiscs, FIELD_INTEGER),
	DEFINE_FIELD(m_iTotalDiscs, FIELD_INTEGER),
	DEFINE_FIELD(m_bIsNearGlowTarget, FIELD_BOOLEAN),
	DEFINE_FIELD(m_hGlowTarget, FIELD_EHANDLE),
	DEFINE_FIELD(m_flExplosiveDamageScale, FIELD_FLOAT),
	DEFINE_EMBEDDED(m_TwhlLocal),
	DEFINE_FIELD(m_hGlowProxy, FIELD_EHANDLE),
	DEFINE_FIELD(m_GlowColor, FIELD_COLOR32),
	DEFINE_FIELD(m_hPotentialUseEntity, FIELD_EHANDLE),
	DEFINE_FIELD(m_hLookAtEntity, FIELD_EHANDLE),
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST(CTwhlTower_Player, DT_TwhlPlayer)
	SendPropDataTable(SENDINFO_DT(m_TwhlLocal), &REFERENCE_SEND_TABLE(DT_TwhlLocal), SendProxy_SendLocalDataTable),
	SendPropEHandle(SENDINFO(m_hGlowProxy)),
	SendPropInt(SENDINFO(m_GlowColor), 32, SPROP_UNSIGNED, SendProxy_Color32ToInt),
	SendPropEHandle(SENDINFO(m_hPotentialUseEntity)),
END_SEND_TABLE()

//------------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------------
CTwhlTower_Player::CTwhlTower_Player() :
	m_iCollectedDiscs(0),
	m_iTotalDiscs(0),
	m_bIsNearGlowTarget(false),
	m_hGlowTarget(),
	m_flExplosiveDamageScale(),
	m_TwhlLocal(),
	m_hGlowProxy(),
	m_GlowColor(),
	m_hPotentialUseEntity(),
	BaseClass()
{
}


//------------------------------------------------------------------------------
// Purpose: Called on entity spawn
//------------------------------------------------------------------------------
void CTwhlTower_Player::Spawn()
{
	SetTotalDiscCount(g_sv_total_disc_count.GetInt());

	m_flExplosiveDamageScale = 1.0F;
	m_GlowColor = {255, 208, 64, 255};

	BaseClass::Spawn();
}


//------------------------------------------------------------------------------
// Purpose: Called every usercmd by the player PostThink
//------------------------------------------------------------------------------
void CTwhlTower_Player::PostThink()
{
	BaseClass::PostThink();

	UpdateItemGlow();

	CBaseEntity* pUseEntity = FindUseEntity();

	if (pUseEntity != m_hPotentialUseEntity)
	{
		if (m_hPotentialUseEntity != NULL)
		{
			variant_t variant = variant_t();
			variant.SetEntity(pUseEntity);
			FirePlayerProxyOutput(
				"OnLostPotentialUseTarget",
				variant,
				m_hPotentialUseEntity,
				this
			);
		}

		if (pUseEntity != NULL)
		{
			variant_t variant = variant_t();
			variant.SetEntity(pUseEntity);
			FirePlayerProxyOutput(
				"OnGotPotentialUseTarget",
				variant,
				pUseEntity,
				this
			);
		}
	}

	trace_t tr;
	Vector vecForward;
	EyeVectors(&vecForward);
	UTIL_TraceLine(
		EyePosition(),
		EyePosition() + vecForward * 1024.0F,
		MASK_SOLID | CONTENTS_DEBRIS | CONTENTS_PLAYERCLIP | MASK_NPCSOLID,
		this,
		COLLISION_GROUP_NONE,
		&tr
	);
	CBaseEntity* const pLookEntity = tr.m_pEnt;

	bool const bHasLookEntity = pLookEntity != NULL;
	bool const bNewLookEntity = pLookEntity != m_hLookAtEntity;
	bool const bNewUseEntity = pUseEntity != m_hPotentialUseEntity;
	bool const bLookEntityIsUseEntity = pLookEntity == pUseEntity;
	bool const bOldLookEntityIsUseEntity
		= m_hLookAtEntity == m_hPotentialUseEntity;
	if (bHasLookEntity
		&& bLookEntityIsUseEntity
		&& (bNewLookEntity || bNewUseEntity)
	)
	{
		variant_t variant = variant_t();
		variant.SetEntity(pLookEntity);
		FirePlayerProxyOutput(
			"OnLookAtPotentialUseTarget",
			variant,
			pLookEntity,
			this
		);
	}
	else if (bOldLookEntityIsUseEntity
		&& (!bHasLookEntity || !bLookEntityIsUseEntity)
	)
	{
		variant_t variant = variant_t();
		variant.SetEntity(m_hLookAtEntity);
		FirePlayerProxyOutput(
			"OnEndLookAtPotentialUseTarget",
			variant,
			m_hLookAtEntity,
			this
		);
	}

	m_hPotentialUseEntity = pUseEntity;
	m_hLookAtEntity = pLookEntity;
}


//------------------------------------------------------------------------------
// Purpose: Process damage taken
//------------------------------------------------------------------------------
int CTwhlTower_Player::OnTakeDamage(const CTakeDamageInfo& inputInfo)
{
	CTakeDamageInfo info(inputInfo);

	// Scale explosive damage
	if (info.GetDamageType() & DMG_BLAST)
		info.ScaleDamage(m_flExplosiveDamageScale);

	return BaseClass::OnTakeDamage(info);
}


//------------------------------------------------------------------------------
// Purpose: Increments the number of discs that this player has collected
//------------------------------------------------------------------------------
void CTwhlTower_Player::IncrementCollectedDiscsCount(const char* pszMessage)
{
	m_iCollectedDiscs++;

	CSingleUserRecipientFilter user(this);
	user.MakeReliable();

	UserMessageBegin(user, "DvdMessage");
		WRITE_BYTE(m_iCollectedDiscs);
		WRITE_BYTE(m_iTotalDiscs);
		WRITE_STRING(pszMessage);
	MessageEnd();
}


//----------------------------------------------------------------------------
// Purpose: Does the player have a weapon that uses this ammo type?
//----------------------------------------------------------------------------
bool CTwhlTower_Player::HasWeaponThatUsesAmmoType(int nAmmoIndex)
{
	for (int i = 0; i < MAX_WEAPONS; i++)
	{
		CBaseCombatWeapon* pWeapon = m_hMyWeapons.Get(i);
		if (!pWeapon)
			continue;

		if (pWeapon->GetPrimaryAmmoType() == nAmmoIndex)
			return true;

		if (pWeapon->GetSecondaryAmmoType() == nAmmoIndex)
			return true;
	}

	return false;
}


//----------------------------------------------------------------------------
// Purpose: Determine whether the given entity is an ammo source and this
//			player has a weapon that uses that ammo type.
//----------------------------------------------------------------------------
bool CTwhlTower_Player::HasWeaponForAmmoSource(CBaseEntity* pEntity)
{
	if (!pEntity)
		return false;

	// Is this an ammo crate?
	CItem_AmmoCrate* pAmmoCrate = dynamic_cast<CItem_AmmoCrate*>(pEntity);
	if (pAmmoCrate)
	{
		if (HasWeaponThatUsesAmmoType(pAmmoCrate->GetAmmoIndex()))
			return true;

		const char* pszWeapon = CItem_AmmoCrate::GetGiveWeapon(pAmmoCrate->GetAmmoType());
		if (!pszWeapon)
			return false;

		return (Weapon_OwnsThisType(pszWeapon) != NULL);
	}

	// Is this an item?
	if (!dynamic_cast<CItem*>(pEntity))
		return false;

	if (pEntity->ClassMatches("item_ammo_pistol*") && HasWeaponThatUsesAmmoType(GetAmmoDef()->Index("Pistol")))
		return true;

	if (pEntity->ClassMatches("item_ammo_smg1_grenade") && HasWeaponThatUsesAmmoType(GetAmmoDef()->Index("SMG1_Grenade")))
		return true;

	if (pEntity->ClassMatches("item_ammo_smg1*") && HasWeaponThatUsesAmmoType(GetAmmoDef()->Index("SMG1")))
		return true;

	if (pEntity->ClassMatches("item_ar2_altfire*") && HasWeaponThatUsesAmmoType(GetAmmoDef()->Index("AR2AltFire")))
		return true;

	if (pEntity->ClassMatches("item_ammo_ar2*") && HasWeaponThatUsesAmmoType(GetAmmoDef()->Index("AR2")))
		return true;

	if (pEntity->ClassMatches("item_ammo_357*") && HasWeaponThatUsesAmmoType(GetAmmoDef()->Index("357")))
		return true;

	if (pEntity->ClassMatches("item_ammo_crossbow*") && HasWeaponThatUsesAmmoType(GetAmmoDef()->Index("XBowBolt")))
		return true;

	if (pEntity->ClassMatches("item_rpg_round*") && HasWeaponThatUsesAmmoType(GetAmmoDef()->Index("RPG_Round")))
		return true;

	return false;
}


CLogicTwhlTowerPlayerProxy* CTwhlTower_Player::GetTwhlTowerPlayerProxy()
{
	return static_cast<CLogicTwhlTowerPlayerProxy*>(GetPlayerProxy());
}


void CTwhlTower_Player::InputSetMousePitchOverride(inputdata_t& inputdata)
{
	m_TwhlLocal.m_bIsMousePitchOverriden	= true;
	m_TwhlLocal.m_flMousePitchOverride		= inputdata.value.Float();
}


void CTwhlTower_Player::InputClearMousePitchOverride(inputdata_t& inputdata)
{
	m_TwhlLocal.m_bIsMousePitchOverriden	= false;
}


void CTwhlTower_Player::InputSetSideSpeedOverride(inputdata_t& inputdata)
{
	m_TwhlLocal.m_bIsSideSpeedOverriden	= true;
	m_TwhlLocal.m_flSideSpeedOverride	= inputdata.value.Float();
}


void CTwhlTower_Player::InputClearSideSpeedOverride(inputdata_t& inputdata)
{
	m_TwhlLocal.m_bIsSideSpeedOverriden	= false;
}


void CTwhlTower_Player::InputSetBackSpeedOverride(inputdata_t& inputdata)
{
	m_TwhlLocal.m_bIsBackSpeedOverriden	= true;
	m_TwhlLocal.m_flBackSpeedOverride	= inputdata.value.Float();
}


void CTwhlTower_Player::InputClearBackSpeedOverride(inputdata_t& inputdata)
{
	m_TwhlLocal.m_bIsBackSpeedOverriden	= false;
}


void CTwhlTower_Player::InputSetForwardSpeedOverride(inputdata_t& inputdata)
{
	m_TwhlLocal.m_bIsForwardSpeedOverriden	= true;
	m_TwhlLocal.m_flForwardSpeedOverride	= inputdata.value.Float();
}


void CTwhlTower_Player::InputClearForwardSpeedOverride(inputdata_t& inputdata)
{
	m_TwhlLocal.m_bIsForwardSpeedOverriden	= false;
}


void CTwhlTower_Player::InputSetExplosiveDamageScale(inputdata_t& inputdata)
{
	m_flExplosiveDamageScale = inputdata.value.Float();
}


//-----------------------------------------------------------------------------
// Purpose: Update target item glow effect
//-----------------------------------------------------------------------------
void CTwhlTower_Player::UpdateItemGlow()
{
	// Don't show item glows on background maps
	if (gpGlobals->eLoadType == MapLoad_Background)
	{
		if (m_hGlowTarget)
			SetGlowTarget(NULL);

		return;
	}

	// Don't highlight an entity if we're already carrying something
	if (m_hUseEntity)
	{
		if (m_hGlowTarget)
			SetGlowTarget(NULL);

		return;
	}

	// Check if we're within use range of an entity
	CBaseEntity* pGlowTarget = FindNearGlowTarget();
	if (pGlowTarget)
	{
		if (pGlowTarget != m_hGlowTarget)
		{
			// Set glow target to use target
			SetGlowTarget(pGlowTarget);
		}

		if (!m_bIsNearGlowTarget)
		{
			m_bIsNearGlowTarget = true;
			m_GlowColor = { 255, 255, 255, 255 };
		}

		return;
	}

	// Find item glow entity
	pGlowTarget = FindFarGlowTarget(ITEM_GLOW_MIN_DOT);
	if (pGlowTarget)
	{
		if (pGlowTarget != m_hGlowTarget)
			SetGlowTarget(pGlowTarget);
		
		float flDistance = (pGlowTarget->WorldSpaceCenter() - EyePosition()).Length();
		float flGlowStrength;

		const float flMaxDistance = g_sv_item_glow_distance_max.GetFloat();
		const float flNearDistance = g_sv_item_glow_distance_near.GetFloat();

		if (flDistance <= flNearDistance)
			flGlowStrength = 1.0F;
		else if (flDistance >= flMaxDistance)
			flGlowStrength = 0.0F;
		else
			flGlowStrength = 1.0F - ((flDistance - flNearDistance) / (flMaxDistance - flNearDistance));

		if (m_bIsNearGlowTarget)
			m_bIsNearGlowTarget = false;

		color32 glowColor;
		if (pGlowTarget->OverrideGlowColor())
		{
			glowColor = pGlowTarget->GetGlowColorOverride();
		}
		else
		{
			glowColor = {255, 208, 64, 255};
		}

		glowColor.a *= flGlowStrength;
		m_GlowColor = glowColor;

		return;
	}

	// Clear glow target
	if (m_hGlowTarget)
		SetGlowTarget(NULL);
}


//-----------------------------------------------------------------------------
// Purpose: Set the current item glow target entity
//-----------------------------------------------------------------------------
void CTwhlTower_Player::SetGlowTarget(CBaseEntity* pEntity)
{
	m_hGlowTarget = pEntity;

	if (pEntity)
	{
		m_hGlowProxy = pEntity->GetGlowProxy();

		IGameEvent* pEvent = gameeventmanager->CreateEvent("entity_highlighted");
		if (pEvent)
		{
			CBaseEntity* pProxy = m_hGlowProxy;

			pEvent->SetInt("userid", GetUserID());
			pEvent->SetInt("entity", pProxy->entindex());
			pEvent->SetString("classname", pProxy->GetClassname());
			pEvent->SetString("entityname", pProxy->GetEntityNameAsCStr());
			pEvent->SetBool("hasweapon", HasWeaponForAmmoSource(pProxy));
			pEvent->SetBool("near", m_bIsNearGlowTarget);

			gameeventmanager->FireEvent(pEvent);
		}
	}
	else
	{
		m_hGlowProxy = NULL;
	}
}


//-----------------------------------------------------------------------------
// Purpose: Find near item glow target entity
//-----------------------------------------------------------------------------
CBaseEntity* CTwhlTower_Player::FindNearGlowTarget()
{
	CBaseEntity* pUseEntity = FindUseEntity();
	if (!pUseEntity)
		return NULL;

	if (!pUseEntity->UseNearItemGlow())
		return NULL;

	return pUseEntity;
}


//-----------------------------------------------------------------------------
// Purpose: Find a distant item glow target entity
//-----------------------------------------------------------------------------
CBaseEntity* CTwhlTower_Player::FindFarGlowTarget(float flMinDot)
{
	const float flMaxDistance = g_sv_item_glow_distance_max.GetFloat();
	const unsigned int uUseMask = MASK_SOLID | CONTENTS_DEBRIS | CONTENTS_PLAYERCLIP;

	// First check if we're looking directly at an entity
	Vector vecStart = EyePosition();
	Vector vecForward;
	EyeVectors(&vecForward);

	trace_t tr;
	UTIL_TraceLine(vecStart, vecStart + vecForward * flMaxDistance, uUseMask, this, COLLISION_GROUP_NONE, &tr);

	if (tr.DidHit() && (IsUseableEntity(tr.m_pEnt, 0) || tr.m_pEnt->ForceGlow()) && tr.m_pEnt->UseFarItemGlow())
		return tr.m_pEnt;

	// Find entity closest to player crosshair
	CBaseEntity* pEnt = NULL;
	CBaseEntity* pBestEnt = NULL;
	float flBestDot = flMinDot;
	while ((pEnt = UTIL_EntitiesInPVS(this, pEnt)) != NULL)
	{
		// Only show far glow on usable entities with far glow enabled
		if (!(IsUseableEntity(pEnt, 0) || pEnt->ForceGlow()) || !pEnt->UseFarItemGlow())
			continue;

		Vector vecEnd = pEnt->WorldSpaceCenter();
		Vector vecDistance = vecEnd - vecStart;
		float flDistance = vecDistance.Length();

		// Don't show glow on entities that are too far away
		if (flDistance > flMaxDistance)
			continue;

		UTIL_TraceLine(vecStart, vecEnd, uUseMask, this, COLLISION_GROUP_NONE, &tr);

		// Is the glow entity or its proxy visible?
		if (tr.DidHit() && (tr.m_pEnt != pEnt && tr.m_pEnt != pEnt->GetGlowProxy()))
			continue;

		float flDot = vecForward.Dot(vecDistance.Normalized());

		if (flDot <= flBestDot)
			continue;

		flBestDot = flDot;
		pBestEnt = pEnt;
	}

	if (!pBestEnt)
		return NULL;

	return pBestEnt;
}

