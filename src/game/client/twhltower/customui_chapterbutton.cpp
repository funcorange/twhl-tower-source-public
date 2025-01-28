//==============================================================================
//
// Purpose: Custom UI chapter button
//
//==============================================================================

#include "cbase.h"
#include "customui_chapterbutton.h"

#include <vgui/ILocalize.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_BUILD_FACTORY(CCustomUIChapterButton);

CCustomUIChapterButton::CCustomUIChapterButton(vgui::Panel* pParent, const char* pszPanelName) :
	m_iChapterNumber(0),
	m_pThumbnailImagePanel(NULL),
	m_bIsLocked(false),
	BaseClass(pParent, pszPanelName)
{
	LoadControlSettings("resource/UI/custom/CustomUIChapterButton.res");

	// Disable mouse input on all children
	const int iChildCount = GetChildCount();
	for (int i = 0; i < iChildCount; i++)
	{
		Panel* pChild = GetChild(i);
		if (!pChild)
			continue;

		pChild->SetMouseInputEnabled(false);
	}

	m_pThumbnailImagePanel = FindControl<vgui::ImagePanel>("ChapterThumbnail");

	SetBgColor(Color(0, 0, 0, 0));
	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);
	SetConsoleStylePanel(true);
}


CCustomUIChapterButton::~CCustomUIChapterButton()
{
}


void CCustomUIChapterButton::OnCursorEntered()
{
	if (m_bIsLocked)
		return;

	BaseClass::OnCursorEntered();
}


void CCustomUIChapterButton::OnCursorExited()
{
	if (m_bIsLocked)
		return;

	BaseClass::OnCursorExited();
}


void CCustomUIChapterButton::OnMousePressed(vgui::MouseCode code)
{
	if (m_bIsLocked)
		return;

	BaseClass::OnMousePressed(code);
}


void CCustomUIChapterButton::OnMouseDoublePressed(vgui::MouseCode code)
{
	if (m_bIsLocked)
		return;

	if (code == MOUSE_LEFT)
		PostMessage(GetParent(), new KeyValues("StartChapter", "iChapter", GetChapterNumber()));

	BaseClass::OnMouseDoublePressed(code);
}


void CCustomUIChapterButton::OnMouseReleased(vgui::MouseCode code)
{
	if (m_bIsLocked)
		return;

	BaseClass::OnMouseReleased(code);
}


void CCustomUIChapterButton::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	if (m_bIsLocked)
	{
		m_pThumbnailImagePanel->SetAlpha(64);
		m_pBackgroundPanel->SetAlpha(64);
		m_pLabelText->SetAlpha(64);
	}
}


//----------------------------------------------------------------------------
// Purpose:	Called when navigating to this button
//----------------------------------------------------------------------------
void CCustomUIChapterButton::NavigateTo()
{
	BaseClass::NavigateTo();
	PostMessage(GetParent(), new KeyValues("NavigateToChapter", "iChapter", GetChapterNumber()));
}


void CCustomUIChapterButton::SetChapterNumber(int iChapterNumber)
{
	m_iChapterNumber = iChapterNumber;
}


void CCustomUIChapterButton::SetChapterNumberString(const wchar_t* pwcszChapterNumberString)
{
	if (pwcszChapterNumberString == NULL)
		return;

	wchar_t wcsBuffer[256];
	const wchar_t* pwcszFloorFmt = g_pVGuiLocalize->Find("twhltower_Floor_Fmt");

	if (pwcszFloorFmt != NULL)
	{
		g_pVGuiLocalize->ConstructString(wcsBuffer, sizeof(wcsBuffer), pwcszFloorFmt, 1, pwcszChapterNumberString);
	}
	else
	{
		V_snwprintf(wcsBuffer, ARRAYSIZE(wcsBuffer), L"Floor %s", pwcszChapterNumberString);
	}

	SetDialogVariable("label", wcsBuffer);
}


void CCustomUIChapterButton::SetChapterThumbnail(const char* pszThumbnail)
{
	if (m_pThumbnailImagePanel == NULL)
		return;

	m_pThumbnailImagePanel->SetImage(pszThumbnail);
}


void CCustomUIChapterButton::SetLocked(bool bLocked)
{
	m_bIsLocked = bLocked;

	m_pThumbnailImagePanel->SetAlpha(bLocked ? 64 : 255);
	m_pBackgroundPanel->SetAlpha(bLocked ? 64 : 255);
	m_pLabelText->SetAlpha(bLocked ? 64 : 255);
}


void CCustomUIChapterButton::Press()
{
	if (m_bIsLocked)
		return;

	BaseClass::Press();
}


void CCustomUIChapterButton::UnPress()
{
	if (m_bIsLocked)
		return;

	BaseClass::UnPress();
}


void CCustomUIChapterButton::OnPressed()
{
	BaseClass::OnPressed();

	PostMessage(GetParent(), new KeyValues("SelectChapter", "iChapter", GetChapterNumber()));
}

