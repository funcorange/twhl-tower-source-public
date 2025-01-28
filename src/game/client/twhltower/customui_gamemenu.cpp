//==============================================================================
//
// Purpose: Custom UI game menu
//
//==============================================================================

#include "cbase.h"
#include "customui_gamemenu.h"

#include <vgui/IPanel.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui/MouseCode.h>

#include "GameUI/IGameUI.h"
#include "iclientmode.h"
#include "icommandline.h"
#include "ioverrideinterface.h"
#include "vgui_controls/AnimationController.h"
#include "vgui_controls/EditablePanel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

static CCustomUIGameMenu* g_pGameMenu = NULL;

//------------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------------
CCustomUIGameMenu::CCustomUIGameMenu(Panel* pParent) :
	m_bIsInGame(false),
	m_bIsNavLocked(false),
	BaseClass(pParent, "CustomUIGameMenu")
{
	g_pGameMenu = this;

	SetProportional(true);

	m_bIsInGame = engine->IsInGame();

	LoadControlSettings("resource/UI/custom/CustomUIGameMenu.res");

	m_pImagePanelLogo = FindControl<ImagePanel>("Logo");

	const int iChildCount = GetChildCount();
	for (int i = 0; i < iChildCount; i++)
	{
		Panel* pPanel = GetChild(i);
		if (!pPanel)
			continue;

		CCustomUIMenuButton* pButton = dynamic_cast<CCustomUIMenuButton*>(pPanel);
		if (!pButton)
		{
			pPanel->SetMouseInputEnabled(false);
			continue;
		}

		m_Buttons.InsertNoSort(pButton);
	}
	m_Buttons.RedoSort(true);

	SetKeyBoardInputEnabled(true);

	MakePopup();

	if (engine->Con_IsVisible())
		surface()->MovePopupToBack(GetVPanel());
}


CCustomUIGameMenu::~CCustomUIGameMenu()
{
}


//------------------------------------------------------------------------------
// Purpose: Set size to full screen
//------------------------------------------------------------------------------
void CCustomUIGameMenu::PerformLayout()
{
	BaseClass::PerformLayout();

	if (m_pImagePanelLogo)
	{
		int xpos = scheme()->GetProportionalScaledValue(atoi(m_pScheme->GetResourceString("Logo.OffsetX")));
		int ypos = scheme()->GetProportionalScaledValue(atoi(m_pScheme->GetResourceString((engine->IsInGame() && !engine->IsLevelMainMenuBackground()) ? "Logo.OffsetY.InGame" : "Logo.OffsetY.MainMenu")));
		m_pImagePanelLogo->SetPos(xpos, ypos);
	}

	//int wide, tall;
	//vgui::surface()->GetScreenSize(wide, tall);
	//SetSize(wide, tall);

	UpdateButtons();
}


void CCustomUIGameMenu::OnMousePressed(vgui::MouseCode code)
{
	MoveToFront();
	RequestFocus();

	BaseClass::OnMousePressed(code);
}


void CCustomUIGameMenu::OnNavigateTo(const char* pszPanelName)
{
	if (m_bIsNavLocked)
		return;

	CCustomUIMenuButton* pButton = FindControl<CCustomUIMenuButton>(pszPanelName);
	if (!pButton)
		return;

	HighlightButton(pButton);
	pButton->RequestFocus();
}


void CCustomUIGameMenu::ApplySchemeSettings(IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	m_pScheme = pScheme;
}


void CCustomUIGameMenu::OnThink()
{
	if ((engine->IsInGame() && !engine->IsLevelMainMenuBackground()) != m_bIsInGame)
	{
		m_bIsInGame = (engine->IsInGame() && !engine->IsLevelMainMenuBackground());
		InvalidateLayout(false, true);
	}
}


void CCustomUIGameMenu::OnLockNav()
{
	m_bIsNavLocked = true;
}


void CCustomUIGameMenu::OnUnlockNav()
{
	m_bIsNavLocked = false;
}


//------------------------------------------------------------------------------
// Purpose: Set up the layout for menu buttons
//------------------------------------------------------------------------------
void CCustomUIGameMenu::UpdateButtons()
{
	// Construct list of all active buttons
	CUtlVector<CCustomUIMenuButton*> activeButtons;
	const int iButtonCount = m_Buttons.Count();
	for (int i = 0; i < iButtonCount; i++)
	{
		CCustomUIMenuButton* pButton = m_Buttons.Element(i);

		if (((!engine->IsInGame() || engine->IsLevelMainMenuBackground()) && pButton->IsOnlyInGame()) || (developer.GetInt() == 0 && pButton->IsDeveloperOnly()))
		{
			pButton->SetVisible(false);
			continue;
		}

		activeButtons.AddToTail(pButton);
	}

	// Get button positions from scheme
	const int posx = scheme()->GetProportionalScaledValue(GetButtonsOffsetX());
	const int iButtonsOffsetY = GetButtonsOffsetY();
	const int iButtonHeight = atoi(m_pScheme->GetResourceString("Button.Height"));
	const int iButtonSpacing = atoi(m_pScheme->GetResourceString("Buttons.Space"));

	// Set up visible buttons for navigation
	const int iActiveButtonCount = activeButtons.Count();
	for (int i = 0; i < iActiveButtonCount; i++)
	{
		CCustomUIMenuButton* pButton = activeButtons.Element(i);

		// Set navigation targets
		if (i > 0)
			pButton->SetNavUp(activeButtons.Element(i - 1));

		if (i < iActiveButtonCount - 1)
			pButton->SetNavDown(activeButtons.Element(i + 1));

		// Hard-coded values for now
		const int ypos = scheme()->GetProportionalScaledValue(iButtonsOffsetY + (iButtonHeight + iButtonSpacing) * i);
		pButton->SetPos(posx, ypos);

		pButton->SetVisible(true);
	}

	if (activeButtons.Count() > 0)
		activeButtons.Head()->NavigateTo();
}


int CCustomUIGameMenu::GetButtonsOffsetX() const
{
	const char* const pszResourceString = m_pScheme->GetResourceString("Buttons.OffsetX");
	if (!pszResourceString)
		return 0;

	return atoi(pszResourceString);
}


int CCustomUIGameMenu::GetButtonsOffsetY() const
{
	const char* const pszResourceString = m_pScheme->GetResourceString((engine->IsInGame() && !engine->IsLevelMainMenuBackground()) ? "Buttons.OffsetY.InGame" : "Buttons.OffsetY.MainMenu");
	if (!pszResourceString)
		return 0;

	return atoi(pszResourceString);
}


void CCustomUIGameMenu::HighlightButton(CCustomUIMenuButton* pButton)
{
	const int iChildCount = GetChildCount();
	for (int i = 0; i < iChildCount; i++)
	{
		CCustomUIMenuButton* pCurrentButton = dynamic_cast<CCustomUIMenuButton*>(GetChild(i));
		if (!pCurrentButton)
			continue;

		if (pCurrentButton == pButton)
		{
			pCurrentButton->RequestFocus();
			pCurrentButton->SetHighlighted(true);
			continue;
		}

		pCurrentButton->SetHighlighted(false);
	}
}


CON_COMMAND_F(customui_show_gamemenu, "Set the main game menu to the current menu", FCVAR_CLIENTCMD_CAN_EXECUTE)
{
	if (!g_pGameMenu)
	{
		DevMsg("No GameMenu\n");
		return;
	}

	g_pOverrideUI->SetCurrentMenu(g_pGameMenu);
}

