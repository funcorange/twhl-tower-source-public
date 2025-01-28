//=============================================================================
//
// Purpose: Entity for querying player disc count
//
//=============================================================================

#ifndef LOGIC_QUERY_DISCS_H
#define LOGIC_QUERY_DISCS_H

#include "baseentity.h"
#include "entityoutput.h"

//=============================================================================
// CLogicQueryDiscs
//=============================================================================
class CLogicQueryDiscs : public CLogicalEntity
{
public:
	DECLARE_CLASS(CLogicQueryDiscs, CLogicalEntity);
	DECLARE_DATADESC();

	CLogicQueryDiscs();

	void InputQueryCollectedDiscs(inputdata_t& inputdata);
	void InputQueryTotalDiscs(inputdata_t& inputdata);

private:
	COutputInt m_OnQueryCollectedDiscs;
	COutputInt m_OnQueryTotalDiscs;
};

#endif // !LOGIC_QUERY_DISCS_H
