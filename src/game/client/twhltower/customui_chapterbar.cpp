//==============================================================================
//
// Purpose: Custom UI chapter select bar
//
//==============================================================================

#include "cbase.h"
#include "customui_chapterbar.h"

#include <vgui/IScheme.h>

#include "iclientmode.h"
#include "vgui_controls/AnimationController.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define NORMALIZED_SCROLL_DISTANCE	40	// How far to scroll the selection with the scroll wheel in normalized pixels
#define CHAPTER_BUTTON_WIDTH		106	// Width in normalized pixels of a chapter button
#define CHAPTER_BUTTON_SPACING		16	// Space between chapter buttons in normalized pixels
#define SELECTION_AREA_BUFFER		16	// Distance from the edges of the screen in normalized pixels to keep chapter scroll within
#define SELECTION_AREA_PADDING		32	// Padding in normalized pixels to add when scrolling to a chapter that is not the first or the last

DECLARE_BUILD_FACTORY(CCustomUIChapterBar);

CCustomUIChapterBar::CCustomUIChapterBar(vgui::Panel* pParent, const char* pszPanelName) :
	m_ChapterButtons(),
	m_iChapterSelectionOffset(0),
	BaseClass(pParent, pszPanelName)
{
	SetMouseInputEnabled(true);
}


CCustomUIChapterBar::~CCustomUIChapterBar()
{
}


void CCustomUIChapterBar::OnMouseWheeled(int iDelta)
{
	const int iScrollAmount = vgui::scheme()->GetProportionalScaledValue(NORMALIZED_SCROLL_DISTANCE * iDelta);
	SetChapterSelectionOffset(m_iChapterSelectionOffset + iScrollAmount);
}


void CCustomUIChapterBar::PerformLayout()
{
	BaseClass::PerformLayout();

	int iChapterButtonCount = m_ChapterButtons.Count();
	for (int i = 0; i < iChapterButtonCount; i++)
	{
		CCustomUIChapterButton* pButton = m_ChapterButtons.Element(i);
		if (pButton == NULL)
			continue;

		int x, y;
		x = CHAPTER_BUTTON_SPACING + i * (CHAPTER_BUTTON_WIDTH + CHAPTER_BUTTON_SPACING);
		y = 8;

		pButton->SetPos(
			vgui::scheme()->GetProportionalScaledValue(x),
			vgui::scheme()->GetProportionalScaledValue(y)
		);

		int wide = vgui::scheme()->GetProportionalScaledValue(106);
		int tall = vgui::scheme()->GetProportionalScaledValue(80);
		pButton->SetSize(wide, tall);
	}
}


void CCustomUIChapterBar::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	int iChapterButtonCount = m_ChapterButtons.Count();
	for (int i = 0; i < iChapterButtonCount; i++)
	{
		CCustomUIChapterButton* pButton = m_ChapterButtons.Element(i);
		if (pButton == NULL)
			continue;

		pButton->SetBackgroundOutColor(Color(0, 0, 0, 255));
		pButton->SetBackgroundOverColor(Color(64, 64, 64, 255));
		pButton->SetBackgroundPressedColor(Color(255, 255, 255, 255));
		pButton->SetBackgroundActiveColor(Color(255, 160, 0, 255));
		pButton->SetTextOutColor(Color(255, 255, 255, 255));
		pButton->SetTextOverColor(Color(255, 255, 255, 255));
		pButton->SetTextPressedColor(Color(255, 255, 255, 255));
		pButton->SetTextActiveColor(Color(255, 255, 255, 255));
		pButton->ApplySchemeSettings(pScheme);
		pButton->SetBgColor(Color(0, 0, 0, 0));
	}
}


void CCustomUIChapterBar::NavigateTo()
{
	const int nCount = m_ChapterButtons.Count();
	for (int i = 0; i < nCount; i++)
	{
		CCustomUIChapterButton* pButton = m_ChapterButtons.Element(i);
		if (pButton == NULL)
			continue;

		if (pButton->IsActive())
		{
			pButton->NavigateTo();
			return;
		}
	}
}


void CCustomUIChapterBar::OnChapterSelected(int iChapter)
{
	int iChapterButtonCount = m_ChapterButtons.Count();
	for (int i = 0; i < iChapterButtonCount; i++)
	{
		CCustomUIChapterButton* pButton = m_ChapterButtons.Element(i);
		if (pButton == NULL)
			continue;

		if (i + 1 == iChapter)
		{
			pButton->SetActive(true);
			pButton->RequestFocus();
		}
		else
		{
			pButton->SetActive(false);
			pButton->NavigateFrom();
		}
	}

	PostMessage(GetParent(), new KeyValues("SelectChapter", "iChapter", iChapter));
}


void CCustomUIChapterBar::OnChapterStarted(int iChapter)
{
	PostMessage(GetParent(), new KeyValues("StartChapter", "iChapter", iChapter));
}


//---------------------------------------------------------------------------
// Purpose:	Called when a chapter button is highlighted
// Input:	Number of the chapter that was navigated to.
//---------------------------------------------------------------------------
void CCustomUIChapterBar::OnChapterNavigatedTo(int iChapter)
{
	int iChapterButtonCount = m_ChapterButtons.Count();
	for (int i = 0; i < iChapterButtonCount; i++)
	{
		if (i + 1 == iChapter)
			continue;

		CCustomUIChapterButton* pButton = m_ChapterButtons.Element(i);
		if (pButton == NULL)
			continue;

		pButton->NavigateFrom();
	}

	ScrollToFitChapter(iChapter);
}


void CCustomUIChapterBar::AddChapter(int iChapterNumber, const wchar_t* pwcszChapterNumberString, const char* pszThumbnail)
{
	char szPanelName[64];
	V_snprintf(szPanelName, ARRAYSIZE(szPanelName), "Chapter%i", iChapterNumber);
	CCustomUIChapterButton* pButton = new CCustomUIChapterButton(this, szPanelName);
	pButton->SetChapterNumber(iChapterNumber);
	pButton->SetChapterNumberString(pwcszChapterNumberString);
	pButton->SetChapterThumbnail(pszThumbnail);

	if (m_ChapterButtons.Count() > 0)
	{
		CCustomUIChapterButton* pPreviousButton = m_ChapterButtons.Tail();
		pPreviousButton->SetNavRight(pButton);
		pButton->SetNavLeft(pPreviousButton);
		pButton->SetNavDown(GetNavDown());
	}

	m_ChapterButtons.AddToTail(pButton);
}


void CCustomUIChapterBar::SetUnlockedChapters(int iUnlockedChapters)
{
	const int buttonCount = m_ChapterButtons.Count();
	for (int i = 0; i < buttonCount; i++)
	{
		CCustomUIChapterButton* pButton = m_ChapterButtons.Element(i);
		if (pButton == NULL)
			continue;

		pButton->SetLocked(i >= iUnlockedChapters);
	}
}


//----------------------------------------------------------------------------
// Purpose: Called when the new game menu is shown
//----------------------------------------------------------------------------
void CCustomUIChapterBar::OnShow()
{
	// Set focus to currently selected chapter button
	const int nCount = m_ChapterButtons.Count();
	for (int i = 0; i < nCount; i++)
	{
		CCustomUIChapterButton* pButton = m_ChapterButtons.Element(i);
		if (!pButton)
			continue;

		if (pButton->IsActive())
		{
			pButton->RequestFocus();
			return;
		}
	}
}


//----------------------------------------------------------------------------
// Purpose:	Scroll the chapter selection so that the specified chapter fits
//			within the visible area.
// Input:	iChapter - Number of the chapter to scroll to.
//----------------------------------------------------------------------------
void CCustomUIChapterBar::ScrollToFitChapter(int iChapter)
{
	// Get the bounds of the specified chapter
	const int nChapterCount = m_ChapterButtons.Count();
	if (iChapter > nChapterCount)
		return;

	CCustomUIChapterButton* pButton = m_ChapterButtons.Element(iChapter - 1);
	if (!pButton)
		return;

	int x, y, wide, tall;
	pButton->GetPos(x, y);
	pButton->GetSize(wide, tall);

	// Calculate scrollable area.
	// Scrollable area is slightly smaller if there is a chapter before or after
	// so that it may be visible after scrolling.
	const int iLeftBound = vgui::scheme()->GetProportionalScaledValue(SELECTION_AREA_BUFFER + ((iChapter > 1) ? SELECTION_AREA_PADDING : 0));
	const int iRightBound = GetWide() - vgui::scheme()->GetProportionalScaledValue(SELECTION_AREA_BUFFER + ((iChapter < nChapterCount) ? SELECTION_AREA_PADDING : 0));
	
	// Check if button is already within scrollable area
	if (x >= iLeftBound && (x + wide) <= iRightBound)
		// Don't need to scroll
		return;

	// Calculate scroll offset for chapter
	const int iScrollOffset = CalculateScrollOffsetForChapter(iChapter, x, iLeftBound, iRightBound);

	// Scroll!
	SetChapterSelectionOffset(iScrollOffset);
}


int CCustomUIChapterBar::GetChapterSelectionWidth()
{
	int iChapterButtonCount = m_ChapterButtons.Count();
	int iChapterSelectionWidth = iChapterButtonCount * CHAPTER_BUTTON_WIDTH + (iChapterButtonCount - 1) * CHAPTER_BUTTON_SPACING;
	return vgui::scheme()->GetProportionalScaledValue(iChapterSelectionWidth);
}


void CCustomUIChapterBar::MoveChapterSelection()
{
	int iChapterButtonCount = m_ChapterButtons.Count();

	// Animate the position of each chapter button
	for (int i = 0; i < iChapterButtonCount; i++)
	{
		CCustomUIChapterButton* pButton = m_ChapterButtons.Element(i);
		if (pButton == NULL)
			continue;

		int targetX = vgui::scheme()->GetProportionalScaledValue(SELECTION_AREA_BUFFER + i * (CHAPTER_BUTTON_WIDTH + CHAPTER_BUTTON_SPACING)) + m_iChapterSelectionOffset;

		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			pButton,
			"xpos",
			targetX,
			0,
			0.2f,
			vgui::AnimationController::Interpolators_e::INTERPOLATOR_DEACCEL
		);
	}
}


void CCustomUIChapterBar::SetChapterSelectionOffset(int iChapterSelectionOffset)
{
	m_iChapterSelectionOffset = iChapterSelectionOffset;

	int iMaxOffset = -GetChapterSelectionWidth() + GetWide() - vgui::scheme()->GetProportionalScaledValue(32);
	m_iChapterSelectionOffset = Min(0, Clamp(m_iChapterSelectionOffset, iMaxOffset, 0));

	MoveChapterSelection();
}


//----------------------------------------------------------------------------
// Purpose:	Calculates the scroll offset needed to fit a chapter in the
//			selection area.
// Input:
//	iChapter	- Number of the chapter to scroll to
//	x			- x-coordinate of the chapter button
//	leftBound	- Left bound of the selection area
//	rightBound	- Right bound of the selection area
// Output:	Scroll offset for the chapter
//----------------------------------------------------------------------------
int CCustomUIChapterBar::CalculateScrollOffsetForChapter(int iChapter, int x, int leftBound, int rightBound)
{
	// Is the button to the left or to the right of the selection area?
	if (x < leftBound)
	{
		// To the left
		return leftBound - vgui::scheme()->GetProportionalScaledValue(SELECTION_AREA_BUFFER + ((iChapter - 1) * (CHAPTER_BUTTON_WIDTH + CHAPTER_BUTTON_SPACING)));
	}

	// To the right
	return rightBound - vgui::scheme()->GetProportionalScaledValue(SELECTION_AREA_BUFFER + (iChapter * CHAPTER_BUTTON_WIDTH + (iChapter - 1) * CHAPTER_BUTTON_SPACING));
}

