//==============================================================================
//
// Purpose: TWHL Tower: Source user messages
//
//==============================================================================

#include "cbase.h"
#include "twhltower_usermessages.h"

#include "usermessages.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//------------------------------------------------------------------------------
// Purpose: Register TWHL Tower specific user messages
//------------------------------------------------------------------------------
void RegisterTwhlTowerUserMessages()
{
	usermessages->Register("DvdMessage", -1);
	usermessages->Register("HudCounter", -1);
}

