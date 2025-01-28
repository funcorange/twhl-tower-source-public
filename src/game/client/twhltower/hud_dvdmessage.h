//==============================================================================
//
// Purpose: DVD pickup message hud element
//
//==============================================================================

#ifndef HUD_DVDMESSAGE_H
#define HUD_DVDMESSAGE_H

#include "hudelement.h"
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/ImagePanel.h"
#include "vgui_controls/Label.h"

class CHudDvdMessage : public CHudElement, public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE(CHudDvdMessage, vgui::EditablePanel);

	CHudDvdMessage(const char* pszElementName);
	virtual ~CHudDvdMessage();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void ApplySchemeSettings(vgui::IScheme* pScheme) override;
	virtual bool ShouldDraw() override;
	virtual void OnTick() override;

	void ShowDvdMessage(int iCollected, int iMax, const wchar_t* pwszMessage);
	void MsgFunc_DvdMessage(bf_read& msg);

private:
	void SetProgressText(int iNumCollected, int iNumTotal);

	vgui::EditablePanel*	m_pPanelBackground;
	vgui::ImagePanel*		m_pImagePanelIcon;
	vgui::Label*			m_pLabelProgress;
	vgui::Label*			m_pLabelMessage;
	vgui::Label*			m_pLabelProgressGlow;

	int	m_iCurrentValue;
	int m_iNextValue;
	int m_iMaxValue;
	float m_flHideTime;
	bool m_bHiding;
	float m_flUpdateTime;
	int m_iUpdate;
};

#endif // !HUD_DVDMESSAGE_H
