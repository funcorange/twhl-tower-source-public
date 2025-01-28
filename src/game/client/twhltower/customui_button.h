//==============================================================================
//
// Purpose: Custom UI button
//
//==============================================================================

#ifndef CUSTOMUI_BUTTON_H
#define CUSTOMUI_BUTTON_H

#include <vgui/IScheme.h>

#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/Panel.h"

class CCustomUIButton : public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE(CCustomUIButton, vgui::EditablePanel);

	CCustomUIButton(vgui::Panel* pParent, const char* pszPanelName);
	virtual ~CCustomUIButton();

	void	ApplySettings(KeyValues* inResourceData) override;
	void	ApplySchemeSettings(vgui::IScheme* pScheme) override;
	void	PerformLayout() override;

	void	OnCursorEntered() override;
	void	OnCursorExited() override;
	void	OnMousePressed(vgui::MouseCode code) override;
	void	OnMouseReleased(vgui::MouseCode code) override;
	void	OnKeyCodePressed(vgui::KeyCode code) override;
	void	OnKeyCodeTyped(vgui::KeyCode code) override;
	void	OnThink() override;

	void	NavigateTo() override;
	void	NavigateFrom() override;

	inline void		SetBackgroundOutColor(Color color) { m_colBackgroundOutColor = color; }
	inline void		SetBackgroundOverColor(Color color) { m_colBackgroundOverColor = color; }
	inline void		SetBackgroundPressedColor(Color color) { m_colBackgroundPressedColor = color; }
	inline void		SetBackgroundActiveColor(Color color) { m_colBackgroundActiveColor = color; }
	inline void		SetTextOutColor(Color color) { m_colTextOutColor = color; }
	inline void		SetTextOverColor(Color color) { m_colTextOverColor = color; }
	inline void		SetTextPressedColor(Color color) { m_colTextPressedColor = color; }
	inline void		SetTextActiveColor(Color color) { m_colTextActiveColor = color; }
	inline bool		IsActive() { return m_bIsActive; }

	void			SetActive(bool bActive);
	void			RunCommand();

protected:
	virtual void	Press();
	virtual void	UnPress();
	virtual void	OnPressed();

	vgui::Panel*	m_pBackgroundPanel;
	vgui::Label*	m_pLabelText;

private:
	void			Highlight();
	void			UnHighlight();
	void			SetPressed(bool bPressed);

	bool			m_bIsHighlighted;	// This button is currently highlighted
	bool			m_bIsPressed;		// This button is currently pressed
	const char*		m_pszCommand;		// Client command that is run when button is pressed
	bool			m_bIsActive;		// This button is active

	CPanelAnimationVar(Color, m_colBackgroundOutColor, "BackgroundOutColor", "0 0 0 0");
	CPanelAnimationVar(Color, m_colBackgroundOverColor, "BackgroundOverColor", "0 0 0 0");
	CPanelAnimationVar(Color, m_colBackgroundPressedColor, "BackgroundPressedColor", "0 0 0 0");
	CPanelAnimationVar(Color, m_colBackgroundActiveColor, "BackgroundActiveColor", "0 0 0 0");
	CPanelAnimationVar(Color, m_colTextOutColor, "TextOutColor", "0 0 0 0");
	CPanelAnimationVar(Color, m_colTextOverColor, "TextOverColor", "0 0 0 0");
	CPanelAnimationVar(Color, m_colTextPressedColor, "TextPressedColor", "0 0 0 0");
	CPanelAnimationVar(Color, m_colTextActiveColor, "TextActiveColor", "0 0 0 0");
	CPanelAnimationVar(float, m_flSelectAnimationDuration, "SelectAnimationDuration", "0");
	CPanelAnimationVar(float, m_flDeselectAnimationDuration, "DeselectAnimationDuration", "0");
	CPanelAnimationVar(float, m_flPressAnimationDuration, "PressAnimationDuration", "0");
	CPanelAnimationVar(float, m_flUnpressAnimationDuration, "UnpressAnimationDuration", "0");
};

#endif // !CUSTOMUI_BUTTON_H
