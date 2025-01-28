//=============================================================================
//
// Purpose: TWHL Tower: Source options menu settings
//
//=============================================================================

#ifndef TWHLTOWER_OPTIONS_H
#define TWHLTOWER_OPTIONS_H

#include "igamesystem.h"
#include "vgui/IVGui.h"
#include "vgui/VGUI.h"

IGameSystem* TwhlTowerOptions();

//=============================================================================
// CTwhlTowerOptions
//=============================================================================
class CTwhlTowerOptions : public CBaseGameSystemPerFrame
{
public:
	CTwhlTowerOptions();

	const char* Name() OVERRIDE { return "CTwhlTowerOptions"; }

	void Update(float) OVERRIDE;

private:
	vgui::HPanel GetOptionsDialog();
	vgui::HPanel GetOptionsSubVideo();

	vgui::HPanel m_hOptionsDialog;
	vgui::HPanel m_hOptionsSubVideo;
};

#endif // !TWHLTOWER_OPTIONS_H
