//=============================================================================
//
// Purpose: CvarToggleCheckButton that changes its cvar immediately without
//			waiting for an ApplyChanges signal.
//
//=============================================================================

#include "cbase.h"
#include "cvar_toggle_check_button_immediate.h"

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CvarToggleCheckButtonImmediate::CvarToggleCheckButtonImmediate(
	vgui::Panel* pParent,
	const char* pszPanelName,
	const char* pszText,
	char const* pszCvarname,
	bool bIgnoreMissingCvar
) :
	BaseClass(
		pParent,
		pszPanelName,
		pszText,
		pszCvarname,
		bIgnoreMissingCvar
	)
{
}


//-----------------------------------------------------------------------------
// Purpose: Called when checkbox changes state
//-----------------------------------------------------------------------------
void CvarToggleCheckButtonImmediate::SetSelected(bool bState)
{
	BaseClass::SetSelected(bState);
	ApplyChanges();
}

