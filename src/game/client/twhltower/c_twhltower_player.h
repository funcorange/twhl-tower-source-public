//==============================================================================
//
// Purpose: Player for TWHL Tower: Source
//
//==============================================================================

#ifndef C_TWHLTOWER_PLAYER_H
#define C_TWHLTOWER_PLAYER_H

#include "c_basehlplayer.h"
#include "c_twhltower_playerlocaldata.h"

extern ConVar g_cl_enable_item_glow;

//==============================================================================
// C_TwhlTower_Player
//==============================================================================
class C_TwhlTower_Player : public C_BaseHLPlayer
{
public:
	DECLARE_CLASS(C_TwhlTower_Player, C_BaseHLPlayer);
	DECLARE_CLIENTCLASS();

	C_TwhlTower_Player();
	virtual ~C_TwhlTower_Player();

	void	OnDataChanged(DataUpdateType_t updateType) OVERRIDE;
	void	PostThink() OVERRIDE;
	inline CBaseEntity* GetPotentialUseEntity() { return m_hPotentialUseEntity; }

private:
	void UpdateGlowEffect();
	void DestroyGlowEffect();

	C_TwhlTowerPlayerLocalData	m_TwhlLocal;
	EHANDLE m_hGlowProxy;
	color32 m_GlowColor;
	CGlowObject* m_pGlowEffect;
	EHANDLE m_hPotentialUseEntity;
};

#endif // !C_TWHLTOWER_PLAYER_H
