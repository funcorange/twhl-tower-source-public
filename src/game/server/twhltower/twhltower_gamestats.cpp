//============================================================================
//
// TWHL Tower: Source game stats
//
//============================================================================

#include "cbase.h"
#include "twhltower_gamestats.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static CTwhlTowerGameStats s_CTwhlTowerGameStats_Singleton;
CBaseGameStats* g_pTwhlTowerGameStats = &s_CTwhlTowerGameStats_Singleton;
static const char* s_rgpszTwhlTowerMaps[] =
{
	"tower_01_drorange",
	"tower_02_novadelta",
	"tower_03_kouta",
	"tower_04_tmob",
	"tower_05_gravitycat",
	"tower_06_drorange",
	"tower_07_kriegmorder",
	"tower_08_cambreaker",
	"tower_09_q0",
	"tower_10_shoes",
	"tower_11_shambles",
	"tower_12_jen3r1k",
	"tower_13_aqualuzara",
	"tower_14_jinks",
	"tower_15_shoes",
	"tower_16_skorly",
	"tower_17_xinus22",
	"tower_18_drorange"
};

//----------------------------------------------------------------------------
// Purpose: Constructor
//----------------------------------------------------------------------------
CTwhlTowerGameStats::CTwhlTowerGameStats() :
	BaseClass()
{
}


//----------------------------------------------------------------------------
// Purpose: Destructor
//----------------------------------------------------------------------------
CTwhlTowerGameStats::~CTwhlTowerGameStats()
{
}


//----------------------------------------------------------------------------
// Purpose: Has the user played all the maps in this mod?
//----------------------------------------------------------------------------
bool CTwhlTowerGameStats::UserPlayedAllTheMaps()
{
	const int c = ARRAYSIZE(s_rgpszTwhlTowerMaps);
	for (int i = 0; i < c; i++)
	{
		int idx = m_BasicStats.m_MapTotals.Find(s_rgpszTwhlTowerMaps[i]);
		if (idx == m_BasicStats.m_MapTotals.InvalidIndex())
			return false;
	}

	return true;
}


//----------------------------------------------------------------------------
// Purpose: Get game stats save file name
//----------------------------------------------------------------------------
const char* CTwhlTowerGameStats::GetStatSaveFileName()
{
	return "twhltower_gamestats.dat";
}


//----------------------------------------------------------------------------
// Purpose:
//----------------------------------------------------------------------------
const char* CTwhlTowerGameStats::GetStatUploadRegistryKeyName()
{
	return "GameStatsUpload_TwhlTower";
}


//----------------------------------------------------------------------------
// Purpose:
//----------------------------------------------------------------------------
void CTwhlTowerGameStats::Event_WeaponFired(CBasePlayer* pShooter, bool bPrimary, char const* pchWeaponName)
{
	IGameEvent* pEvent = gameeventmanager->CreateEvent("weapon_fire");
	if (pEvent)
	{
		pEvent->SetInt("userid", pShooter->GetUserID());
		pEvent->SetString("weapon", pchWeaponName);
		pEvent->SetBool("secondary", !bPrimary);

		gameeventmanager->FireEvent(pEvent);
	}

	BaseClass::Event_WeaponFired(pShooter, bPrimary, pchWeaponName);
}

