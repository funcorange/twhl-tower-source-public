//==============================================================================
//
// Purpose: Custom UI new game dialog
//
//==============================================================================

#ifndef CUSTOMUI_NEWGAME_H
#define CUSTOMUI_NEWGAME_H

#include <vgui/IScheme.h>

#include "customui_chapterbar.h"
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/ImagePanel.h"

class CCustomUINewGame : public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE(CCustomUINewGame, vgui::EditablePanel);

	CCustomUINewGame(vgui::Panel* pParent);
	virtual ~CCustomUINewGame();

	void	PerformLayout() override;
	void	ApplySchemeSettings(vgui::IScheme* pScheme);

	MESSAGE_FUNC_INT(OnChapterSelected, "SelectChapter", iChapter);
	MESSAGE_FUNC_INT(OnChapterStart, "StartChapter", iChapter);
	
	void	StartNewGame();
	void	OnShow();

private:
	class CChapterInfo
	{
	public:
		CChapterInfo(
			const char* pszNumber,
			const char* pszName,
			const char* pszAuthor,
			const char* pszDescription,
			const char* pszCommand,
			const char* pszScreenshot,
			const char* pszThumbnail
		) :
			m_pszNumber(NULL),
			m_pszName(NULL),
			m_pszAuthor(NULL),
			m_pszDescription(NULL),
			m_pszCommand(NULL),
			m_pszScreenshot(NULL),
			m_pszThumbnail(NULL)
		{
			struct StringPair
			{
				const char* m_pszString1;
				const char** m_ppszString2;
			};

			StringPair rgPairs[] =
			{
				{pszNumber, &m_pszNumber},
				{pszName, &m_pszName},
				{pszAuthor, &m_pszAuthor},
				{pszDescription, &m_pszDescription},
				{pszCommand, &m_pszCommand},
				{pszScreenshot, &m_pszScreenshot},
				{pszThumbnail, &m_pszThumbnail}
			};

			for (int i = 0; i < ARRAYSIZE(rgPairs); i++)
			{
				if (rgPairs[i].m_pszString1 == NULL)
					continue;

				int len = V_strlen(rgPairs[i].m_pszString1);
				char* pszBuffer = new char[len + 1];
				V_strcpy(pszBuffer, rgPairs[i].m_pszString1);
				*(rgPairs[i].m_ppszString2) = pszBuffer;
			}
		}

		CChapterInfo(const CChapterInfo& chapterInfo) :
			CChapterInfo(
				chapterInfo.GetNumber(),
				chapterInfo.GetName(),
				chapterInfo.GetAuthor(),
				chapterInfo.GetDescription(),
				chapterInfo.GetCommand(),
				chapterInfo.GetScreenshot(),
				chapterInfo.GetThumbnail()
			)
		{
		}

		~CChapterInfo()
		{
			const char* rgpszStrings[] =
			{
				m_pszNumber,
				m_pszName,
				m_pszAuthor,
				m_pszDescription,
				m_pszCommand,
				m_pszScreenshot,
				m_pszThumbnail
			};

			for (int i = 0; i < ARRAYSIZE(rgpszStrings); i++)
			{
				if (rgpszStrings[i] == NULL)
					continue;

				delete[] rgpszStrings[i];
			}
		}

		inline const char* GetNumber() const { return m_pszNumber; }
		inline const char* GetName() const { return m_pszName; }
		inline const char* GetAuthor() const { return m_pszAuthor; }
		inline const char* GetDescription() const { return m_pszDescription; }
		inline const char* GetCommand() const { return m_pszCommand; }
		inline const char* GetScreenshot() const { return m_pszScreenshot; }
		inline const char* GetThumbnail() const { return m_pszThumbnail; }

	private:
		const char* m_pszNumber;
		const char* m_pszName;
		const char* m_pszAuthor;
		const char* m_pszDescription;
		const char* m_pszCommand;
		const char* m_pszScreenshot;
		const char* m_pszThumbnail;
	};

	void			LoadChapters();
	CChapterInfo	ParseChapterInfo(KeyValues* pkvKeys);
	void			AnimateFloorInfo();
	void			UpdateChapterInfo(const CChapterInfo& chapterInfo);

	vgui::IScheme*			m_pScheme;
	CCustomUIChapterBar*	m_pChapterBar;
	CUtlLinkedList<CChapterInfo>	m_Chapters;
	vgui::Label*			m_pFloorNumberLabel;
	vgui::Label*			m_pFloorNameLabel;
	vgui::Label*			m_pFloorAuthorLabel;
	vgui::Label*			m_pFloorDescriptionLabel;
	vgui::Panel*			m_pFloorDescriptionBackgroundPanel;
	int				m_iSelectedChapter;
	vgui::ImagePanel*		m_pScreenshotImagePanel;
};

#endif // !CUSTOMUI_NEWGAME_H
