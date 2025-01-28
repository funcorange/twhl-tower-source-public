//=============================================================================
//
// Purpose: HUD counter entity
//
//=============================================================================

#ifndef ENV_HUD_COUNTER_H
#define ENV_HUD_COUNTER_H

class CEnvHudCounter : public CLogicalEntity
{
public:
	DECLARE_CLASS(CEnvHudCounter, CLogicalEntity);

	CEnvHudCounter();

	void		Spawn() OVERRIDE;

	void		ShowCounter();
	void		HideCounter();
	bool		GetShowState();
	void		SetValue(int iValue);

	DECLARE_DATADESC();

private:
	void		InputShowCounter(inputdata_t& inputdata);
	void		InputHideCounter(inputdata_t& inputdata);
	void		InputSetValue(inputdata_t& inputdata);

	// Title to show on HUD counter
	string_t	m_iszTitle;
	// Max value of the counter
	int			m_nMaxCount;
	// Current value of the counter
	int			m_nValue;
	// Whether or not this counter is currently being shown on the HUD
	bool		m_bIsShown;
};

#endif // !ENV_HUD_COUNTER_H
