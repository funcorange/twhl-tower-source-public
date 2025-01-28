//=============================================================================
//
// Purpose: Player for TWHL Tower: Source
//
//=============================================================================

#include "cbase.h"

#include "c_twhltower_player.h"
#include "in_main.h"
#include "in_mouse.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar g_cl_enable_item_glow("cl_enable_item_glow", "1", FCVAR_ARCHIVE | FCVAR_CLIENTDLL, "Enable glow outline effect on items under player crosshair.");

//=============================================================================
// 
// C_TwhlTower_Player
// 
//=============================================================================
IMPLEMENT_CLIENTCLASS_DT(C_TwhlTower_Player, DT_TwhlPlayer, CTwhlTower_Player)
	RecvPropDataTable(RECVINFO_DT(m_TwhlLocal), 0, &REFERENCE_RECV_TABLE(DT_TwhlLocal)),
	RecvPropEHandle(RECVINFO(m_hGlowProxy)),
	RecvPropInt(RECVINFO(m_GlowColor), 0, RecvProxy_IntToColor32),
	RecvPropEHandle(RECVINFO(m_hPotentialUseEntity)),
END_RECV_TABLE()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
C_TwhlTower_Player::C_TwhlTower_Player() :
	m_TwhlLocal(),
	m_hGlowProxy(),
	m_GlowColor(),
	m_hPotentialUseEntity()
{
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
C_TwhlTower_Player::~C_TwhlTower_Player()
{
	DestroyGlowEffect();
}


//-----------------------------------------------------------------------------
// Purpose: Called on data update
//-----------------------------------------------------------------------------
void C_TwhlTower_Player::OnDataChanged(DataUpdateType_t updateType)
{
	if (IsLocalPlayer())
	{
		if (m_TwhlLocal.m_bIsMousePitchOverriden)
		{
			g_bIsMousePitchOverriden	= true;
			g_flMousePitchOverride		= m_TwhlLocal.m_flMousePitchOverride;
		}
		else
		{
			g_bIsMousePitchOverriden	= false;
		}

		if (m_TwhlLocal.m_bIsSideSpeedOverriden)
		{
			g_bIsSideSpeedOverriden		= true;
			g_flSideSpeedOverride		= m_TwhlLocal.m_flSideSpeedOverride;
		}
		else
		{
			g_bIsSideSpeedOverriden		= false;
		}

		if (m_TwhlLocal.m_bIsBackSpeedOverriden)
		{
			g_bIsBackSpeedOverriden		= true;
			g_flBackSpeedOverride		= m_TwhlLocal.m_flBackSpeedOverride;
		}
		else
		{
			g_bIsBackSpeedOverriden		= false;
		}

		if (m_TwhlLocal.m_bIsForwardSpeedOverriden)
		{
			g_bIsForwardSpeedOverriden	= true;
			g_flForwardSpeedOverride	= m_TwhlLocal.m_flForwardSpeedOverride;
		}
		else
		{
			g_bIsForwardSpeedOverriden	= false;
		}
	}

	UpdateGlowEffect();

	BaseClass::OnDataChanged(updateType);
}


//-----------------------------------------------------------------------------
// Purpose: Called every frame
//-----------------------------------------------------------------------------
void C_TwhlTower_Player::PostThink()
{
	bool bEnableGlow = g_cl_enable_item_glow.GetBool();
	if ((!bEnableGlow && m_pGlowEffect) || (bEnableGlow && m_hGlowProxy && !m_pGlowEffect))
		UpdateGlowEffect();

	BaseClass::PostThink();
}


//-----------------------------------------------------------------------------
// Purpose: Update glow effect
//-----------------------------------------------------------------------------
void C_TwhlTower_Player::UpdateGlowEffect()
{
	if (m_pGlowEffect)
		DestroyGlowEffect();

	if (!g_cl_enable_item_glow.GetBool())
		return;

	Vector4D vecColor(m_GlowColor.r, m_GlowColor.g, m_GlowColor.b, m_GlowColor.a);
	for (int i = 0; i < 4; i++)
	{
		if (vecColor[i] == 0.0f)
			continue;

		vecColor[i] /= 255.0f;
	}

	m_pGlowEffect = new CGlowObject(m_hGlowProxy, vecColor.AsVector3D(), vecColor.w, false, true);
}


//-----------------------------------------------------------------------------
// Purpose: Destroy glow effect
//-----------------------------------------------------------------------------
void C_TwhlTower_Player::DestroyGlowEffect()
{
	if (m_pGlowEffect)
	{
		delete m_pGlowEffect;
		m_pGlowEffect = NULL;
	}
}

