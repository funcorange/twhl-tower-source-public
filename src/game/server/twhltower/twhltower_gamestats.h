//============================================================================
//
// TWHL Tower: Source game stats
//
//============================================================================

#ifndef TWHLTOWER_GAMESTATS_H
#define TWHLTOWER_GAMESTATS_H

#include "ep2_gamestats.h"

class CTwhlTowerGameStats : public CEP2GameStats
{
public:
	using BaseClass = CEP2GameStats;

	CTwhlTowerGameStats();
	virtual ~CTwhlTowerGameStats();

	inline bool	StatTrackingEnabledForMod() OVERRIDE { return true; }
	bool		UserPlayedAllTheMaps() OVERRIDE;

	const char* GetStatSaveFileName() OVERRIDE;
	const char* GetStatUploadRegistryKeyName() OVERRIDE;

#ifdef GAME_DLL
	void		Event_WeaponFired(CBasePlayer* pShooter, bool bPrimary, char const* pchWeaponName) OVERRIDE;
#endif
};

#endif // !TWHLTOWER_GAMESTATS_H
