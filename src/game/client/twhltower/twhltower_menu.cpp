//=============================================================================
//
// Purpose: TWHL Tower: Source mod options menu
//
//=============================================================================

#include "cbase.h"
#include "twhltower_menu.h"

#include <vgui/IPanel.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>

#include "ienginevgui.h"
#include "vgui_controls/Controls.h"
#include "vgui_controls/cvartogglecheckbutton.h"
#include "vgui_controls/Frame.h"
#include "vgui_controls/PropertySheet.h"

extern ConVar v_viewmodel_fov;

static TwhlTowerMenu* g_pTwhlTowerMenu;

CON_COMMAND(twhltower_menu_show, "Show TWHL Tower: Source options menu")
{
	if (g_pTwhlTowerMenu)
	{
		g_pTwhlTowerMenu->Activate();
		return;
	}

	vgui::VPANEL gameUiPanel = enginevgui->GetPanel(PANEL_GAMEUIDLL);
	if (gameUiPanel == 0)
		return;

	TwhlTowerMenu* pFrame = new TwhlTowerMenu(
		NULL,
		"TwhlTowerMenu"
	);

	if (pFrame == NULL)
		return;

	pFrame->SetParent(gameUiPanel);
	pFrame->SetEnabled(true);
	pFrame->SetVisible(true);
	pFrame->Activate();

	g_pTwhlTowerMenu = pFrame;
}


//=============================================================================
//
// TwhlTowerMenu
//
//=============================================================================

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
TwhlTowerMenu::TwhlTowerMenu(vgui::Panel* pParent, const char* pszPanelName) :
	BaseClass(pParent, pszPanelName)
{
	int screenW = 0;
	int screenH = 0;
	vgui::surface()->GetScreenSize(screenW, screenH);

	int w = 512;
	int h = 406;
	int x = (screenW - w) / 2;
	int y = (screenH - h) / 2;

	SetProportional(false);
	SetDeleteSelfOnClose(true);
	SetBounds(x, y, w, h);
	SetSizeable(false);
	SetApplyButtonVisible(true);
	SetTitle("#GameUI_GameplaySettings", true);

	// Create item glow checkbox
	m_pItemGlowCheckBox = new vgui::CvarToggleCheckButton<ConVarRef>(
		GetPropertySheet(),
		"ItemGlowCheckBox",
		"#GameplaySettings_EnableItemGlow",
		"cl_enable_item_glow"
	);
	if (m_pItemGlowCheckBox)
	{
		m_pItemGlowCheckBox->AddActionSignalTarget(this);
	}

	// Create game instructor checkbox
	m_pGameInstructorCheckBox = new vgui::CvarToggleCheckButton<ConVarRef>(
		GetPropertySheet(),
		"GameInstructorCheckBox",
		"#GameplaySettings_EnableGameInstructor",
		"gameinstructor_enable"
	);
	if (m_pGameInstructorCheckBox)
	{
		m_pGameInstructorCheckBox->AddActionSignalTarget(this);
	}

	// Create show weapon slots checkbox
	m_pShowEmptyWeaponSlotsCheckBox =
		new vgui::CvarToggleCheckButton<ConVarRef>(
			GetPropertySheet(),
			"ShowEmptyWeaponSlotsCheckBox",
			"#GameplaySettings_ShowEmptyWeaponSlots",
			"hud_showemptyweaponslots"
		);
	if (m_pShowEmptyWeaponSlotsCheckBox)
	{
		m_pShowEmptyWeaponSlotsCheckBox->AddActionSignalTarget(this);
	}

	// Create view model FOV slider
	m_pViewModelFovSlider = new vgui::Slider(
		GetPropertySheet(),
		"ViewModelFovSlider"
	);
	if (m_pViewModelFovSlider)
	{
		m_pViewModelFovSlider->AddActionSignalTarget(this);
	}

	LoadControlSettings("resource/twhltowermenu.res");
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
TwhlTowerMenu::~TwhlTowerMenu()
{
	g_pTwhlTowerMenu = NULL;
}


//-----------------------------------------------------------------------------
// Purpose: Called when the panel is shown
//-----------------------------------------------------------------------------
void TwhlTowerMenu::Activate()
{
	BaseClass::Activate();

	if (m_pViewModelFovSlider)
	{
		int iMin = 0;
		int iMax = 0;
		m_pViewModelFovSlider->GetRange(iMin, iMax);
		const int iRange = iMax - iMin;
		const int iValue = static_cast<int>(iRange
			* (v_viewmodel_fov.GetFloat() - 54.0F) / (70.0F - 54.0F) + iMin);
		m_pViewModelFovSlider->SetValue(iValue);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Called when the OK button is pressed
//-----------------------------------------------------------------------------
bool TwhlTowerMenu::OnOK(bool bApplyOnly)
{
	m_pItemGlowCheckBox->ApplyChanges();
	m_pGameInstructorCheckBox->ApplyChanges();
	m_pShowEmptyWeaponSlotsCheckBox->ApplyChanges();
	ApplyViewModelFovChanges();

	return BaseClass::OnOK(bApplyOnly);
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void TwhlTowerMenu::OnControlModified(vgui::Panel* panel)
{
	EnableApplyButton(true);
}


//-----------------------------------------------------------------------------
// Purpose: Update viewmodel_fov cvar based on slider value
//-----------------------------------------------------------------------------
void TwhlTowerMenu::ApplyViewModelFovChanges()
{
	if (!m_pViewModelFovSlider)
		return;

	int iMin, iMax;
	m_pViewModelFovSlider->GetRange(iMin, iMax);
	int iValue = m_pViewModelFovSlider->GetValue();

	float flValue =
		static_cast<float>(iValue - iMin) / static_cast<float>(iMax - iMin);
	flValue = flValue * (70.0F - 54.0F) + 54.0F;

	v_viewmodel_fov.SetValue(flValue);
}

