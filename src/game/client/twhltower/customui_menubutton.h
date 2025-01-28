//==============================================================================
//
// Purpose: Custom UI game menu button
//
//==============================================================================

#ifndef CUSTOMUI_MENUBUTTON_H
#define CUSTOMUI_MENUBUTTON_H

#include <vgui/IScheme.h>
#include <vgui/MouseCode.h>

#include "KeyValues.h"
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/Panel.h"

class CCustomUIMenuButton : public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE(CCustomUIMenuButton, vgui::EditablePanel);

	CCustomUIMenuButton(vgui::Panel* pParent, const char* pszPanelName);

	virtual void ApplySettings(KeyValues* inResourceData) override;
	virtual void ApplySchemeSettings(vgui::IScheme* pScheme) override;
	virtual void PerformLayout() override;

	virtual void OnThink() override;
	virtual void OnCursorEntered() override;
	virtual void OnCursorExited() override;
	virtual void OnMousePressed(vgui::MouseCode code) override;
	virtual void OnMouseReleased(vgui::MouseCode code) override;
	virtual void OnKeyCodePressed(vgui::KeyCode code) override;
	virtual void OnKeyCodeTyped(vgui::KeyCode code) override;

	virtual void NavigateTo() override;
	virtual void NavigateFrom() override;

	virtual inline int	GetInGameOrder() const { return m_iInGameOrder; }
	virtual inline bool	IsOnlyInGame() const { return m_bOnlyInGame; }
	virtual inline bool IsDeveloperOnly() const { return m_bIsDeveloperOnly; }

	virtual void SetPressed(bool bPressed);
	virtual void SetHighlighted(bool bHighlighted);

private:
	void RunCommand();
	void Press();
	void Unpress();
	void Highlight();
	void Unhighlight();

	char		m_szCommand[512];
	int			m_iInGameOrder;
	bool		m_bOnlyInGame;
	bool		m_bIsHighlighted;
	vgui::IScheme*	m_pScheme;
	bool		m_bIsPressed;
	bool		m_bIsDeveloperOnly;

	// Animation settings
	int			m_iLeftBorderWidthOut;
	int			m_iLeftBorderWidthOver;
	int			m_iLeftBorderWidthPressed;

	float		m_flSelectAnimationDuration;
	float		m_flDeselectAnimationDuration;
	float		m_flPressAnimationDuration;
	float		m_flUnpressAnimationDuration;

	// Colors
	Color		m_colBackgroundOutColor;
	Color		m_colBackgroundOverColor;
	Color		m_colBackgroundPressedColor;

	Color		m_colLeftBorderOutColor;
	Color		m_colLeftBorderOverColor;
	Color		m_colLeftBorderPressedColor;

	Color		m_colTextOutColor;
	Color		m_colTextOverColor;
	Color		m_colTextPressedColor;

	// Elements
	vgui::Panel*		m_pPanelBackground;
	vgui::Panel*		m_pPanelLeftBorder;
	vgui::Label*		m_pLabelText;
};


class CCustomUIMenuButtonOrderLess
{
public:
	inline bool Less(CCustomUIMenuButton* const& b1, CCustomUIMenuButton* const& b2, void* pCtx) const
	{
		return b1->GetInGameOrder() < b2->GetInGameOrder();
	}
};

#endif // !CUSTOMUI_MENUBUTTON_H
