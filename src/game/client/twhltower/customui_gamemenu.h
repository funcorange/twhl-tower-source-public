//==============================================================================
//
// Purpose: Custom UI game menu
//
//==============================================================================

#ifndef CUSTOMUI_GAMEMENU_H
#define CUSTOMUI_GAMEMENU_H

#include <vgui/IScheme.h>
#include <vgui/MouseCode.h>

#include "customui_menubutton.h"
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/Frame.h"
#include "vgui_controls/ImagePanel.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/Panel.h"

class CCustomUIGameMenu : public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE(CCustomUIGameMenu, vgui::EditablePanel);

	CCustomUIGameMenu(vgui::Panel* pParent);
	~CCustomUIGameMenu();

	virtual void ApplySchemeSettings(vgui::IScheme* pScheme) override;
	virtual void PerformLayout() override;

	virtual void OnMousePressed(vgui::MouseCode code) override;

	virtual void OnNavigateTo(const char* pszPanelName) override;

	virtual void OnThink() override;

	MESSAGE_FUNC(OnLockNav, "LockNav");
	MESSAGE_FUNC(OnUnlockNav, "UnlockNav");

private:

	void	UpdateButtons();
	int		GetButtonsOffsetX() const;
	int		GetButtonsOffsetY() const;
	void	HighlightButton(CCustomUIMenuButton* pButton);

	bool			m_bIsInGame;
	vgui::IScheme*	m_pScheme;
	vgui::ImagePanel*	m_pImagePanelLogo;
	CUtlSortVector<CCustomUIMenuButton*, CCustomUIMenuButtonOrderLess> m_Buttons;
	bool			m_bIsNavLocked;
};

#endif // !CUSTOMUI_GAMEMENU_H
