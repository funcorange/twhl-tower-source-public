//=============================================================================
//
// Purpose: TWHL Tower: Source HUD save/restore
//
//=============================================================================

#ifndef TWHLTOWER_HUD_SAVERESTORE_H
#define TWHLTOWER_HUD_SAVERESTORE_H

#include "isaverestore.h"

ISaveRestoreBlockHandler* GetTWHLTowerHudSaveRestoreBlockHandler();

//=============================================================================
// CTWHLTowerHudSaveRestoreBlockHandler
//=============================================================================
class CTWHLTowerHudSaveRestoreBlockHandler : public CDefSaveRestoreBlockHandler
{
public:
	static const short s_sTWHLTowerHudSaveRestoreVersion;

	CTWHLTowerHudSaveRestoreBlockHandler();

	const char*	GetBlockName() OVERRIDE;

	void		Save(ISave* pSave) OVERRIDE;
	void		WriteSaveHeaders(ISave* pSave) OVERRIDE;

	void		ReadRestoreHeaders(IRestore* pRestore) OVERRIDE;
	void		Restore(IRestore* pRestore, bool bCreatePlayers) OVERRIDE;

private:
	bool		m_bDoLoad;
};

#endif // !TWHLTOWER_HUD_SAVERESTORE_H
