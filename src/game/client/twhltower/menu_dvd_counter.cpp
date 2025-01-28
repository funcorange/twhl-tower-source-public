//=============================================================================
//
// Purpose: Pause menu DVD counter
//
//=============================================================================

#include "cbase.h"
#include "menu_dvd_counter.h"

#include <vgui/ILocalize.h>
#include <vgui/IScheme.h>
#include <vgui/IVGui.h>

#include "twhltower_hudelement.h"
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/ImagePanel.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/Panel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CMenuDvdCounter* g_pMenuDvdCounter = NULL;

void CreateMenuDvdCounter(vgui::VPANEL parent, const char* pszElementName)
{
	if (g_pMenuDvdCounter != NULL)
	{
		delete g_pMenuDvdCounter;
	}
	g_pMenuDvdCounter = new CMenuDvdCounter(NULL, pszElementName);
	g_pMenuDvdCounter->SetParent(parent);
	UpdateMenuDvdCounter();
}


void DestroyMenuDvdCounter()
{
	if (g_pMenuDvdCounter == NULL)
	{
		return;
	}
	delete g_pMenuDvdCounter;
}


void UpdateMenuDvdCounter()
{
	if (g_pMenuDvdCounter == NULL)
	{
		return;
	}
	bool const bVisible = engine->IsInGame()
		&& !engine->IsLevelMainMenuBackground();
	g_pMenuDvdCounter->SetEnabled(bVisible);
	g_pMenuDvdCounter->SetVisible(bVisible);
}


BEGIN_DATADESC(CMenuDvdCounter)
	DEFINE_FIELD(m_iTotal, FIELD_INTEGER),
	DEFINE_FIELD(m_iCount, FIELD_INTEGER),
	DEFINE_FIELD(m_bCurrentFloorDiscFound, FIELD_BOOLEAN),
	DEFINE_FIELD(m_iState, FIELD_INTEGER),
	DEFINE_FIELD(m_bEnabled, FIELD_BOOLEAN),
	DEFINE_FIELD(m_bVisible, FIELD_BOOLEAN),
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CMenuDvdCounter::CMenuDvdCounter(
	vgui::Panel* pParent,
	const char* pszElementName
) :
	m_pBackgroundPanel(new vgui::Panel(this, "BackgroundPanel")),
	m_pDvdImage(new vgui::ImagePanel(this, "DvdImage")),
	m_pTotalLabel(new vgui::Label(this, "TotalLabel", L"TOTAL:")),
	m_pCounterLabel(new vgui::Label(this, "CounterLabel", L"0/0")),
	m_pThisFloorLabel(new vgui::Label(this, "ThisFloorLabel", L"THIS FLOOR:")),
	m_pFoundLabel(new vgui::Label(this, "FoundLabel", L"NOT FOUND")),
	m_iTotal(0),
	m_iCount(0),
	BaseClass(pszElementName),
	vgui::EditablePanel(pParent, pszElementName)
{
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMenuDvdCounter::ApplySchemeSettings(vgui::IScheme* const pScheme)
{
	vgui::EditablePanel::ApplySchemeSettings(pScheme);
	LoadControlSettings("resource/UI/menudvdcounter.res");
	m_hLabelFont = pScheme->GetFont("DvdCounter", true);
	m_pTotalLabel->SetFont(m_hLabelFont);
	m_pCounterLabel->SetFont(m_hLabelFont);
	m_pThisFloorLabel->SetFont(m_hLabelFont);
	m_pFoundLabel->SetFont(m_hLabelFont);
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMenuDvdCounter::PerformLayout()
{
	vgui::EditablePanel::PerformLayout();

	// DVD icon
	m_pDvdImage->SetPos(
		vgui::scheme()->GetProportionalScaledValue(8),
		vgui::scheme()->GetProportionalScaledValue(8)
	);
	m_pDvdImage->SetSize(
		vgui::scheme()->GetProportionalScaledValue(32),
		vgui::scheme()->GetProportionalScaledValue(32)
	);

	// "Total" label
	wchar_t const* totalLabelText = g_pVGuiLocalize->Find(
		"#GameUI_DvdCounter_Total"
	);
	if (totalLabelText == NULL)
	{
		totalLabelText = L"TOTAL:";
	}
	m_pTotalLabel->SetText(totalLabelText);
	int const totalLabelWide = UTIL_ComputeStringWidth(
		m_hLabelFont,
		totalLabelText
	);
	int totalY = 0;
	if (m_iState == MenuDvdCounterState_TotalAndCurrent)
	{
		totalY = vgui::scheme()->GetProportionalScaledValue(8);
	}
	else
	{
		totalY = vgui::scheme()->GetProportionalScaledValue(16);
	}
	m_pTotalLabel->SetPos(
		vgui::scheme()->GetProportionalScaledValue(48),
		totalY
	);
	m_pTotalLabel->SetSize(
		totalLabelWide,
		vgui::scheme()->GetProportionalScaledValue(12)
	);

	// "This floor" label
	int thisFloorLabelWide = 0;
	if (m_iState == MenuDvdCounterState_TotalAndCurrent)
	{
		m_pThisFloorLabel->SetEnabled(true);
		m_pThisFloorLabel->SetVisible(true);

		wchar_t const* thisFloorText = g_pVGuiLocalize->Find(
			"#GameUI_DvdCounter_ThisFloor"
		);
		if (thisFloorText == NULL)
		{
			thisFloorText = L"THIS FLOOR:";
		}
		m_pThisFloorLabel->SetText(thisFloorText);
		thisFloorLabelWide = UTIL_ComputeStringWidth(
			m_hLabelFont,
			thisFloorText
		);
		m_pThisFloorLabel->SetPos(
			vgui::scheme()->GetProportionalScaledValue(48),
			vgui::scheme()->GetProportionalScaledValue(24)
		);
		m_pThisFloorLabel->SetSize(
			thisFloorLabelWide,
			vgui::scheme()->GetProportionalScaledValue(12)
		);
	}
	else
	{
		m_pThisFloorLabel->SetEnabled(false);
		m_pThisFloorLabel->SetVisible(false);
	}

	int const titleWidth = Max(totalLabelWide, thisFloorLabelWide);

	// Counter label
	wchar_t const* counterFormat = g_pVGuiLocalize->Find(
		"twhltower_DvdMessage_Fmt"
	);
	if (counterFormat == NULL)
	{
		counterFormat = L"%s1/%s2";
	}
	wchar_t totalText[8] = { 0 };
	V_snwprintf(totalText, Q_ARRAYSIZE(totalText), L"%i", m_iTotal);
	wchar_t countText[8] = { 0 };
	V_snwprintf(countText, Q_ARRAYSIZE(countText), L"%i", m_iCount);
	wchar_t counterText[16] = { 0 };
	g_pVGuiLocalize->ConstructString(
		counterText,
		sizeof(counterText),
		counterFormat,
		2,
		countText,
		totalText
	);
	int const counterWidth = UTIL_ComputeStringWidth(
		m_hLabelFont,
		counterText
	);
	m_pCounterLabel->SetText(counterText);
	m_pCounterLabel->SetPos(
		vgui::scheme()->GetProportionalScaledValue(56) + titleWidth,
		totalY
	);
	m_pCounterLabel->SetSize(
		counterWidth,
		vgui::scheme()->GetProportionalScaledValue(12)
	);

	// Found/not found label
	int foundWidth = 0;
	if (m_iState == MenuDvdCounterState_TotalAndCurrent)
	{
		m_pFoundLabel->SetEnabled(true);
		m_pFoundLabel->SetVisible(true);

		wchar_t const* foundText = NULL;
		if (m_bCurrentFloorDiscFound)
		{
			foundText = g_pVGuiLocalize->Find(
				"#GameUI_DvdCounter_Found"
			);
			if (foundText == NULL)
			{
				foundText = L"FOUND";
			}
		}
		else
		{
			foundText = g_pVGuiLocalize->Find(
				"#GameUI_DvdCounter_NotFound"
			);
			if (foundText == NULL)
			{
				foundText = L"NOT FOUND";
			}
		}
		m_pFoundLabel->SetText(foundText);
		foundWidth = UTIL_ComputeStringWidth(
			m_hLabelFont,
			foundText
		);
		m_pFoundLabel->SetPos(
			vgui::scheme()->GetProportionalScaledValue(56) + titleWidth,
			vgui::scheme()->GetProportionalScaledValue(24)
		);
		m_pFoundLabel->SetSize(
			foundWidth,
			vgui::scheme()->GetProportionalScaledValue(12)
		);
	}
	else
	{
		m_pFoundLabel->SetEnabled(false);
		m_pFoundLabel->SetVisible(false);
	}

	// Background panel
	int const indicatorWidth = Max(counterWidth, foundWidth);
	int const wide = vgui::scheme()->GetProportionalScaledValue(64)
		+ titleWidth + indicatorWidth;
	int const tall = vgui::scheme()->GetProportionalScaledValue(48);
	SetSize(wide, tall);
	m_pBackgroundPanel->SetSize(wide, tall);

	int parentWide = 0;
	int parentTall = 0;
	vgui::ipanel()->GetSize(
		vgui::ipanel()->GetParent(GetVPanel()),
		parentWide,
		parentTall
	);
	int const x = parentWide - vgui::scheme()->GetProportionalScaledValue(32)
		- wide;
	int const y = parentTall - vgui::scheme()->GetProportionalScaledValue(80);
	SetPos(x, y);
}


//-----------------------------------------------------------------------------
// Purpose: Set max number for counter
//-----------------------------------------------------------------------------
void CMenuDvdCounter::SetTotal(int const iTotal)
{
	if (iTotal == m_iTotal)
	{
		return;
	}
	m_iTotal = iTotal;
	InvalidateLayout();
}


//-----------------------------------------------------------------------------
// Purpose: Set current number for counter
//-----------------------------------------------------------------------------
void CMenuDvdCounter::SetCount(int const iCount)
{
	if (iCount == m_iCount)
	{
		return;
	}
	m_iCount = iCount;
	InvalidateLayout();
}


//-----------------------------------------------------------------------------
// Purpose: Set whether the disc for the current floor has been found
//-----------------------------------------------------------------------------
void CMenuDvdCounter::SetCurrentFloorDiscFound(bool const bFound)
{
	if (bFound == m_bCurrentFloorDiscFound)
	{
		return;
	}
	m_bCurrentFloorDiscFound = bFound;
	InvalidateLayout();
}


void CMenuDvdCounter::SetEnabled(bool const bEnabled)
{
	m_bEnabled = bEnabled;
	UpdateEnabled();
}


void CMenuDvdCounter::SetVisible(bool const bVisible)
{
	m_bVisible = bVisible;
	UpdateVisible();
}


void CMenuDvdCounter::UpdateEnabled()
{
	if (m_iState == MenuDvdCounterState_Hidden)
	{
		vgui::EditablePanel::SetEnabled(false);
		return;
	}
	vgui::EditablePanel::SetEnabled(m_bEnabled);
}


void CMenuDvdCounter::UpdateVisible()
{
	if (m_iState == MenuDvdCounterState_Hidden)
	{
		vgui::EditablePanel::SetVisible(false);
		return;
	}
	vgui::EditablePanel::SetVisible(m_bVisible);
}


void CMenuDvdCounter::SetState(MenuDvdCounterState_e const state)
{
	if (state == m_iState)
	{
		return;
	}
	m_iState = state;
	DevMsg("CMenuDvdCounter::SetState(), m_iState = %i\n", m_iState);
	UpdateEnabled();
	UpdateVisible();
}

