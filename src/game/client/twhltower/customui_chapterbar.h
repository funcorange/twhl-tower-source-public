//==============================================================================
//
// Purpose: Custom UI chapter select bar
//
//==============================================================================

#ifndef CUSTOMUI_CHAPTERBAR_H
#define CUSTOMUI_CHAPTERBAR_H

#include "customui_chapterbutton.h"
#include "vgui_controls/EditablePanel.h"

class CCustomUIChapterBar : public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE(CCustomUIChapterBar, vgui::EditablePanel);

	CCustomUIChapterBar(vgui::Panel* pParent, const char* pszPanelName);
	virtual ~CCustomUIChapterBar();

	virtual void	OnMouseWheeled(int iDelta) override;
	virtual void	PerformLayout() override;
	virtual void	ApplySchemeSettings(vgui::IScheme* pScheme) override;
	void			NavigateTo() override;

	MESSAGE_FUNC_INT(OnChapterSelected, "SelectChapter", iChapter);
	MESSAGE_FUNC_INT(OnChapterStarted, "StartChapter", iChapter);
	MESSAGE_FUNC_INT(OnChapterNavigatedTo, "NavigateToChapter", iChapter);

	void			AddChapter(int iChapterNumber, const wchar_t* pwcszChapterNumberString, const char* pszThumbnail);
	void			SetUnlockedChapters(int iUnlockedChapters);
	void			OnShow();
	void			ScrollToFitChapter(int iChapter);

private:
	int				GetChapterSelectionWidth();
	void			MoveChapterSelection();
	void			SetChapterSelectionOffset(int iChapterSelectionOffset);
	int				CalculateScrollOffsetForChapter(int iChapter, int x, int leftBound, int rightBound);

	CUtlVector<CCustomUIChapterButton*>		m_ChapterButtons;
	int				m_iChapterSelectionOffset;
};

#endif // !CUSTOMUI_CHAPTERBAR_H
