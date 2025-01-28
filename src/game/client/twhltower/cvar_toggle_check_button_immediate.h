//=============================================================================
//
// Purpose: CvarToggleCheckButton that changes its cvar immediately without
//			waiting for an ApplyChanges signal.
//
//=============================================================================

#ifndef CVAR_TOGGLE_CHECK_BUTTON_IMMEDIATE_H
#define CVAR_TOGGLE_CHECK_BUTTON_IMMEDIATE_H

#include "vgui_controls/cvartogglecheckbutton.h"

class CvarToggleCheckButtonImmediate :
	public vgui::CvarToggleCheckButton<ConVarRef>
{
public:
	DECLARE_CLASS_SIMPLE(
		CvarToggleCheckButtonImmediate,
		CvarToggleCheckButton<ConVarRef>
	);

	CvarToggleCheckButtonImmediate(
		vgui::Panel* pParent,
		const char* pszPanelName,
		const char* pszText = "",
		char const* pszCvarname = NULL,
		bool bIgnoreMissingCvar = false
	);

	void SetSelected(bool bState) OVERRIDE;
};

#endif // CVAR_TOGGLE_CHECK_BUTTON_IMMEDIATE_H
