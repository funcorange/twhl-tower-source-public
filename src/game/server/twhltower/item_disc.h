//==============================================================================
//
// Purpose: TWHL Tower: Source Half-Life 2 DVD collectible
//
//==============================================================================

#ifndef ITEM_DISC_H
#define ITEM_DISC_H

#include "items.h"

class CItemDisc : public CItem
{
public:
	DECLARE_CLASS(CItemDisc, CItem);
	DECLARE_DATADESC();

	void Spawn() override;
	void Precache() override;
	bool MyTouch(CBasePlayer* pPlayer) override;

private:
	string_t m_iszMessage;
};

#endif // !ITEM_DISC_H
