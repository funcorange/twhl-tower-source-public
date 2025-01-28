//=============================================================================
//
// Purpose: Counter HUD element
//
//=============================================================================

#ifndef HUD_COUNTER_H
#define HUD_COUNTER_H

#include "twhltower_hudelement.h"
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/Label.h"

//=============================================================================
// CHudCounter
//=============================================================================
class CHudCounter : public CTWHLTowerHudElement, public vgui::EditablePanel
{
public:
	DECLARE_CLASS(CHudCounter, CTWHLTowerHudElement);

	CHudCounter(const char* pszElementName);
	virtual ~CHudCounter();

	void ApplySchemeSettings(vgui::IScheme* pScheme) OVERRIDE;
	void Init() OVERRIDE;
	void OnRestore() OVERRIDE;
	void OnTick() OVERRIDE;
	void PerformLayout() OVERRIDE;
	void Reset() OVERRIDE;
	bool ShouldDraw() OVERRIDE;

	void MsgFunc_HudCounter(bf_read& msg);

	DECLARE_DATADESC();

private:
	int		CalculateWidth();

	void	Show();
	void	Hide();

	void	SetMaxValue(int iMaxValue);
	void	SetTitle(const char* pszTitle);
	void	SetValue(int iValue, bool bUpdate = true);

	bool			m_bShouldDraw;
	bool			m_bHiding;
	float			m_flHideTime;
	int				m_iMaxValue;
	int				m_iValue;
	vgui::Panel*	m_pBackgroundPanel;
	vgui::Label*	m_pTitleLabel;
	vgui::Label*	m_pCountLabel;
	vgui::Label*	m_pCountGlowLabel;
	string_t		m_iszTitle;
};

#endif // !HUD_COUNTER_H
