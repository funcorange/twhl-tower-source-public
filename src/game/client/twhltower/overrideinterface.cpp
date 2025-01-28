//==============================================================================
//
// Purpose: Custom UI override creation interface implementation
//
//==============================================================================

#include "cbase.h"
#include "GameUI/IGameUI.h"
#include "overrideinterface.h"

#include <vgui/VGUI.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static COverrideInterface g_OverrideInterface;
IOverrideInterface* g_pOverrideUI = static_cast<IOverrideInterface*>(&g_OverrideInterface);

//------------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------------
COverrideInterface::COverrideInterface() :
	m_pMainMenu(NULL)
{
}


//------------------------------------------------------------------------------
// Purpose: Create UI override panel
//------------------------------------------------------------------------------
void COverrideInterface::Create(vgui::VPANEL parent)
{
	m_pMainMenu = new CCustomUIRoot(parent);
}


//------------------------------------------------------------------------------
// Purpose: Get UI override VPanel
//------------------------------------------------------------------------------
vgui::VPANEL COverrideInterface::GetPanel()
{
	if (!m_pMainMenu)
		return NULL;

	return m_pMainMenu->GetVPanel();
}


//------------------------------------------------------------------------------
// Purpose: Destroy UI override panel
//------------------------------------------------------------------------------
void COverrideInterface::Destroy()
{
	if (!m_pMainMenu)
		return;

	m_pMainMenu->SetParent(static_cast<vgui::VPANEL>(NULL));
	delete m_pMainMenu;
}


IGameUI* COverrideInterface::GetGameUI()
{
	if (!m_pMainMenu)
		return NULL;

	return m_pMainMenu->GetGameUI();
}


void COverrideInterface::SetCurrentMenu(vgui::Panel* pPanel)
{
	if (!m_pMainMenu)
		return;

	m_pMainMenu->SetCurrentMenu(pPanel);
}

