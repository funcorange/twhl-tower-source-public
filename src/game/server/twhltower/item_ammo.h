//============================================================================
//
// Purpose: The various ammo types for HL2
//
//============================================================================

#ifndef ITEM_AMMO_H
#define ITEM_AMMO_H

enum
{
	AMMOCRATE_SMALL_ROUNDS,
	AMMOCRATE_MEDIUM_ROUNDS,
	AMMOCRATE_LARGE_ROUNDS,
	AMMOCRATE_RPG_ROUNDS,
	AMMOCRATE_BUCKSHOT,
	AMMOCRATE_GRENADES,
	AMMOCRATE_357,
	AMMOCRATE_CROSSBOW,
	AMMOCRATE_AR2_ALTFIRE,
	AMMOCRATE_SMG_ALTFIRE,
#ifdef MAPBASE
	AMMOCRATE_SLAM,
	AMMOCRATE_EMPTY,
#endif
	NUM_AMMO_CRATE_TYPES,
};

class CItem_AmmoCrate : public CBaseAnimating
{
public:
	DECLARE_CLASS(CItem_AmmoCrate, CBaseAnimating);

	void	Spawn(void);
	void	Precache(void);
	bool	CreateVPhysics(void);

	virtual void HandleAnimEvent(animevent_t* pEvent);

	void	SetupCrate(void);
	void	OnRestore(void);

	//FIXME: May not want to have this used in a radius
	int		ObjectCaps(void) { return (BaseClass::ObjectCaps() | (FCAP_IMPULSE_USE | FCAP_USE_IN_RADIUS)); };
	void	Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

	void	InputKill(inputdata_t& data);
	void	CrateThink(void);

	virtual int OnTakeDamage(const CTakeDamageInfo& info);

#ifdef TWHLTOWER
	bool	UseNearItemGlow() OVERRIDE { return true; }
	bool	UseFarItemGlow() OVERRIDE { return true; }

	inline int	GetAmmoType() { return m_nAmmoType; }
	inline int	GetAmmoIndex() { return m_nAmmoIndex; }
	static inline const char* GetGiveWeapon(int nAmmoType)
	{
		if (nAmmoType < 0 || nAmmoType >= NUM_AMMO_CRATE_TYPES)
			return NULL;

		return m_pGiveWeapon[nAmmoType];
	}
#endif

protected:

	int		m_nAmmoType;
	int		m_nAmmoIndex;

	static const char* m_lpzModelNames[NUM_AMMO_CRATE_TYPES];
	static const char* m_lpzAmmoNames[NUM_AMMO_CRATE_TYPES];
	static int m_nAmmoAmounts[NUM_AMMO_CRATE_TYPES];
	static const char* m_pGiveWeapon[NUM_AMMO_CRATE_TYPES];

	float	m_flCloseTime;
	COutputEvent	m_OnUsed;
	CHandle< CBasePlayer > m_hActivator;

#ifdef MAPBASE
	COutputEvent	m_OnAmmoTaken;
#endif

	DECLARE_DATADESC();
};

#endif // !ITEM_AMMO_H
