//==============================================================================
//
// Purpose: Custom UI chapter button
//
//==============================================================================

#ifndef CUSTOMUI_CHAPTERBUTTON_H
#define CUSTOMUI_CHAPTERBUTTON_H

#include "customui_button.h"
#include "vgui_controls/ImagePanel.h"

//------------------------------------------------------------------------------
// CCustomUIChapterButton
// 
// Purpose: Button that shows a floor number and chapter thumbnail for the
// custom UI new game menu.
//------------------------------------------------------------------------------
class CCustomUIChapterButton : public CCustomUIButton
{
public:
	DECLARE_CLASS_SIMPLE(CCustomUIChapterButton, CCustomUIButton);

	CCustomUIChapterButton(vgui::Panel* pParent, const char* pszPanelName);
	virtual ~CCustomUIChapterButton();

	void		OnCursorEntered() override;
	void		OnCursorExited() override;
	void		OnMousePressed(vgui::MouseCode code) override;
	void		OnMouseDoublePressed(vgui::MouseCode code) override;
	void		OnMouseReleased(vgui::MouseCode code) override;
	void		ApplySchemeSettings(vgui::IScheme* pScheme) override;
	void		NavigateTo() override;

	inline int	GetChapterNumber() { return m_iChapterNumber; }
	void		SetChapterNumber(int iChapterNumber);
	void		SetChapterNumberString(const wchar_t* pwcszChapterNumberString);
	void		SetChapterThumbnail(const char* pszThumbnail);
	void		SetLocked(bool bLocked);

protected:
	void		Press() override;
	void		UnPress() override;
	void		OnPressed() override;

private:
	int					m_iChapterNumber;
	vgui::ImagePanel*	m_pThumbnailImagePanel;
	bool				m_bIsLocked;
};

#endif // !CUSTOMUI_CHAPTERBUTTON_H
