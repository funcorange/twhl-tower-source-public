//=============================================================================
//
// Purpose: Counter HUD element
//
//=============================================================================

#include "cbase.h"
#include "hud_counter.h"

#include <vgui/ILocalize.h>
#include <vgui/IVGui.h>

#include "gamestringpool.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "iclientmode.h"
#include "vgui_controls/AnimationController.h"
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/Label.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//=============================================================================
// 
// CHudCounter
// 
//=============================================================================
DECLARE_HUDELEMENT(CHudCounter);
DECLARE_HUD_MESSAGE(CHudCounter, HudCounter);

BEGIN_DATADESC(CHudCounter)

	DEFINE_FIELD(m_bShouldDraw, FIELD_BOOLEAN),
	DEFINE_FIELD(m_bHiding, FIELD_BOOLEAN),
	DEFINE_FIELD(m_iMaxValue, FIELD_INTEGER),
	DEFINE_FIELD(m_iValue, FIELD_INTEGER),
	DEFINE_FIELD(m_iszTitle, FIELD_STRING),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudCounter::CHudCounter(const char* pszElementName) :
	m_bShouldDraw(false),
	m_bHiding(false),
	m_flHideTime(0.0F),
	m_pBackgroundPanel(NULL),
	m_pTitleLabel(NULL),
	m_pCountLabel(NULL),
	m_pCountGlowLabel(NULL),
	m_iszTitle(),
	BaseClass(pszElementName),
	vgui::EditablePanel(g_pClientMode->GetViewport(), "HudCounter")
{
	vgui::ivgui()->AddTickSignal(GetVPanel());
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CHudCounter::~CHudCounter()
{
}


//-----------------------------------------------------------------------------
// Purpose: Apply scheme settings
//-----------------------------------------------------------------------------
void CHudCounter::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	vgui::EditablePanel::ApplySchemeSettings(pScheme);

	LoadControlSettings("resource/UI/hudcounter.res");
	
	m_pBackgroundPanel = FindControl<vgui::Panel>("CounterBackgroundPanel");
	m_pTitleLabel = FindControl<vgui::Label>("CounterTitleLabel");
	m_pCountLabel = FindControl<vgui::Label>("CounterCountLabel");
	m_pCountGlowLabel = FindControl<vgui::Label>("CounterCountGlowLabel");
}


//-----------------------------------------------------------------------------
// Purpose: Initialize HUD element
//-----------------------------------------------------------------------------
void CHudCounter::Init()
{
	HOOK_HUD_MESSAGE(CHudCounter, HudCounter);
}


//-----------------------------------------------------------------------------
// Purpose: Called after loading a save
//-----------------------------------------------------------------------------
void CHudCounter::OnRestore()
{
	if (m_pCountGlowLabel)
	{
		m_pCountGlowLabel->SetAlpha(0);
	}

	if (m_bHiding)
	{
		m_bHiding = false;
		m_bShouldDraw = false;
	}

	if (m_bShouldDraw)
	{
		if (m_pBackgroundPanel)
			m_pBackgroundPanel->SetAlpha(255);

		if (m_pTitleLabel)
			m_pTitleLabel->SetAlpha(255);

		if (m_pCountLabel)
			m_pCountLabel->SetAlpha(255);
	}

	SetTitle(STRING(m_iszTitle));
	SetMaxValue(m_iMaxValue);
	SetValue(m_iValue, false);

	InvalidateLayout();
}


//-----------------------------------------------------------------------------
// Purpose: Called every tick
//-----------------------------------------------------------------------------
void CHudCounter::OnTick()
{	
	if (!m_bShouldDraw)
		return;

	if (m_bHiding && gpGlobals->curtime >= m_flHideTime)
	{
		m_bHiding		= false;
		m_bShouldDraw	= false;
	}
}


//-----------------------------------------------------------------------------
// Purpose: Reset HUD element
//-----------------------------------------------------------------------------
void CHudCounter::Reset()
{
	m_bShouldDraw = false;
}


//-----------------------------------------------------------------------------
// Purpose: Perform layout
//-----------------------------------------------------------------------------
void CHudCounter::PerformLayout()
{
	// Set title string
	const wchar_t* pwcszTitleString = NULL;

	if (m_iszTitle == NULL_STRING)
	{
		pwcszTitleString = L"UNKNOWN";
		SetDialogVariable("title", pwcszTitleString);
	}
	else
	{
		const wchar_t* pwcszTitleToken =
			g_pVGuiLocalize->Find(STRING(m_iszTitle));

		if (pwcszTitleToken)
		{
			pwcszTitleString = pwcszTitleToken;
			SetDialogVariable("title", pwcszTitleString);
		}
		else
		{
			SetDialogVariable("title", STRING(m_iszTitle));
		}
	}

	int iTitleWidth = 0;
	if (m_pTitleLabel)
	{
		vgui::HFont hTitleFont = m_pTitleLabel->GetFont();
		if (pwcszTitleString)
		{
			iTitleWidth = UTIL_ComputeStringWidth(
				hTitleFont,
				pwcszTitleString
			);
		}
		else
		{
			iTitleWidth = UTIL_ComputeStringWidth(
				hTitleFont,
				STRING(m_iszTitle)
			);
		}
		m_pTitleLabel->SetWide(iTitleWidth);
	}

	// Set count string
	const wchar_t* pwcszFormatToken =
		g_pVGuiLocalize->Find("#twhltower_Counter_Fmt");

	if (!pwcszFormatToken)
	{
		pwcszFormatToken = L"%s1 / %s2";
	}

	wchar_t wcszMaxValueString[16] = {};
	V_snwprintf(
		wcszMaxValueString,
		Q_ARRAYSIZE(wcszMaxValueString),
		L"%i",
		m_iMaxValue
	);

	int iCountWidth = 0;
	if (m_pCountLabel)
	{
		int countX, countY;
		m_pCountLabel->GetPos(countX, countY);

		m_pCountLabel->SetPos(
			vgui::scheme()->GetProportionalScaledValue(24) +
			iTitleWidth,
			countY
		);

		vgui::HFont hCountFont = m_pCountLabel->GetFont();

		// Find widest count string
		for (int i = 0; i <= m_iMaxValue; ++i)
		{
			wchar_t wcszValue[12] = {0};
			V_snwprintf(wcszValue, Q_ARRAYSIZE(wcszValue), L"%i", i);
			
			wchar_t wcszFormattedCount[32];

			g_pVGuiLocalize->ConstructString(
				wcszFormattedCount,
				sizeof(wcszFormattedCount),
				pwcszFormatToken,
				2,
				wcszValue,
				wcszMaxValueString
			);

			const int iWidth = UTIL_ComputeStringWidth(
				hCountFont,
				wcszFormattedCount
			);

			if (iWidth > iCountWidth)
				iCountWidth = iWidth;
		}

		m_pCountLabel->SetWide(iCountWidth +
			vgui::scheme()->GetProportionalScaledValue(8)
		);
	}

	wchar_t wcszValueString[16] = {};
	V_snwprintf(
		wcszValueString,
		Q_ARRAYSIZE(wcszValueString),
		L"%i",
		m_iValue
	);

	wchar_t wcszCountString[32];
	g_pVGuiLocalize->ConstructString(
		wcszCountString,
		sizeof(wcszCountString),
		pwcszFormatToken,
		2,
		wcszValueString,
		wcszMaxValueString
	);

	SetDialogVariable("count", wcszCountString);

	// Set count glow label layout
	if (m_pCountGlowLabel)
	{
		int countX, countY;
		m_pCountGlowLabel->GetPos(countX, countY);

		m_pCountGlowLabel->SetPos(
			vgui::scheme()->GetProportionalScaledValue(24) +
			iTitleWidth,
			countY
		);

		m_pCountGlowLabel->SetWide(iCountWidth +
			vgui::scheme()->GetProportionalScaledValue(8)
		);
	}

	// Set HUD element layout
	SetWide(
		vgui::scheme()->GetProportionalScaledValue(32) +
		iTitleWidth + iCountWidth
	);

	// Set background panel layout
	if (m_pBackgroundPanel)
	{
		m_pBackgroundPanel->SetWide(GetWide());
	}
}


//-----------------------------------------------------------------------------
// Purpose: Determine whether to draw this HUD element
// Output:	True if this HUD element should be drawn, false if it should not.
//-----------------------------------------------------------------------------
bool CHudCounter::ShouldDraw()
{
	return m_bShouldDraw;
}


//-----------------------------------------------------------------------------
// Purpose: HudCounter message handler
// Input:   msg - Message buffer
//-----------------------------------------------------------------------------
void CHudCounter::MsgFunc_HudCounter(bf_read& msg)
{
	const int nType = msg.ReadByte();	// Message type

	switch (nType)
	{
		case 0:
		{
			// Show
			m_iMaxValue	= msg.ReadLong();
			m_iValue	= msg.ReadLong();

			char szTitle[256] = {0};
			msg.ReadString(szTitle, sizeof(szTitle));
			SetTitle(szTitle);

			Show();
			break;
		}

		case 1:
		{
			// Hide
			Hide();
			break;
		}

		case 2:
		{
			// Set value
			SetValue(msg.ReadLong());
			break;
		}

		default:
		{
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: Calculate the width of this HUD element
//-----------------------------------------------------------------------------
int CHudCounter::CalculateWidth()
{
	KeyValues* pkvDialogVariables = GetDialogVariables();
	const wchar_t* pwszTitleString = pkvDialogVariables->GetWString("title");
	const wchar_t* pwszCountString = pkvDialogVariables->GetWString("count");

	int iTitleWidth = 0;
	int iCountWidth = 0;

	if (m_pTitleLabel)
	{
		vgui::HFont hTitleFont = m_pTitleLabel->GetFont();
		UTIL_ComputeStringWidth(hTitleFont, pwszTitleString);
	}

	if (m_pCountLabel)
	{
		vgui::HFont hCountFont = m_pCountLabel->GetFont();
		UTIL_ComputeStringWidth(hCountFont, pwszCountString);
	}

	const int iTotalWidth =
		vgui::scheme()->GetProportionalScaledValue(32) +
		iTitleWidth + iCountWidth;

	return iTotalWidth;
}


//-----------------------------------------------------------------------------
// Purpose: Show counter on the HUD
//-----------------------------------------------------------------------------
void CHudCounter::Show()
{
	m_bShouldDraw = true;
	m_bHiding = false;

	InvalidateLayout(true);

	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(
		"ShowCounter"
	);

	if (m_pBackgroundPanel)
	{
		m_pBackgroundPanel->SetWide(0);

		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pBackgroundPanel,
			"wide",
			static_cast<float>(GetWide()),
			0.001F,
			0.2F,
			vgui::AnimationController::INTERPOLATOR_LINEAR
		);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Hide counter from the HUD
//-----------------------------------------------------------------------------
void CHudCounter::Hide()
{
	if (!m_bShouldDraw || m_bHiding)
		return;

	m_bHiding = true;
	m_flHideTime = gpGlobals->curtime + 0.5F;
	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(
		"HideCounter"
	);

	if (m_pTitleLabel)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pTitleLabel,
			"alpha",
			0.0F,
			0.0F,
			0.5F,
			vgui::AnimationController::INTERPOLATOR_LINEAR
		);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Sets the maximum counter value
//-----------------------------------------------------------------------------
void CHudCounter::SetMaxValue(int iMaxValue)
{
	m_iMaxValue = iMaxValue;

	InvalidateLayout();
}


//-----------------------------------------------------------------------------
// Purpose: Sets the counter title
//-----------------------------------------------------------------------------
void CHudCounter::SetTitle(const char* pszTitle)
{
	if (!pszTitle)
	{
		m_iszTitle = MAKE_STRING("UNKNOWN");
		return;
	}

	m_iszTitle = AllocPooledString(pszTitle);

	InvalidateLayout();
}


//-----------------------------------------------------------------------------
// Purpose: Sets the counter value
//-----------------------------------------------------------------------------
void CHudCounter::SetValue(int iValue, bool bUpdate)
{
	m_iValue = iValue;

	wchar_t wcszMaxValueString[16] = {};
	V_snwprintf(
		wcszMaxValueString,
		Q_ARRAYSIZE(wcszMaxValueString),
		L"%i",
		m_iMaxValue
	);

	wchar_t wcszValueString[16] = {};
	V_snwprintf(
		wcszValueString,
		Q_ARRAYSIZE(wcszValueString),
		L"%i",
		m_iValue
	);

	const wchar_t* pwcszFormatToken =
		g_pVGuiLocalize->Find("#twhltower_Counter_Fmt");

	if (!pwcszFormatToken)
	{
		pwcszFormatToken = L"%s1 / %s2";
	}

	wchar_t wcszCountString[32];
	g_pVGuiLocalize->ConstructString(
		wcszCountString,
		sizeof(wcszCountString),
		pwcszFormatToken,
		2,
		wcszValueString,
		wcszMaxValueString
	);

	SetDialogVariable("count", wcszCountString);

	if (!bUpdate)
		return;

	if (!m_bShouldDraw || m_bHiding)
		return;

	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(
		"CounterChanged"
	);
}

