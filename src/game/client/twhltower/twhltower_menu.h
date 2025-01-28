//=============================================================================
//
// Purpose: TWHL Tower: Source mod options menu
//
//=============================================================================

#ifndef TWHLTOWER_MENU_H
#define TWHLTOWER_MENU_H

#include "vgui_controls/cvartogglecheckbutton.h"
#include "vgui_controls/PropertyDialog.h"
#include "vgui_controls/PropertyPage.h"
#include "vgui_controls/Slider.h"

//=============================================================================
// TwhlTowerMenu
//=============================================================================
class TwhlTowerMenu : public vgui::PropertyDialog
{
public:
	DECLARE_CLASS_SIMPLE(TwhlTowerMenu, vgui::PropertyDialog);

	TwhlTowerMenu(vgui::Panel* pParent, const char* pszPanelName);
	virtual ~TwhlTowerMenu();

	void Activate() OVERRIDE;

protected:
	bool OnOK(bool bApplyOnly) OVERRIDE;

private:
	MESSAGE_FUNC_PTR(OnControlModified, "ControlModified", panel);
	MESSAGE_FUNC_PTR(OnCheckButtonChecked, "CheckButtonChecked", panel)
	{
		OnControlModified(panel);
	}
	MESSAGE_FUNC_PTR(OnSliderMoved, "SliderMoved", panel)
	{
		OnControlModified(panel);
	}
	void ApplyViewModelFovChanges();

	vgui::CvarToggleCheckButton<ConVarRef>* m_pItemGlowCheckBox;
	vgui::CvarToggleCheckButton<ConVarRef>* m_pGameInstructorCheckBox;
	vgui::CvarToggleCheckButton<ConVarRef>* m_pShowEmptyWeaponSlotsCheckBox;
	vgui::Slider* m_pViewModelFovSlider;
};

#endif // TWHLTOWER_MENU_H
