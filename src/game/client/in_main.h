//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef IN_MAIN_H
#define IN_MAIN_H
#ifdef _WIN32
#pragma once
#endif


#include "kbutton.h"


extern kbutton_t in_commandermousemove;
extern kbutton_t in_ducktoggle;

extern bool		g_bIsForwardSpeedOverriden;
extern bool		g_bIsSideSpeedOverriden;
extern bool		g_bIsBackSpeedOverriden;
extern float	g_flForwardSpeedOverride;
extern float	g_flSideSpeedOverride;
extern float	g_flBackSpeedOverride;

float GetSideSpeed();
float GetBackSpeed();
float GetForwardSpeed();

#endif // IN_MAIN_H
