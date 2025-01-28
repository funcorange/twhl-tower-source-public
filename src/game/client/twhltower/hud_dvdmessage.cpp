//==============================================================================
//
// Purpose: DVD pickup message hud element
//
//==============================================================================

#include "cbase.h"
#include "hud_dvdmessage.h"

#include <vgui/ILocalize.h>
#include <vgui/IVGui.h>

#include "convar.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "iclientmode.h"
#include "vgui_controls/AnimationController.h"
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/ImagePanel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define DVDMESSAGE_SHOW_TIME 10.0f
#define DVDMESSAGE_HIDE_ANIM_TIME 0.5f
#define DVDMESSAGE_UPDATE_DELAY 1.0f

DECLARE_HUDELEMENT(CHudDvdMessage);
DECLARE_HUD_MESSAGE(CHudDvdMessage, DvdMessage);

//------------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------------
CHudDvdMessage::CHudDvdMessage(const char* pszElementName) :
	m_pPanelBackground(NULL),
	m_pImagePanelIcon(NULL),
	m_pLabelProgress(NULL),
	m_pLabelMessage(NULL),
	m_pLabelProgressGlow(NULL),
	m_iCurrentValue(0),
	m_iNextValue(0),
	m_iMaxValue(0),
	m_flHideTime(0.0f),
	m_flUpdateTime(0.0f),
	m_iUpdate(0),
	CHudElement(pszElementName),
	vgui::EditablePanel(NULL, "HudDvdMessage")
{
	vgui::Panel* pPanel = g_pClientMode->GetViewport();
	SetParent(pPanel);

	m_pPanelBackground = new vgui::EditablePanel(this, "DvdBackgroundPanel");
	m_pImagePanelIcon = new vgui::ImagePanel(this, "DvdIcon");
	m_pLabelProgress = new vgui::Label(this, "DvdProgressLabel", "");
	m_pLabelMessage = new vgui::Label(this, "DvdMessageLabel", "");
	m_pLabelProgressGlow = new vgui::Label(this, "DvdProgressGlowLabel", "");

	vgui::ivgui()->AddTickSignal(GetVPanel());
}


//------------------------------------------------------------------------------
// Purpose: Destructor
//------------------------------------------------------------------------------
CHudDvdMessage::~CHudDvdMessage()
{
}


//------------------------------------------------------------------------------
// Purpose: Initialize HUD element
//------------------------------------------------------------------------------
void CHudDvdMessage::Init()
{
	HOOK_HUD_MESSAGE(CHudDvdMessage, DvdMessage);
}


//------------------------------------------------------------------------------
// Purpose: Reset HUD element
//------------------------------------------------------------------------------
void CHudDvdMessage::Reset()
{
	m_flHideTime = 0.0f;
}


//------------------------------------------------------------------------------
// Purpose: Load scheme file for DVD message
//------------------------------------------------------------------------------
void CHudDvdMessage::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	LoadControlSettings("resource/UI/HudDvdMessage.res");

	BaseClass::ApplySchemeSettings(pScheme);
}


//------------------------------------------------------------------------------
// Purpose: Determine whether this control is visible
//------------------------------------------------------------------------------
bool CHudDvdMessage::ShouldDraw()
{
	return (m_flHideTime > 0 && gpGlobals->curtime < m_flHideTime && CHudElement::ShouldDraw());
}


//------------------------------------------------------------------------------
// Purpose: Update animations
//------------------------------------------------------------------------------
void CHudDvdMessage::OnTick()
{
	if (m_flHideTime <= 0)
		return;

	if (m_iUpdate == 1 && gpGlobals->curtime >= m_flUpdateTime)
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("PulseDvdMessage");
		m_iUpdate = 2;
	}

	if (m_iUpdate == 2 && gpGlobals->curtime >= m_flUpdateTime + 0.1f)
	{
		m_iCurrentValue = m_iNextValue;
		SetProgressText(m_iCurrentValue, m_iMaxValue);
		m_iUpdate = 0;
	}

	if (!m_bHiding && gpGlobals->curtime >= m_flHideTime - DVDMESSAGE_HIDE_ANIM_TIME)
	{
		m_bHiding = true;
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HideDvdMessage");
	}

	if (gpGlobals->curtime >= m_flHideTime)
	{
		m_flHideTime	= 0.0f;
		m_bHiding		= false;
	}
}


//------------------------------------------------------------------------------
// Purpose: Update progress and display HUD element
//------------------------------------------------------------------------------
void CHudDvdMessage::ShowDvdMessage(int iCollected, int iMax, const wchar_t* pwszMessage)
{
	m_iNextValue	= iCollected;
	m_iCurrentValue	= m_iNextValue - 1;
	m_iMaxValue		= iMax;

	vgui::HFont hFontMessage = m_pLabelMessage->GetFont();
	int iMessageWidth;

	if (m_iNextValue == m_iMaxValue)
	{
		const wchar_t* pwszAllMessage = g_pVGuiLocalize->Find("#twhltower_DvdMessage_All");
		if (pwszAllMessage)
		{
			SetDialogVariable("message", pwszAllMessage);
			iMessageWidth = UTIL_ComputeStringWidth(hFontMessage, pwszAllMessage);
		}
		else
		{
			const wchar_t wszAllMessage[] = L"You found every Half-Life® 2 disc! Congratulations!";
			SetDialogVariable("message", wszAllMessage);
			iMessageWidth = UTIL_ComputeStringWidth(hFontMessage, wszAllMessage);
		}
	}
	else
	{
		SetDialogVariable("message", pwszMessage);
		iMessageWidth = UTIL_ComputeStringWidth(hFontMessage, pwszMessage);
	}

	int iMessageX, iMessageY;
	m_pLabelMessage->GetPos(iMessageX, iMessageY);
	int iVariableWidth = iMessageX + iMessageWidth + vgui::scheme()->GetProportionalScaledValue(4);
	int iMaxWidth = GetWide();
	m_pPanelBackground->SetWide(min(iVariableWidth, iMaxWidth));

	SetProgressText(m_iCurrentValue, m_iMaxValue);

	m_flHideTime	= gpGlobals->curtime + DVDMESSAGE_SHOW_TIME;
	m_flUpdateTime	= gpGlobals->curtime + DVDMESSAGE_UPDATE_DELAY;
	m_iUpdate		= 1;

	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("ShowDvdMessage");
}


//------------------------------------------------------------------------------
// Purpose: Handle DvdMessage message
//------------------------------------------------------------------------------
void CHudDvdMessage::MsgFunc_DvdMessage(bf_read& msg)
{
	int iNumCollected	= msg.ReadByte();	// Get current number of collected discs
	int iNumTotal		= msg.ReadByte();	// Get total number of discs

	char szMessage[512] = {0};
	msg.ReadString(szMessage, sizeof(szMessage));	// Get pickup message string

	wchar_t* pwszLocalizedMessage = NULL;
	if (!szMessage[0])
	{
		// Provide a default message if none was specified
		wchar_t* pwszDefaultMessage = g_pVGuiLocalize->Find("#twhltower_DvdMessage_Default");
		if (!pwszDefaultMessage || !pwszDefaultMessage[0])
		{
			// Provide a default string if the message couldn't be localized
			pwszLocalizedMessage = L"You picked up a Half-Life® 2 disc! Collect them all!";
		}
		else
		{
			pwszLocalizedMessage = pwszDefaultMessage;
		}
	}
	else
	{
		if (szMessage[0] != '#')
		{
			// Use the message as-is
			wchar_t wszConvertedMessage[512] = {0};
			g_pVGuiLocalize->ConvertANSIToUnicode(szMessage, wszConvertedMessage, sizeof(wszConvertedMessage));
			pwszLocalizedMessage = wszConvertedMessage;
		}
		else
		{
			// Localize the message
			pwszLocalizedMessage = g_pVGuiLocalize->Find(szMessage);
		}
	}

	// Show the DVD pickup message
	ShowDvdMessage(iNumCollected, iNumTotal, pwszLocalizedMessage);
}


//------------------------------------------------------------------------------
// Purpose: Set progress display
//------------------------------------------------------------------------------
void CHudDvdMessage::SetProgressText(int iNumCollected, int iNumTotal)
{
	wchar_t wszProgress[64] = { 0 };

	const wchar_t* pwszFmt = g_pVGuiLocalize->Find("#twhltower_DvdMessage_Fmt");
	if (pwszFmt && pwszFmt[0])
	{
		wchar_t wszFmt[128] = { 0 };
		V_wcsncpy(wszFmt, pwszFmt, sizeof(wszFmt));

		wchar_t wszNumFound[16] = { 0 };
		wchar_t wszNumTotal[16] = { 0 };
		_snwprintf(wszNumFound, ARRAYSIZE(wszNumFound), L"%i", iNumCollected);
		_snwprintf(wszNumTotal, ARRAYSIZE(wszNumTotal), L"%i", iNumTotal);

		g_pVGuiLocalize->ConstructString(wszProgress, sizeof(wszProgress), wszFmt, 2, wszNumFound, wszNumTotal);
	}
	else
	{
		_snwprintf(wszProgress, ARRAYSIZE(wszProgress), L"%i/%i", iNumCollected, iNumTotal);
	}

	SetDialogVariable("progress", wszProgress);
}


CON_COMMAND_F(dvd_message_test, "Test the DVD message hud element\n", FCVAR_CHEAT | FCVAR_DEVELOPMENTONLY)
{
	static int iCount = 14;

	CHudDvdMessage* pPanel = GET_HUDELEMENT(CHudDvdMessage);
	if (!pPanel)
		return;

	pPanel->ShowDvdMessage((iCount % 18) + 1, 18, g_pVGuiLocalize->Find("#twhltower_DvdMessage_Default"));

	iCount++;
}

