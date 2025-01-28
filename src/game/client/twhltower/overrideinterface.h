//==============================================================================
//
// Purpose: Custom UI override creation interface implementation
//
//==============================================================================

#ifndef OVERRIDEINTERFACE_H
#define OVERRIDEINTERFACE_H

#include <vgui/VGUI.h>

#include "customui_root.h"
#include "GameUI/IGameUI.h"
#include "ioverrideinterface.h"

class COverrideInterface : public IOverrideInterface
{
public:
	COverrideInterface();

	virtual void			Create(vgui::VPANEL parent) override;
	virtual vgui::VPANEL	GetPanel() override;
	virtual void			Destroy() override;
	virtual IGameUI*		GetGameUI() override;
	virtual void			SetCurrentMenu(vgui::Panel* pPanel) override;

private:
	CCustomUIRoot*	m_pMainMenu;
};

#endif // !OVERRIDEINTERFACE_H
