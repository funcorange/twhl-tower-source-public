//==============================================================================
//
// Purpose: Custom UI new game dialog
//
//==============================================================================

#include "cbase.h"
#include "customui_newgame.h"

#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>

#include "filesystem.h"
#include "iclientmode.h"
#include "ioverrideinterface.h"
#include "vgui_controls/AnimationController.h"

static CCustomUINewGame* g_pNewGame;

CCustomUINewGame::CCustomUINewGame(vgui::Panel* pParent) :
	m_pScheme(NULL),
	m_pChapterBar(NULL),
	m_Chapters(),
	m_pFloorNumberLabel(NULL),
	m_pFloorNameLabel(NULL),
	m_pFloorAuthorLabel(NULL),
	m_pFloorDescriptionLabel(NULL),
	m_pFloorDescriptionBackgroundPanel(NULL),
	m_iSelectedChapter(0),
	BaseClass(pParent, "CustomUINewGame")
{
	g_pNewGame = this;

	SetProportional(true);
	LoadControlSettings("resource/UI/custom/CustomUINewGame.res");

	m_pChapterBar = FindControl<CCustomUIChapterBar>("ChapterBar");
	m_pFloorNumberLabel = FindControl<vgui::Label>("FloorNumberTitle");
	m_pFloorNameLabel = FindControl<vgui::Label>("FloorNameTitle");
	m_pFloorAuthorLabel = FindControl<vgui::Label>("FloorAuthorTitle");
	m_pFloorDescriptionLabel = FindControl<vgui::Label>("FloorDescriptionTitle");
	m_pFloorDescriptionBackgroundPanel = FindControl<vgui::Panel>("FloorDescriptionBackgroundPanel");
	m_pScreenshotImagePanel = FindControl<vgui::ImagePanel>("ScreenshotPanel");

	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);

	LoadChapters();

	int iChapterCount = m_Chapters.Count();
	if (m_pChapterBar != NULL)
	{
		for (int i = 0; i < iChapterCount; i++)
		{
			const CChapterInfo& chapterInfo = m_Chapters.Element(i);

			wchar_t wcsChapterNumberString[256];
			mbstowcs(wcsChapterNumberString, chapterInfo.GetNumber(), ARRAYSIZE(wcsChapterNumberString));

			m_pChapterBar->AddChapter(i + 1, wcsChapterNumberString, chapterInfo.GetThumbnail());
		}

		m_pChapterBar->OnChapterSelected(1);

		ConVarRef unlockedChapters("sv_unlockedchapters");
		m_pChapterBar->SetUnlockedChapters(unlockedChapters.GetInt());
	}

	if (iChapterCount > 0)
		m_iSelectedChapter = 1;

	MakePopup();
}


CCustomUINewGame::~CCustomUINewGame()
{
}


void CCustomUINewGame::PerformLayout()
{
	BaseClass::PerformLayout();
}


void CCustomUINewGame::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	m_pScheme = pScheme;
}


void CCustomUINewGame::StartNewGame()
{
	if (m_iSelectedChapter < 1)
		return;

	if (m_iSelectedChapter > m_Chapters.Count())
		return;

	const CChapterInfo& chapterInfo = m_Chapters.Element(m_iSelectedChapter - 1);
	const char* pszChapterCommand = chapterInfo.GetCommand();
	if (pszChapterCommand == NULL)
		return;

	engine->ClientCmd("customui_show_gamemenu");
	engine->ClientCmd(pszChapterCommand);
}


void CCustomUINewGame::OnShow()
{
	ConVarRef unlockedChapters("sv_unlockedchapters");
	m_pChapterBar->SetUnlockedChapters(unlockedChapters.GetInt());
	m_pChapterBar->OnChapterSelected(1);
}


void CCustomUINewGame::OnChapterSelected(int iChapter)
{
	int iChapterCount = m_Chapters.Count();

	if (iChapter < 1)
		return;

	if (iChapter > iChapterCount)
		return;

	m_iSelectedChapter = iChapter;

	CChapterInfo& chapterInfo = m_Chapters.Element(iChapter - 1);
	UpdateChapterInfo(chapterInfo);

	AnimateFloorInfo();
}


void CCustomUINewGame::OnChapterStart(int iChapter)
{
	m_iSelectedChapter = iChapter;
	engine->ClientCmd("customui_newgame_start");
}


void CCustomUINewGame::LoadChapters()
{
	KeyValues* pkvChaptersData = new KeyValues("Chapters");
	KeyValues::AutoDelete autoDelete(pkvChaptersData);

	if (!pkvChaptersData->LoadFromFile(filesystem, "resource/chapters.res"))
		return;

	m_Chapters.RemoveAll();

	for (KeyValues* pkvSubKey = pkvChaptersData->GetFirstSubKey(); pkvSubKey != NULL; pkvSubKey = pkvSubKey->GetNextKey())
		m_Chapters.AddToTail(ParseChapterInfo(pkvSubKey));
}

CCustomUINewGame::CChapterInfo CCustomUINewGame::ParseChapterInfo(KeyValues* pkvKeys)
{
	const char* pszNumber = NULL;
	const char* pszName = NULL;
	const char* pszAuthor = NULL;
	const char* pszDescription = NULL;
	const char* pszCommand = NULL;
	const char* pszScreenshot = NULL;
	const char* pszThumbnail = NULL;

	pszNumber = pkvKeys->GetName();

	for (KeyValues* pkvValue = pkvKeys->GetFirstValue(); pkvValue != NULL; pkvValue = pkvValue->GetNextValue())
	{
		if (V_strcasecmp(pkvValue->GetName(), "title") == 0)
		{
			pszName = pkvValue->GetString();
			continue;
		}

		if (V_strcasecmp(pkvValue->GetName(), "author") == 0)
		{
			pszAuthor = pkvValue->GetString();
			continue;
		}

		if (V_strcasecmp(pkvValue->GetName(), "description") == 0)
		{
			pszDescription = pkvValue->GetString();
			continue;
		}

		if (V_strcasecmp(pkvValue->GetName(), "command") == 0)
		{
			pszCommand = pkvValue->GetString();
			continue;
		}

		if (V_strcasecmp(pkvValue->GetName(), "screenshot") == 0)
		{
			pszScreenshot = pkvValue->GetString();
			continue;
		}

		if (V_strcasecmp(pkvValue->GetName(), "thumbnail") == 0)
		{
			pszThumbnail = pkvValue->GetString();
			continue;
		}
	}

	return CChapterInfo(pszNumber, pszName, pszAuthor, pszDescription, pszCommand, pszScreenshot, pszThumbnail);
}


void CCustomUINewGame::AnimateFloorInfo()
{
	vgui::Label* rgpLabels[] =
	{
		m_pFloorNumberLabel,
		m_pFloorNameLabel,
		m_pFloorAuthorLabel,
		m_pFloorDescriptionLabel
	};

	for (int i = 0; i < ARRAYSIZE(rgpLabels); i++)
	{
		if (rgpLabels[i] == NULL)
			continue;

		rgpLabels[i]->SetAlpha(0);

		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			rgpLabels[i],
			"alpha",
			255.0f,
			0.0f,
			0.5f,
			vgui::AnimationController::Interpolators_e::INTERPOLATOR_DEACCEL
		);
	}

	if (m_pFloorDescriptionBackgroundPanel)
	{
		m_pFloorDescriptionBackgroundPanel->SetAlpha(0);

		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(
			m_pFloorDescriptionBackgroundPanel,
			"alpha",
			192.0f,
			0.0f,
			0.5f,
			vgui::AnimationController::Interpolators_e::INTERPOLATOR_DEACCEL
		);
	}
}


void CCustomUINewGame::UpdateChapterInfo(const CChapterInfo& chapterInfo)
{
	const wchar_t* pwcszFloorFormat = g_pVGuiLocalize->Find("#twhltower_Floor_Fmt");

	const char* pszNarrowFloorNumber = chapterInfo.GetNumber();
	wchar_t wcszFloorNumber[16];
	wchar_t wcszFloorNumberText[128];
	wchar_t wcszUnlocalizedBuffer[512];	// Buffer for unlocalized wide character strings

	// Convert floor number string to wide character string
	if (pszNarrowFloorNumber != NULL)
	{
		mbstowcs(wcszFloorNumber, pszNarrowFloorNumber, ARRAYSIZE(wcszFloorNumber));
	}
	else
	{
		V_snwprintf(wcszFloorNumber, ARRAYSIZE(wcszFloorNumber), L"??");
	}

	// Format floor number string
	if (pwcszFloorFormat != NULL)
	{
		g_pVGuiLocalize->ConstructString(wcszFloorNumberText, sizeof(wcszFloorNumber), pwcszFloorFormat, 1, wcszFloorNumber);
	}
	else
	{
		V_snwprintf(wcszFloorNumberText, ARRAYSIZE(wcszFloorNumberText), L"Floor %s", wcszFloorNumber);
	}

	SetDialogVariable("floorNumber", wcszFloorNumberText);

	// Localize floor name
	const wchar_t* pwcszLocalizedFloorName = g_pVGuiLocalize->Find(chapterInfo.GetName());
	if (pwcszLocalizedFloorName == NULL)
	{
		mbstowcs(wcszUnlocalizedBuffer, chapterInfo.GetName(), ARRAYSIZE(wcszUnlocalizedBuffer));
		SetDialogVariable("floorName", wcszUnlocalizedBuffer);
	}
	else
	{
		SetDialogVariable("floorName", pwcszLocalizedFloorName);
	}

	// Localize floor author
	const wchar_t* pwcszLocalizedFloorAuthor = g_pVGuiLocalize->Find(chapterInfo.GetAuthor());
	if (pwcszLocalizedFloorAuthor == NULL)
	{
		mbstowcs(wcszUnlocalizedBuffer, chapterInfo.GetAuthor(), ARRAYSIZE(wcszUnlocalizedBuffer));
		SetDialogVariable("floorAuthor", wcszUnlocalizedBuffer);
	}
	else
	{
		SetDialogVariable("floorAuthor", pwcszLocalizedFloorAuthor);
	}

	// Localize floor description
	const wchar_t* pwcszLocalizedFloorDescription = g_pVGuiLocalize->Find(chapterInfo.GetDescription());
	if (pwcszLocalizedFloorDescription == NULL)
	{
		mbstowcs(wcszUnlocalizedBuffer, chapterInfo.GetDescription(), ARRAYSIZE(wcszUnlocalizedBuffer));
		SetDialogVariable("floorDescription", wcszUnlocalizedBuffer);
	}
	else
	{
		SetDialogVariable("floorDescription", pwcszLocalizedFloorDescription);
	}

	// Set screenshot
	if (m_pScreenshotImagePanel)
		m_pScreenshotImagePanel->SetImage(chapterInfo.GetScreenshot());
}


CON_COMMAND_F(customui_show_newgame, "Show new game dialog", FCVAR_CLIENTCMD_CAN_EXECUTE)
{
	if (!g_pNewGame)
		return;

	g_pOverrideUI->SetCurrentMenu(g_pNewGame);
	g_pNewGame->OnShow();
}


CON_COMMAND_F(customui_newgame_start, "Start a new game from the selected chapter in the new game menu", FCVAR_CLIENTCMD_CAN_EXECUTE)
{
	if (!g_pNewGame)
		return;

	g_pNewGame->StartNewGame();
}

