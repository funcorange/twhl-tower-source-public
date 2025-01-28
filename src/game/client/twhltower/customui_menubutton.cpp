//==============================================================================
//
// Purpose: Custom UI game menu button
//
//==============================================================================

#include "cbase.h"
#include "customui_menubutton.h"

#include <vgui/ILocalize.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>

#include "gamestringpool.h"
#include "GameUI/IGameUI.h"
#include "iclientmode.h"
#include "ioverrideinterface.h"
#include "KeyValues.h"
#include "vgui_controls/AnimationController.h"
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/Panel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

DECLARE_BUILD_FACTORY(CCustomUIMenuButton);

CCustomUIMenuButton::CCustomUIMenuButton(Panel* pParent, const char* pszPanelName) :
	m_iInGameOrder(0),
	m_bOnlyInGame(false),
	m_bIsHighlighted(false),
	m_bIsPressed(false),
	EditablePanel(pParent, pszPanelName)
{
	SetProportional(true);
	SetScheme(g_pVGuiSchemeManager->LoadSchemeFromFile("resource/UI/custom/CustomUIScheme.res", "CustomUIScheme"));
	LoadControlSettings("resource/UI/custom/CustomUIMenuButton.res");

	// Disable mouse input on all children
	const int iChildCount = GetChildCount();
	for (int i = 0; i < iChildCount; i++)
	{
		Panel* pChild = GetChild(i);
		if (!pChild)
			continue;

		pChild->SetMouseInputEnabled(false);
	}

	m_pPanelBackground	= FindControl<Panel>("MenuButtonBackgroundPanel");
	m_pPanelLeftBorder	= FindControl<Panel>("MenuButtonSidebarPanel");
	m_pLabelText		= FindControl<Label>("MenuButtonTextLabel");

	SetKeyBoardInputEnabled(true);
	SetConsoleStylePanel(true);
}


void CCustomUIMenuButton::ApplySettings(KeyValues* inResourceData)
{
	BaseClass::ApplySettings(inResourceData);

	const char* pszLabel = inResourceData->GetString("label", NULL);
	if (pszLabel)
		SetDialogVariable("label", g_pVGuiLocalize->Find(pszLabel));

	const char* pszCommand = inResourceData->GetString("command", NULL);
	if (pszCommand)
		V_strncpy(m_szCommand, pszCommand, sizeof(m_szCommand));

	m_iInGameOrder = inResourceData->GetInt("InGameOrder");
	m_bOnlyInGame = inResourceData->GetBool("OnlyInGame");
	m_bIsDeveloperOnly = inResourceData->GetBool("DeveloperOnly");
}


void CCustomUIMenuButton::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	m_pScheme = pScheme;

	m_iLeftBorderWidthOut			= scheme()->GetProportionalScaledValue(atoi(pScheme->GetResourceString("Button.LeftBorder.Out")));
	m_iLeftBorderWidthOver			= scheme()->GetProportionalScaledValue(atoi(pScheme->GetResourceString("Button.LeftBorder.Over")));
	m_iLeftBorderWidthPressed		= scheme()->GetProportionalScaledValue(atoi(pScheme->GetResourceString("Button.LeftBorder.Pressed")));

	m_flSelectAnimationDuration		= atof(pScheme->GetResourceString("Button.Animation.Select.Duration"));
	m_flDeselectAnimationDuration	= atof(pScheme->GetResourceString("Button.Animation.Deselect.Duration"));
	m_flPressAnimationDuration		= atof(pScheme->GetResourceString("Button.Animation.Press.Duration"));
	m_flUnpressAnimationDuration	= atof(pScheme->GetResourceString("Button.Animation.Unpress.Duration"));

	m_colBackgroundOutColor		= pScheme->GetColor("Button.Background.Out", Color(0, 0, 0, 0));
	m_colBackgroundOverColor	= pScheme->GetColor("Button.Background.Over", Color(25, 16, 0, 120));
	m_colBackgroundPressedColor	= pScheme->GetColor("Button.Background.Pressed", Color(255, 255, 255, 255));

	m_colLeftBorderOutColor = pScheme->GetColor("Button.LeftBorder.Out", Color(0, 0, 0, 0));
	m_colLeftBorderOverColor = pScheme->GetColor("Button.LeftBorder.Over", Color(255, 160, 0, 255));
	m_colLeftBorderPressedColor = pScheme->GetColor("Button.LeftBorder.Pressed", Color(255, 255, 255, 255));

	m_colTextOutColor = pScheme->GetColor("Button.Text.Out", Color(224, 224, 224, 255));
	m_colTextOverColor = pScheme->GetColor("Button.Text.Over", Color(255, 160, 0, 255));
	m_colTextPressedColor = pScheme->GetColor("Button.Text.Pressed", Color(0, 0, 0, 255));

	if (m_pPanelBackground)
		m_pPanelBackground->SetBgColor(m_bIsHighlighted ? m_colBackgroundOverColor : m_colBackgroundOutColor);
	
	if (m_pPanelLeftBorder)
	{
		m_pPanelLeftBorder->SetBgColor(m_bIsHighlighted ? m_colLeftBorderOverColor : m_colLeftBorderOutColor);
		m_pPanelLeftBorder->SetWide(m_bIsHighlighted ? m_iLeftBorderWidthOver : m_iLeftBorderWidthOut);
	}
	
	if (m_pLabelText)
		m_pLabelText->SetFgColor(m_bIsHighlighted ? m_colTextOverColor : m_colTextOutColor);

	BaseClass::ApplySchemeSettings(pScheme);
}


void CCustomUIMenuButton::PerformLayout()
{
	BaseClass::PerformLayout();

	int wide = scheme()->GetProportionalScaledValue(atoi(m_pScheme->GetResourceString("Button.Width")));
	int tall = scheme()->GetProportionalScaledValue(atoi(m_pScheme->GetResourceString("Button.Height")));
	SetSize(wide, tall);
}


void CCustomUIMenuButton::OnThink()
{
	if (engine->IsPaused() || !engine->IsInGame())
	{
		if (g_pClientMode &&
			g_pClientMode->GetViewport() &&
			g_pClientMode->GetViewportAnimationController()
		)
			g_pClientMode->GetViewportAnimationController()->UpdateAnimations(
				gpGlobals->realtime
			);
	}

	BaseClass::OnThink();
}


void CCustomUIMenuButton::OnCursorEntered()
{
	NavigateTo();

	BaseClass::OnCursorEntered();
}


void CCustomUIMenuButton::OnCursorExited()
{
	if (m_bIsPressed)
	{
		PostMessage(GetParent(), new KeyValues("UnlockNav"));
		SetPressed(false);
	}

	BaseClass::OnCursorExited();
}


void CCustomUIMenuButton::OnMousePressed(MouseCode code)
{
	if (code == MOUSE_LEFT)
	{
		PostMessage(GetParent(), new KeyValues("LockNav"));

		surface()->PlaySound("ui/buttonclick.wav");

		SetPressed(true);
		return;
	}

	BaseClass::OnMousePressed(code);
}


void CCustomUIMenuButton::OnMouseReleased(MouseCode code)
{
	if (code == MOUSE_LEFT && m_bIsPressed)
	{
		PostMessage(GetParent(), new KeyValues("UnlockNav"));

		surface()->PlaySound("ui/buttonclickrelease.wav");
		RunCommand();

		SetPressed(false);
		return;
	}

	BaseClass::OnMouseReleased(code);
}


void CCustomUIMenuButton::OnKeyCodePressed(MouseCode code)
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

	BaseClass::OnKeyCodeTyped(code);
}


void CCustomUIMenuButton::OnKeyCodeTyped(MouseCode code)
{
	if (code == KEY_ENTER)
	{
		surface()->PlaySound("ui/buttonclickrelease.wav");
		RunCommand();
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


void CCustomUIMenuButton::NavigateTo()
{
	surface()->PlaySound("ui/buttonrollover.wav");

	BaseClass::NavigateTo();
}


void CCustomUIMenuButton::NavigateFrom()
{
	//SetHighlighted(false);

	BaseClass::NavigateFrom();
}


void CCustomUIMenuButton::SetPressed(bool bPressed)
{
	if (bPressed == m_bIsPressed)
		return;

	m_bIsPressed = bPressed;

	if (m_bIsPressed)
	{
		Press();
		return;
	}

	Unpress();
}


void CCustomUIMenuButton::SetHighlighted(bool bHighlighted)
{
	if (bHighlighted == m_bIsHighlighted)
		return;

	m_bIsHighlighted = bHighlighted;

	if (m_bIsHighlighted)
	{
		Highlight();
		return;
	}

	Unhighlight();
}


void CCustomUIMenuButton::RunCommand()
{
	// Run this button's menu command
	IGameUI* pGameUI = g_pOverrideUI->GetGameUI();
	if (!pGameUI)
	{
		DevMsg("CCustomUIMenuButton::OnMousePressed, No GameUI\n");
		return;
	}

	engine->ClientCmd(m_szCommand);
}


void CCustomUIMenuButton::Press()
{
	if (m_pPanelBackground)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pPanelBackground,
			"BgColor",
			m_colBackgroundPressedColor,
			0.0f,
			m_flPressAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}

	if (m_pLabelText)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pLabelText,
			"FgColor",
			m_colTextPressedColor,
			0.0f,
			m_flPressAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}

	if (m_pPanelLeftBorder)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pPanelLeftBorder,
			"BgColor",
			m_colLeftBorderPressedColor,
			0.0f,
			m_flPressAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);

		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pPanelLeftBorder,
			"Wide",
			m_iLeftBorderWidthPressed,
			0.0f,
			m_flPressAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}
}


void CCustomUIMenuButton::Unpress()
{
	if (m_pPanelBackground)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pPanelBackground,
			"BgColor",
			m_colBackgroundOverColor,
			0.0f,
			m_flUnpressAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}

	if (m_pLabelText)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pLabelText,
			"FgColor",
			m_colTextOverColor,
			0.0f,
			m_flUnpressAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}

	if (m_pPanelLeftBorder)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pPanelLeftBorder,
			"BgColor",
			m_colLeftBorderOverColor,
			0.0f,
			m_flUnpressAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);

		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pPanelLeftBorder,
			"Wide",
			m_iLeftBorderWidthOver,
			0.0f,
			m_flUnpressAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}
}


void CCustomUIMenuButton::Highlight()
{
	if (m_pPanelBackground)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pPanelBackground,
			"BgColor",
			m_colBackgroundOverColor,
			0.0f,
			m_flSelectAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}
	
	if (m_pLabelText)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pLabelText,
			"FgColor",
			m_colTextOverColor,
			0.0f,
			m_flSelectAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}
	
	if (m_pPanelLeftBorder)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pPanelLeftBorder,
			"BgColor",
			m_colLeftBorderOverColor,
			0.0f,
			m_flSelectAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pPanelLeftBorder,
			"Wide",
			m_iLeftBorderWidthOver,
			0.0f,
			m_flSelectAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}
}


void CCustomUIMenuButton::Unhighlight()
{
	if (m_pPanelBackground)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pPanelBackground,
			"BgColor",
			m_colBackgroundOutColor,
			0.0f,
			m_flDeselectAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}

	if (m_pLabelText)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pLabelText,
			"FgColor",
			m_colTextOutColor,
			0.0f,
			m_flDeselectAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}

	if (m_pPanelLeftBorder)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pPanelLeftBorder,
			"BgColor",
			m_colLeftBorderOutColor,
			0.0f,
			m_flDeselectAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);

		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pPanelLeftBorder,
			"Wide",
			m_iLeftBorderWidthOut,
			0.0f,
			m_flDeselectAnimationDuration,
			AnimationController::Interpolators_e::INTERPOLATOR_LINEAR
		);
	}
}

