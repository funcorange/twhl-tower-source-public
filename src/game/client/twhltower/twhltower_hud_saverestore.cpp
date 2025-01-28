//=============================================================================
//
// Purpose: TWHL Tower: Source HUD save/restore
//
//=============================================================================

#include "cbase.h"
#include "twhltower_hud_saverestore.h"

#include "hud.h"
#include "twhltower_hudelement.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static CTWHLTowerHudSaveRestoreBlockHandler
	g_TWHLTowerHudSaveRestoreBlockHandler;

ISaveRestoreBlockHandler* GetTWHLTowerHudSaveRestoreBlockHandler()
{
	return &g_TWHLTowerHudSaveRestoreBlockHandler;
}

//=============================================================================
// 
// CTWHLTowerHudSaveRestoreBlockHandler
// 
//=============================================================================

const short CTWHLTowerHudSaveRestoreBlockHandler::
	s_sTWHLTowerHudSaveRestoreVersion =
		1;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTWHLTowerHudSaveRestoreBlockHandler::CTWHLTowerHudSaveRestoreBlockHandler() :
	m_bDoLoad(false),
	CDefSaveRestoreBlockHandler()
{
}


//-----------------------------------------------------------------------------
// Purpose: Get save/restore block name
//-----------------------------------------------------------------------------
const char* CTWHLTowerHudSaveRestoreBlockHandler::GetBlockName()
{
	return "TWHLTowerHud";
}


//-----------------------------------------------------------------------------
// Purpose: Save
//-----------------------------------------------------------------------------
void CTWHLTowerHudSaveRestoreBlockHandler::Save(ISave* pSave)
{
	CUtlVector<CTWHLTowerHudElement*> vElements;
	for (auto i = gHUD.m_HudList.begin(); i != gHUD.m_HudList.end(); ++i)
	{
		CTWHLTowerHudElement* pElement =
			dynamic_cast<CTWHLTowerHudElement*>(*i);

		if (!pElement)
			continue;

		vElements.AddToTail(pElement);
	}

	const short nSaveCount = static_cast<short>(vElements.Count());
	pSave->WriteShort(&nSaveCount);

	if (nSaveCount <= 0)
		return;

	for (auto i = vElements.begin(); i != vElements.end(); ++i)
	{
		pSave->StartBlock();
		pSave->WriteString((*i)->GetName());
		pSave->WriteAll(
			static_cast<void*>(*i),
			(*i)->GetDataDescMap()
		);
		pSave->EndBlock();
	}
}


//-----------------------------------------------------------------------------
// Purpose: Write save headers
//-----------------------------------------------------------------------------
void CTWHLTowerHudSaveRestoreBlockHandler::WriteSaveHeaders(ISave* pSave)
{
	pSave->WriteShort(&s_sTWHLTowerHudSaveRestoreVersion);
}


//-----------------------------------------------------------------------------
// Purpose: Read restore headers
//-----------------------------------------------------------------------------
void CTWHLTowerHudSaveRestoreBlockHandler::ReadRestoreHeaders(
	IRestore* pRestore
)
{
	short version = 0;
	pRestore->ReadShort(&version);

	m_bDoLoad = (version == s_sTWHLTowerHudSaveRestoreVersion);
}


//-----------------------------------------------------------------------------
// Purpose: Restore
//-----------------------------------------------------------------------------
void CTWHLTowerHudSaveRestoreBlockHandler::Restore(
	IRestore* pRestore,
	bool
)
{
	if (!m_bDoLoad)
		return;

	short cRestoreCount = 0;
	pRestore->ReadShort(&cRestoreCount);

	if (cRestoreCount <= 0)
		return;

	for (short i = 0; i < cRestoreCount; ++i)
	{
		pRestore->StartBlock();

		char szElementName[128] = {0};
		pRestore->ReadString(szElementName, sizeof(szElementName), 0);

		CTWHLTowerHudElement* pElement = dynamic_cast<CTWHLTowerHudElement*>(
			gHUD.FindElement(szElementName)
		);

		if (pElement)
		{
			pRestore->ReadAll(
				static_cast<void*>(pElement),
				pElement->GetDataDescMap()
			);

			pElement->OnRestore();
		}

		pRestore->EndBlock();
	}
}

