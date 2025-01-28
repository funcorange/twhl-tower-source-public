//=============================================================================
//
// Purpose: TWHL Tower: Source options menu settings
//
//=============================================================================

#include "cbase.h"
#include "twhltower_options.h"

#include "ienginevgui.h"
#include "vgui/IPanel.h"
#include "vgui/IVGui.h"
#include "vgui_controls/Controls.h"

static CTwhlTowerOptions g_TwhlTowerOptions;

IGameSystem* TwhlTowerOptions()
{
	return &g_TwhlTowerOptions;
}

//=============================================================================
// 
// CTwhlTowerOptions
// 
//=============================================================================

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTwhlTowerOptions::CTwhlTowerOptions() :
	m_hOptionsDialog(vgui::INVALID_PANEL),
	m_hOptionsSubVideo(vgui::INVALID_PANEL),
	CBaseGameSystemPerFrame()
{
}


//-----------------------------------------------------------------------------
// Purpose: Called before rendering
//-----------------------------------------------------------------------------
void CTwhlTowerOptions::Update(float)
{
	if (m_hOptionsDialog == vgui::INVALID_PANEL)
	{
		m_hOptionsDialog = GetOptionsDialog();

		if (m_hOptionsDialog != vgui::INVALID_PANEL)
		{
			DevMsg("CTwhlTowerOptions: Found OptionsDialog\n");
		}
	}
	else if (vgui::ivgui()->HandleToPanel(m_hOptionsDialog) == 0)
	{
		m_hOptionsDialog = vgui::INVALID_PANEL;
		DevMsg("CTwhlTowerOptions: Lost OptionsDialog\n");
	}

	if (m_hOptionsSubVideo == vgui::INVALID_PANEL)
	{
		m_hOptionsSubVideo = GetOptionsSubVideo();

		if (m_hOptionsSubVideo != vgui::INVALID_PANEL)
		{
			DevMsg("CTwhlTowerOptions: Found OptionsSubVideo\n");
		}
	}
	else if (vgui::ivgui()->HandleToPanel(m_hOptionsSubVideo) == 0)
	{
		m_hOptionsSubVideo = vgui::INVALID_PANEL;
		DevMsg("CTwhlTowerOptions: Lost OptionsSubVideo\n");
	}
}


//-----------------------------------------------------------------------------
// Purpose: Get handle to Game UI options dialog
//-----------------------------------------------------------------------------
vgui::HPanel CTwhlTowerOptions::GetOptionsDialog()
{
	if (!enginevgui->IsGameUIVisible())
		return vgui::INVALID_PANEL;

	// Get GameUI panel
	vgui::VPANEL gameUiPanel = enginevgui->GetPanel(PANEL_GAMEUIDLL);
	if (gameUiPanel == 0)
		return vgui::INVALID_PANEL;

	// Find BaseGameUIPanel
	int nGameUiPanelChildCount = vgui::ipanel()->GetChildCount(gameUiPanel);
	vgui::VPANEL baseGameUiPanel = 0;
	for (int i = 0; i < nGameUiPanelChildCount; ++i)
	{
		vgui::VPANEL panel = vgui::ipanel()->GetChild(gameUiPanel, i);
		if (panel == 0 || panel == -1)
			continue;

		if (FStrEq(vgui::ipanel()->GetName(panel), "BaseGameUIPanel"))
		{
			baseGameUiPanel = panel;
			break;
		}
	}

	if (baseGameUiPanel == 0)
		return vgui::INVALID_PANEL;

	// Find OptionsDialog
	int nBaseGameUiPanelChildCount = vgui::ipanel()->GetChildCount(
		baseGameUiPanel
	);
	vgui::VPANEL optionsDialog = 0;
	for (int i = 0; i < nBaseGameUiPanelChildCount; ++i)
	{
		vgui::VPANEL panel = vgui::ipanel()->GetChild(baseGameUiPanel, i);
		if (panel == 0 || panel == -1)
			continue;

		if (FStrEq(vgui::ipanel()->GetName(panel), "OptionsDialog"))
		{
			optionsDialog = panel;
			break;
		}
	}

	if (optionsDialog == 0)
		return vgui::INVALID_PANEL;

	return vgui::ivgui()->PanelToHandle(optionsDialog);
}


//-----------------------------------------------------------------------------
// Purpose: Get video options panel
//-----------------------------------------------------------------------------
vgui::HPanel CTwhlTowerOptions::GetOptionsSubVideo()
{
	if (m_hOptionsDialog == vgui::INVALID_PANEL)
		return vgui::INVALID_PANEL;

	vgui::VPANEL optionsDialog = vgui::ivgui()->HandleToPanel(
		m_hOptionsDialog
	);
	if (optionsDialog == 0)
		return vgui::INVALID_PANEL;

	// Find Sheet
	int nOptionsDialogChildCount = vgui::ipanel()->GetChildCount(
		optionsDialog
	);
	vgui::VPANEL sheet = 0;
	for (int i = 0; i < nOptionsDialogChildCount; ++i)
	{
		vgui::VPANEL panel = vgui::ipanel()->GetChild(optionsDialog, i);

		if (FStrEq(vgui::ipanel()->GetName(panel), "Sheet"))
		{
			sheet = panel;
			break;
		}
	}

	if (sheet == 0)
		return vgui::INVALID_PANEL;

	// Find OptionsSubVideo
	int nSheetChildCount = vgui::ipanel()->GetChildCount(sheet);
	vgui::VPANEL optionsSubVideo = 0;
	for (int i = 0; i < nSheetChildCount; ++i)
	{
		vgui::VPANEL panel = vgui::ipanel()->GetChild(sheet, i);
		if (FStrEq(vgui::ipanel()->GetClassName(panel), "COptionsSubVideo"))
		{
			optionsSubVideo = panel;
			break;
		}
	}

	if (optionsSubVideo == 0)
		return vgui::INVALID_PANEL;

	return vgui::ivgui()->PanelToHandle(optionsSubVideo);
}

