//=============================================================================
//
// Purpose: TWHL Tower: Source HUD element
//
//=============================================================================

#ifndef TWHLTOWER_HUDELEMENT_H
#define TWHLTOWER_HUDELEMENT_H

#include "hudelement.h"

class CTWHLTowerHudElement : public CHudElement
{
public:
	DECLARE_CLASS(CTWHLTowerHudElement, CHudElement);

	CTWHLTowerHudElement(const char* pszElementName);
	virtual ~CTWHLTowerHudElement();

	virtual void OnRestore()
	{
		Reset();
	};

	DECLARE_DATADESC();
};

#endif // !TWHLTOWER_HUDELEMENT_H
