//==============================================================================
//
// Purpose: Custom UI button
//
//==============================================================================

#include "cbase.h"
#include "customui_button.h"

#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>

#include "iclientmode.h"
#include "vgui_controls/AnimationController.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_BUILD_FACTORY(CCustomUIButton);

CCustomUIButton::CCustomUIButton(vgui::Panel* pParent, const char* pszPanelName) :
	m_pBackgroundPanel(NULL),
	m_pLabelText(NULL),
	m_bIsHighlighted(false),
	m_bIsPressed(false),
	m_pszCommand(NULL),
	m_bIsActive(false),
	BaseClass(pParent, pszPanelName)
{
	SetScheme(g_pVGuiSchemeManager->LoadSchemeFromFile("resource/UI/custom/CustomUIScheme.res", "CustomUIScheme"));
	LoadControlSettings("resource/UI/custom/CustomUIButton.res");
	SetProportional(true);

	// Disable mouse input on all children
	const int iChildCount = GetChildCount();
	for (int i = 0; i < iChildCount; i++)
	{
		Panel* pChild = GetChild(i);
		if (!pChild)
			continue;

		pChild->SetMouseInputEnabled(false);
	}

	m_pBackgroundPanel	= FindControl<vgui::Panel>("ButtonBackgroundPanel");
	m_pLabelText		= FindControl<vgui::Label>("ButtonTextLabel");

	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);
	SetConsoleStylePanel(true);
}


CCustomUIButton::~CCustomUIButton()
{
	if (m_pszCommand != NULL)
		delete[] m_pszCommand;
}


void CCustomUIButton::ApplySettings(KeyValues* inResourceData)
{
	BaseClass::ApplySettings(inResourceData);

	const char* pszLabel = inResourceData->GetString("label", NULL);
	if (pszLabel)
	{
		const wchar_t* pwcszLocalizedLabel = g_pVGuiLocalize->Find(pszLabel);

		if (pwcszLocalizedLabel != NULL)
			SetDialogVariable("label", pwcszLocalizedLabel);
		else
			SetDialogVariable("label", pszLabel);
	}

	// Get button command
	if (m_pszCommand != NULL)
		delete[] m_pszCommand;

	const char* pszCommand = inResourceData->GetString("command", NULL);
	if (pszCommand)
	{
		int len = V_strlen(pszCommand);
		char* pszBuffer = new char[len + 1];
		V_strcpy(pszBuffer, pszCommand);
		m_pszCommand = pszBuffer;
	}
}


void CCustomUIButton::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	if (m_pBackgroundPanel)
		m_pBackgroundPanel->SetBgColor(m_bIsActive ? m_colBackgroundActiveColor : (m_bIsHighlighted ? m_colBackgroundOverColor : m_colBackgroundOutColor));

	BaseClass::ApplySchemeSettings(pScheme);
}


void CCustomUIButton::PerformLayout()
{
	BaseClass::PerformLayout();

	int wide, tall;
	GetSize(wide, tall);

	if (m_pBackgroundPanel)
		m_pBackgroundPanel->SetSize(wide, tall);

	if (m_pLabelText)
		m_pLabelText->SetSize(wide, tall);
}


void CCustomUIButton::OnCursorEntered()
{
	NavigateTo();

	BaseClass::OnCursorEntered();
}


void CCustomUIButton::OnCursorExited()
{
	if (m_bIsPressed)
	{
		PostMessage(GetParent(), new KeyValues("UnlockNav"));
		SetPressed(false);
	}

	NavigateFrom();

	BaseClass::OnCursorExited();
}


void CCustomUIButton::OnMousePressed(vgui::MouseCode code)
{
	if (code == MOUSE_LEFT)
	{
		PostMessage(GetParent(), new KeyValues("LockNav"));

		vgui::surface()->PlaySound("ui/buttonclick.wav");

		SetPressed(true);
		return;
	}

	BaseClass::OnMousePressed(code);
}


void CCustomUIButton::OnMouseReleased(vgui::MouseCode code)
{
	if (code == MOUSE_LEFT && m_bIsPressed)
	{
		PostMessage(GetParent(), new KeyValues("UnlockNav"));

		vgui::surface()->PlaySound("ui/buttonclickrelease.wav");
		OnPressed();

		SetPressed(false);
		return;
	}

	BaseClass::OnMouseReleased(code);
}


void CCustomUIButton::OnKeyCodePressed(vgui::KeyCode code)
{
	bool handled = false;
	switch (GetBaseButtonCode(code))
	{
	case KEY_XBUTTON_UP:
	case KEY_XSTICK1_UP:
	case KEY_XSTICK2_UP:
	case KEY_UP:
	case KEY_XBUTTON_DOWN:
	case KEY_XSTICK1_DOWN:
	case KEY_XSTICK2_DOWN:
	case KEY_DOWN:
	case KEY_XBUTTON_LEFT:
	case KEY_XSTICK1_LEFT:
	case KEY_XSTICK2_LEFT:
	case KEY_LEFT:
	case KEY_XBUTTON_RIGHT:
	case KEY_XSTICK1_RIGHT:
	case KEY_XSTICK2_RIGHT:
	case KEY_RIGHT:
	case KEY_XBUTTON_B:
		handled = true;
		break;
	}

	if (!handled && !m_PassUnhandledInput)
		return;

	BaseClass::OnKeyCodePressed(code);
}


void CCustomUIButton::OnKeyCodeTyped(vgui::KeyCode code)
{
	if (code == KEY_ENTER)
	{
		vgui::surface()->PlaySound("ui/buttonclickrelease.wav");
		OnPressed();
		return;
	}

	static ConVarRef vgui_nav_lock("vgui_nav_lock");

	bool handled = false;
	switch (GetBaseButtonCode(code))
	{
	case KEY_XBUTTON_UP:
	case KEY_XSTICK1_UP:
	case KEY_XSTICK2_UP:
	case KEY_UP:
		if ((!vgui_nav_lock.IsValid() || vgui_nav_lock.GetInt() == 0) && NavigateUp())
		{
			vgui_nav_lock.SetValue(1);
			handled = true;
		}
		break;
	case KEY_XBUTTON_DOWN:
	case KEY_XSTICK1_DOWN:
	case KEY_XSTICK2_DOWN:
	case KEY_DOWN:
		if ((!vgui_nav_lock.IsValid() || vgui_nav_lock.GetInt() == 0) && NavigateDown())
		{
			vgui_nav_lock.SetValue(1);
			handled = true;
		}
		break;
	case KEY_XBUTTON_LEFT:
	case KEY_XSTICK1_LEFT:
	case KEY_XSTICK2_LEFT:
	case KEY_LEFT:
		if ((!vgui_nav_lock.IsValid() || vgui_nav_lock.GetInt() == 0) && NavigateLeft())
		{
			vgui_nav_lock.SetValue(1);
			handled = true;
		}
		break;
	case KEY_XBUTTON_RIGHT:
	case KEY_XSTICK1_RIGHT:
	case KEY_XSTICK2_RIGHT:
	case KEY_RIGHT:
		if ((!vgui_nav_lock.IsValid() || vgui_nav_lock.GetInt() == 0) && NavigateRight())
		{
			vgui_nav_lock.SetValue(1);
			handled = true;
		}
		break;
	case KEY_XBUTTON_B:
		if ((!vgui_nav_lock.IsValid() || vgui_nav_lock.GetInt() == 0) && NavigateBack())
		{
			vgui_nav_lock.SetValue(1);
			handled = true;
		}
		break;
	}

	if (!handled && !m_PassUnhandledInput)
		return;

	BaseClass::OnKeyCodeTyped(code);
}


void CCustomUIButton::OnThink()
{
	if (engine->IsPaused() || !engine->IsInGame())
		g_pClientMode->GetViewportAnimationController()->UpdateAnimations(gpGlobals->realtime);

	BaseClass::OnThink();
}


void CCustomUIButton::NavigateTo()
{
	vgui::surface()->PlaySound("ui/buttonrollover.wav");
	Highlight();
	RequestFocus(0);
	BaseClass::NavigateTo();
}


void CCustomUIButton::NavigateFrom()
{
	UnHighlight();
	BaseClass::NavigateFrom();
}


void CCustomUIButton::SetActive(bool bActive)
{
	if (bActive == m_bIsActive)
		return;

	m_bIsActive = bActive;

	if (m_pBackgroundPanel != NULL)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pBackgroundPanel,
			"BgColor",
			m_bIsActive ? m_colBackgroundActiveColor : (m_bIsHighlighted ? m_colBackgroundOverColor : m_colBackgroundOutColor),
			0.0f,
			m_flPressAnimationDuration,
			vgui::AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}
}


void CCustomUIButton::Press()
{
	if (m_pBackgroundPanel != NULL)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pBackgroundPanel,
			"BgColor",
			m_colBackgroundPressedColor,
			0.0f,
			m_flPressAnimationDuration,
			vgui::AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}

	if (m_pLabelText != NULL)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pLabelText,
			"FgColor",
			m_colTextPressedColor,
			0.0f,
			m_flPressAnimationDuration,
			vgui::AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}
}


void CCustomUIButton::UnPress()
{
	if (m_pBackgroundPanel != NULL)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pBackgroundPanel,
			"BgColor",
			m_bIsActive ? m_colBackgroundActiveColor : m_colBackgroundOverColor,
			0.0f,
			m_flUnpressAnimationDuration,
			vgui::AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}

	if (m_pLabelText != NULL)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pLabelText,
			"FgColor",
			m_bIsActive ? m_colTextActiveColor : m_colTextOverColor,
			0.0f,
			m_flUnpressAnimationDuration,
			vgui::AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}
}


void CCustomUIButton::OnPressed()
{
	RequestFocus(0);
	RunCommand();
}


void CCustomUIButton::Highlight()
{
	if (m_bIsHighlighted)
		return;

	m_bIsHighlighted = true;

	if (m_pBackgroundPanel != NULL)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pBackgroundPanel,
			"BgColor",
			m_bIsActive ? m_colBackgroundActiveColor : m_colBackgroundOverColor,
			0.0f,
			m_flSelectAnimationDuration,
			vgui::AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}

	if (m_pLabelText != NULL)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pLabelText,
			"FgColor",
			m_bIsActive ? m_colTextActiveColor : m_colTextOverColor,
			0.0f,
			m_flSelectAnimationDuration,
			vgui::AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}
}


void CCustomUIButton::UnHighlight()
{
	if (!m_bIsHighlighted)
		return;

	m_bIsHighlighted = false;

	if (m_pBackgroundPanel != NULL)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pBackgroundPanel,
			"BgColor",
			m_bIsActive ? m_colBackgroundActiveColor : m_colBackgroundOutColor,
			0.0f,
			m_flDeselectAnimationDuration,
			vgui::AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}

	if (m_pLabelText != NULL)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pLabelText,
			"FgColor",
			m_bIsActive ? m_colTextActiveColor : m_colTextOutColor,
			0.0f,
			m_flDeselectAnimationDuration,
			vgui::AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}
}


void CCustomUIButton::RunCommand()
{	
	if (m_pszCommand == NULL)
		return;

	// Run this button's client command
	engine->ClientCmd(m_pszCommand);
}


void CCustomUIButton::SetPressed(bool bPressed)
{
	if (bPressed == m_bIsPressed)
		return;

	m_bIsPressed = bPressed;

	if (m_bIsPressed)
	{
		Press();
		return;
	}

	UnPress();
}

