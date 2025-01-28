//============================================================================
//
// Purpose: Entity for firing game instructor events
//
//============================================================================

#ifndef INFO_GAME_EVENT_PROXY_H
#define INFO_GAME_EVENT_PROXY_H

#define SF_GAMEEVENTPROXY_AUTOVISIBILITY	0x01

class CInfoGameEventProxy : public CPointEntity
{
public:
	DECLARE_CLASS(CInfoGameEventProxy, CPointEntity);
	DECLARE_DATADESC();

	CInfoGameEventProxy();

	void		Spawn() OVERRIDE;

	inline bool	IsEnabled() const { return !m_bDisabled; }

private:
	void		AutoVisibilityThink();
	void		GenerateEvent(CBaseEntity* pActivator);
	bool		IsEntityInRadius(CBaseEntity* pEntity) const;
	void		InputGenerateEvent(inputdata_t& inputdata);
	inline void	InputEnable(inputdata_t& inputdata) { m_bDisabled = false; }
	inline void	InputDisable(inputdata_t& inputdata) { m_bDisabled = true; }
	bool		IsEntityVisible(CBaseEntity* pEntity);
	bool		IsEntityInVisibleSet(CBaseEntity* pEntity);
	void		RemoveEntityFromVisibleSet(CBaseEntity* pEntity);
	void		AddEntityToVisibleSet(CBaseEntity* pEntity);

	string_t	m_iszEventName;		// Name of the game event to fire
	float		m_flRadius;			// Range of the event
	string_t	m_iszSubjectName;	// Name of the entity to use as subject
	bool		m_bDisabled;		// Entity is disabled
	CUtlVector<EHANDLE> m_VisibleEntities;

};

#endif // !INFO_GAME_EVENT_PROXY_H
