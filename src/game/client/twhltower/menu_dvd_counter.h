//=============================================================================
//
// Purpose: Pause menu DVD counter
//
//=============================================================================

#ifndef MENU_DVD_COUNTER_H
#define MENU_DVD_COUNTER_H

#include <vgui/IScheme.h>

#include "../shared/twhltower/menu_dvd_counter_state.h"
#include "twhltower_hudelement.h"
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/ImagePanel.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/Panel.h"

class CMenuDvdCounter : public CTWHLTowerHudElement, public vgui::EditablePanel
{
public:
	DECLARE_CLASS(CMenuDvdCounter, CTWHLTowerHudElement);

	CMenuDvdCounter(vgui::Panel* pParent, const char* pszElementName);
	virtual ~CMenuDvdCounter() = default;
	void ApplySchemeSettings(vgui::IScheme* pScheme) OVERRIDE;
	void PerformLayout() OVERRIDE;
	void SetTotal(int iTotal);
	inline int GetTotal() const { return m_iTotal; }
	void SetCount(int iCount);
	inline int GetCount() const { return m_iCount; }
	void SetCurrentFloorDiscFound(bool bFound);
	void SetEnabled(bool bEnabled) OVERRIDE;
	void SetVisible(bool bVisible) OVERRIDE;
	void SetState(MenuDvdCounterState_e state);

	DECLARE_DATADESC();
private:
	void UpdateEnabled();
	void UpdateVisible();
	vgui::Panel* m_pBackgroundPanel;
	vgui::ImagePanel* m_pDvdImage;
	vgui::Label* m_pTotalLabel;
	vgui::Label* m_pCounterLabel;
	vgui::Label* m_pThisFloorLabel;
	vgui::Label* m_pFoundLabel;
	vgui::HFont m_hLabelFont;
	int m_iTotal;
	int m_iCount;
	bool m_bCurrentFloorDiscFound;
	int m_iState;
	bool m_bEnabled;
	bool m_bVisible;
};

void CreateMenuDvdCounter(vgui::VPANEL parent, const char* pszElementName);
void DestroyMenuDvdCounter();
void UpdateMenuDvdCounter();

extern CMenuDvdCounter* g_pMenuDvdCounter;

#endif // !MENU_DVD_COUNTER_H
