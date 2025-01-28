//==============================================================================
//
// Purpose: Custom UI override creation interface
//
//==============================================================================

#ifndef IOVERRIDEINTERFACE_H
#define IOVERRIDEINTERFACE_H

#include <vgui/VGUI.h>

#include "GameUI/IGameUI.h"

abstract_class IOverrideInterface
{
public:
	virtual void			Create(vgui::VPANEL parent) = 0;
	virtual vgui::VPANEL	GetPanel() = 0;
	virtual void			Destroy() = 0;
	virtual IGameUI*		GetGameUI() = 0;
	virtual void			SetCurrentMenu(vgui::Panel* pPanel) = 0;
};

extern IOverrideInterface* g_pOverrideUI;

#endif // !IOVERRIDEINTERFACE_H
