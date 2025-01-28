//==============================================================================
//
// Purpose: Player for TWHL Tower: Source
//
//==============================================================================

#ifndef TWHLTOWER_PLAYER_H
#define TWHLTOWER_PLAYER_H

#include "hl2_player.h"
#include "logic_twhltower_player_proxy.h"
#include "twhltower_playerlocaldata.h"

extern ConVar g_sv_total_disc_count;
extern ConVar g_sv_item_glow_distance_max;
extern ConVar g_sv_item_glow_distance_near;

//==============================================================================
// CTwhlTower_Player
//==============================================================================
class CTwhlTower_Player : public CHL2_Player
{
public:
	DECLARE_CLASS(CTwhlTower_Player, CHL2_Player);
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();

	CTwhlTower_Player();

	void	Spawn() OVERRIDE;
	void	PostThink() OVERRIDE;
	int		OnTakeDamage(const CTakeDamageInfo& inputInfo) OVERRIDE;

	void	IncrementCollectedDiscsCount(const char* pszMessage = NULL);
	inline int GetCollectedDiscCount() { return m_iCollectedDiscs; }
	inline void	SetTotalDiscCount(int iTotalDiscs) { m_iTotalDiscs = iTotalDiscs; }
	inline int GetTotalDiscCount() { return m_iTotalDiscs; }
	bool	HasWeaponThatUsesAmmoType(int nAmmoIndex);
	bool	HasWeaponForAmmoSource(CBaseEntity* pEntity);
	inline CBaseEntity* GetPotentialUseEntity() { return m_hPotentialUseEntity; }
	CLogicTwhlTowerPlayerProxy* GetTwhlTowerPlayerProxy();

private:
	void	InputSetMousePitchOverride(inputdata_t& inputdata);
	void	InputClearMousePitchOverride(inputdata_t& inputdata);
	void	InputSetSideSpeedOverride(inputdata_t& inputdata);
	void	InputClearSideSpeedOverride(inputdata_t& inputdata);
	void	InputSetBackSpeedOverride(inputdata_t& inputdata);
	void	InputClearBackSpeedOverride(inputdata_t& inputdata);
	void	InputSetForwardSpeedOverride(inputdata_t& inputdata);
	void	InputClearForwardSpeedOverride(inputdata_t& inputdata);
	void	InputSetExplosiveDamageScale(inputdata_t& inputdata);
	void	UpdateItemGlow();
	void	SetGlowTarget(CBaseEntity* pEntity);
	CBaseEntity*	FindNearGlowTarget();
	CBaseEntity*	FindFarGlowTarget(float flMinDot = -1.0F);

	int m_iCollectedDiscs;
	int m_iTotalDiscs;
	bool m_bIsNearGlowTarget;
	EHANDLE m_hGlowTarget;
	float m_flExplosiveDamageScale;

	CNetworkVarEmbedded(CTwhlTowerPlayerLocalData, m_TwhlLocal);
	CNetworkHandle(CBaseEntity, m_hGlowProxy);
	CNetworkColor32(m_GlowColor);
	CNetworkHandle(CBaseEntity, m_hPotentialUseEntity);
	EHANDLE m_hLookAtEntity;
};

#endif // !TWHLTOWER_PLAYER_H
