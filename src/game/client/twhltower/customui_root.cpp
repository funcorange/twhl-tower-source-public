//==============================================================================
//
// Purpose: Custom UI override root panel
//
//==============================================================================

#include "cbase.h"
#include "customui_root.h"

#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/VGUI.h>

#include "customui_gamemenu.h"
#include "customui_newgame.h"
#include "GameUI/IGameUI.h"
#include "iclientmode.h"
#include "interface.h"
#include "ioverrideinterface.h"
#include "vgui_controls/AnimationController.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static CDllDemandLoader g_GameUIDLL("GameUI");
CCustomUIRoot* guiroot = NULL;

void OverrideGameUI()
{
	if (!g_pOverrideUI->GetPanel())
		g_pOverrideUI->Create(NULL);

	IGameUI* pGameUI = guiroot->GetGameUI();
	if (!pGameUI)
		return;

	pGameUI->SetMainMenuOverride(guiroot->GetVPanel());
}


//==============================================================================
//
// CCustomUIRoot
//
//==============================================================================
using namespace vgui;

//------------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------------
CCustomUIRoot::CCustomUIRoot(vgui::VPANEL parent) :
	m_iExitingFrameCount(0),
	m_bCopyFrameBuffer(false),
	m_pGameUI(NULL),
	m_pElements(),
	vgui::EditablePanel(NULL, "CustomUIRoot")
{
	SetParent(parent);
	guiroot = this;

	SetSize(10, 10);

	// Create custom UI elements
	CCustomUIGameMenu* pGameMenu	= new CCustomUIGameMenu(this);
	CCustomUINewGame* pNewGame		= new CCustomUINewGame(this);

	pGameMenu->SetProportional(true);
	pNewGame->SetProportional(true);

	m_pElements.AddToTail(pGameMenu);
	m_pElements.AddToTail(pNewGame);

	SetScheme(g_pVGuiSchemeManager->LoadSchemeFromFile("resource/UI/custom/CustomUIScheme.res", "CustomUIScheme"));
	LoadControlSettings("resource/UI/custom/CustomUIRoot.res");

	LoadGameUI();

	SetKeyBoardInputEnabled(true);

	SetCurrentMenu(pGameMenu);
}


//------------------------------------------------------------------------------
// Purpose: Destructor
//------------------------------------------------------------------------------
CCustomUIRoot::~CCustomUIRoot()
{
	m_pGameUI = NULL;
	g_GameUIDLL.Unload();
}


//------------------------------------------------------------------------------
// Purpose: Returns a pointer to the GameUI engine interface
//------------------------------------------------------------------------------
IGameUI* CCustomUIRoot::GetGameUI()
{
	if (!m_pGameUI && !LoadGameUI())
		return NULL;

	return m_pGameUI;
}


void CCustomUIRoot::SetCurrentMenu(vgui::Panel* pPanel)
{
	if (pPanel)
		DevMsg("Setting current menu to %s\n", pPanel->GetName());

	for (int i = 0; i < m_pElements.Count(); i++)
	{
		vgui::Panel* pCurrent = m_pElements.Element(i);
		if (!pCurrent)
			continue;

		bool bIsCurrent = (pCurrent == pPanel);
		pCurrent->SetVisible(bIsCurrent);
		pCurrent->SetEnabled(bIsCurrent);
	}
}


//------------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------------
void CCustomUIRoot::PerformLayout()
{
	BaseClass::PerformLayout();

	int wide, tall;
	vgui::surface()->GetScreenSize(wide, tall);
	SetSize(wide, tall);
}


//------------------------------------------------------------------------------
// Purpose: Loads the GameUI engine interface
//------------------------------------------------------------------------------
bool CCustomUIRoot::LoadGameUI()
{
	if (m_pGameUI)
		return true;

	CreateInterfaceFn gameUIFactory = g_GameUIDLL.GetFactory();
	if (!gameUIFactory)
		return false;

	m_pGameUI = static_cast<IGameUI*>(gameUIFactory(GAMEUI_INTERFACE_VERSION, NULL));
	if (!m_pGameUI)
		return false;

	return true;
}


CON_COMMAND_F(customui_hide, "Hide the custom UI", FCVAR_CLIENTCMD_CAN_EXECUTE)
{
	if (!guiroot)
		return;

	guiroot->SetCurrentMenu(NULL);
}

