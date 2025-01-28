//==============================================================================
//
// Purpose: Custom UI override root panel
//
//==============================================================================

#ifndef CUSTOMUI_ROOT_H
#define CUSTOMUI_ROOT_H

#include <vgui/IScheme.h>

#include "customui_gamemenu.h"
#include "GameUI/IGameUI.h"
#include "vgui_controls/EditablePanel.h"

class CCustomUIRoot : public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE(CCustomUIRoot, vgui::EditablePanel);

	CCustomUIRoot(vgui::VPANEL parent);
	virtual ~CCustomUIRoot();

	IGameUI*		GetGameUI();
	void			SetCurrentMenu(vgui::Panel* pPanel);

protected:
	virtual void	PerformLayout() override;

private:
	bool			LoadGameUI();

	int				m_iExitingFrameCount;
	bool			m_bCopyFrameBuffer;

	IGameUI*		m_pGameUI;

	// Custom UI elements
	CUtlVector<vgui::Panel*>	m_pElements;
};

#endif // !CUSTOMUI_ROOT_H
