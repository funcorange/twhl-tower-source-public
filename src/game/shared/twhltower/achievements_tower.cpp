//=============================================================================
//
// Purpose: TWHL Tower achievements
//
//=============================================================================

#include "cbase.h"

#ifdef GAME_DLL

#include "achievementmgr.h"
#include "ammodef.h"
#include "baseachievement.h"
#include "basehlcombatweapon_shared.h"

#define ACHIEVEMENT_TOWER_FINISH_FLOOR18 151 // ACHIEVEMENT_TF_LAST_ORANGEBOX + 1
#define ACHIEVEMENT_TOWER_BEAT_TUTORIAL 152
#define ACHIEVEMENT_TOWER_REACH_DISC_WITHOUT_TOUCHING_WATER 153
#define ACHIEVEMENT_TOWER_CLEANUP_PLACEHOLDER 154
#define ACHIEVEMENT_TOWER_VIRUS_SCANNERS 155
#define ACHIEVEMENT_TOWER_TELEFRAG 156
#define ACHIEVEMENT_TOWER_DESTROY_ALL_WATERMELONS 157
#define ACHIEVEMENT_TOWER_TROLL_FRIDGE 158
#define ACHIEVEMENT_TOWER_DONT_KILL_HUNTERS 159
#define ACHIEVEMENT_TOWER_DESTROY_LIGHTBULB 160
#define ACHIEVEMENT_TOWER_DEATH_BY_COP 161
#define ACHIEVEMENT_TOWER_ESCORT 162
#define ACHIEVEMENT_TOWER_DESTROY_CRATES 163
#define ACHIEVEMENT_TOWER_BEAT_OFFICES_NOWEAPONS 164
#define ACHIEVEMENT_TOWER_WORKPLACE_ALTERNATE_ROUTE 165
#define ACHIEVEMENT_TOWER_INTERACT_ALL_FLOOR13 166
#define ACHIEVEMENT_TOWER_COLLECT_ALL_DISCS 167
#define ACHIEVEMENT_TOWER_SPEEDRUN_MAZE 168
#define ACHIEVEMENT_TOWER_BLOW_UP_BARRELS 169
#define ACHIEVEMENT_TOWER_TURRET_SPRINT 170
#define ACHIEVEMENT_TOWER_MULTIKILL 171
#define ACHIEVEMENT_TOWER_TALK_TO_ANGEL 172
#define ACHIEVEMENT_TOWER_FINISH_BONUS1 173
#define ACHIEVEMENT_TOWER_FINISH_BONUS3 174
#define ACHIEVEMENT_TOWER_FINISH_BONUS4 175

#define WATERMELON_MODEL_NAME "models/props_junk/watermelon01.mdl"

extern int CalcPlayerAttacks(bool bBulletOnly);

//=============================================================================
// Telefrag
//=============================================================================
class CAchievementTowerTelefrag : public CBaseAchievement
{
	virtual void Init() override
	{
		SetFlags(ACH_SAVE_GLOBAL | ACH_LISTEN_KILL_EVENTS);
		SetMapNameFilter("tower_06_drorange");
		SetInflictorFilter("trigger_hurt");
		SetGoal(1);
	}

	virtual void Event_EntityKilled(CBaseEntity* pVictim, CBaseEntity* pAttacker, CBaseEntity* pInflictor, IGameEvent* event) override
	{
		if (!(pAttacker->NameMatches("portal_4_hurt") || pAttacker->NameMatches("portal_5_hurt")))
			return;

		IncrementCount();
	}
};
DECLARE_ACHIEVEMENT(CAchievementTowerTelefrag, ACHIEVEMENT_TOWER_TELEFRAG, "TOWER_TELEFRAG", 1);

//=============================================================================
// Walking the Plank
//=============================================================================
class CAchievementTowerReachDiscWithoutTouchingWater : public CFailableAchievement
{
protected:
	virtual void Init() override
	{
		SetFlags(ACH_LISTEN_MAP_EVENTS | ACH_SAVE_WITH_GAME);
		SetGoal(1);
	}

	virtual void OnMapEvent(const char* pEventName)
	{
		if (IsAchieved())
			return;

		if (m_bActivated && FStrEq(pEventName, GetFailEventName()))
		{
			m_bActivated = false;
			SetFailed();
		}

		CFailableAchievement::OnMapEvent(pEventName);
	}

	virtual void OnActivationEvent() override
	{
		if (IsAchieved())
			return;

		if (IsFailed())
			m_bFailed = false;

		CFailableAchievement::OnActivationEvent();
	}

	virtual const char* GetActivationEventName() override { return "TOWER_REACH_DISC_WITHOUT_TOUCHING_WATER_START"; }
	virtual const char* GetEvaluationEventName() override { return "TOWER_REACH_DISC_WITHOUT_TOUCHING_WATER_END"; }

private:
	const char* GetFailEventName() { return "TOWER_REACH_DISC_WITHOUT_TOUCHING_WATER_FAIL"; }
};
DECLARE_ACHIEVEMENT(CAchievementTowerReachDiscWithoutTouchingWater, ACHIEVEMENT_TOWER_REACH_DISC_WITHOUT_TOUCHING_WATER, "TOWER_REACH_DISC_WITHOUT_TOUCHING_WATER", 1);

//=============================================================================
// First In Last Out
//=============================================================================
class CAchievementTowerVirusScanners : public CBaseAchievement
{
private:
	static const char* s_rgszComponents[4];

public:
	DECLARE_CLASS(CAchievementTowerVirusScanners, CBaseAchievement);

	virtual void Init() override
	{
		SetFlags(ACH_LISTEN_MAP_EVENTS | ACH_SAVE_WITH_GAME);
		m_bFailed		= false;
		SetGoal(ARRAYSIZE(s_rgszComponents));
	}

	virtual bool ShouldSaveWithGame() override
	{
		return (m_iFlags | ACH_SAVE_WITH_GAME && !IsAchieved());
	}

	virtual void OnMapEvent(const char* pEventName) override
	{
		if (V_strnicmp(pEventName, GetStageEventPrefix(), V_strlen(GetStageEventPrefix())) != 0)
			return;

		if (m_bFailed)
			return;

		if (GetCount() >= ARRAYSIZE(s_rgszComponents))
			return;

		if (!FStrEq(pEventName, s_rgszComponents[GetCount()]))
		{
			if (cc_achievement_debug.GetBool())
				Msg("CAchievementTowerVirusScanners::OnMapEvent, achievement failed\n");

			m_bFailed = true;
			return;
		}

		IncrementCount();
	}

private:
	DECLARE_DATADESC();

	static const char* GetStageEventPrefix() { return "TOWER_VIRUS_SCANNER"; };

	bool m_bFailed;
};
DECLARE_ACHIEVEMENT(CAchievementTowerVirusScanners, ACHIEVEMENT_TOWER_VIRUS_SCANNERS, "TOWER_VIRUS_SCANNERS", 1);

BEGIN_DATADESC(CAchievementTowerVirusScanners)
	DEFINE_FIELD(m_bFailed, FIELD_BOOLEAN),
END_DATADESC()

const char* CAchievementTowerVirusScanners::s_rgszComponents[4] =
{
	"TOWER_VIRUS_SCANNER_04",
	"TOWER_VIRUS_SCANNER_03",
	"TOWER_VIRUS_SCANNER_02",
	"TOWER_VIRUS_SCANNER_01"
};

//=============================================================================
// Watermelons in Hell
//=============================================================================
class CAchievementTowerDestroyAllWatermelons : public CBaseAchievement
{
protected:
	virtual void Init() override
	{
		SetFlags(ACH_SAVE_WITH_GAME);
		SetGoal(69); // nice.
		SetMapNameFilter("tower_14_jinks");
	}

	virtual void ListenForEvents() override
	{
		ListenForGameEvent("break_prop");
	};

	virtual void FireGameEvent_Internal(IGameEvent* event)
	{
		if (!FStrEq(event->GetName(), "break_prop"))
			return;
		
		int iEntIndex = event->GetInt("entindex");
		edict_t* pEdict = engine->PEntityOfEntIndex(iEntIndex);
		if (!pEdict)
			return;
		
		IServerNetworkable* pNetworkable = pEdict->GetNetworkable();
		if (!pNetworkable)
			return;

		CBaseEntity* pEnt = pNetworkable->GetBaseEntity();
		if (!pEnt)
			return;

		string_t iszModel = pEnt->GetModelName();
		if (!FStrEq(STRING(iszModel), WATERMELON_MODEL_NAME))
			return;

		IncrementCount();
	}
};
DECLARE_ACHIEVEMENT(CAchievementTowerDestroyAllWatermelons, ACHIEVEMENT_TOWER_DESTROY_ALL_WATERMELONS, "TOWER_DESTROY_ALL_WATERMELONS", 1);

//=============================================================================
// Pacifist Prey
//=============================================================================
class CAchievementTowerDontKillHunters : public CFailableAchievement
{
protected:
	void Init() override
	{
		SetFlags(ACH_LISTEN_MAP_EVENTS | ACH_LISTEN_KILL_EVENTS | ACH_SAVE_WITH_GAME);
		SetGoal(1);
		SetVictimFilter("npc_hunter");
	}

	virtual const char* GetActivationEventName() override { return "TOWER_DONT_KILL_HUNTERS_START"; }
	virtual const char* GetEvaluationEventName() override { return "TOWER_DONT_KILL_HUNTERS_END"; }

	virtual void Event_EntityKilled(CBaseEntity* pVictim, CBaseEntity* pAttacker, CBaseEntity* pInflictor, IGameEvent* event) override
	{
		if (IsAchieved())
			return;

		SetFailed();
	}
};
DECLARE_ACHIEVEMENT(CAchievementTowerDontKillHunters, ACHIEVEMENT_TOWER_DONT_KILL_HUNTERS, "TOWER_DONT_KILL_HUNTERS", 1);

//=============================================================================
// Capital Malcompliance
//=============================================================================
class CAchievementTowerDeathByCop : public CBaseAchievement
{
protected:
	void Init() override
	{
		SetFlags(ACH_LISTEN_KILL_EVENTS | ACH_SAVE_GLOBAL);
		SetGoal(1);
		SetVictimFilter("player");
		SetInflictorFilter("npc_metropolice");
		SetMapNameFilter("tower_09_q0");
	}

	virtual void Event_EntityKilled(CBaseEntity* pVictim, CBaseEntity* pAttacker, CBaseEntity* pInflictor, IGameEvent* event) override
	{
		AwardAchievement();
	}
};
DECLARE_ACHIEVEMENT(CAchievementTowerDeathByCop, ACHIEVEMENT_TOWER_DEATH_BY_COP, "TOWER_DEATH_BY_COP", 1);

//=============================================================================
// Escort
//=============================================================================
class CAchievementTowerEscort : public CFailableAchievement
{
protected:
	void Init() override
	{
		SetFlags(ACH_LISTEN_KILL_EVENTS | ACH_LISTEN_MAP_EVENTS | ACH_SAVE_WITH_GAME);
		SetGoal(1);
		SetVictimFilter("npc_citizen");
	}

	virtual const char* GetActivationEventName() override { return "TOWER_ESCORT_START"; }
	virtual const char* GetEvaluationEventName() override { return "TOWER_ESCORT_END"; }

	virtual void Event_EntityKilled(CBaseEntity* pVictim, CBaseEntity* pAttacker, CBaseEntity* pInflictor, IGameEvent* event) override
	{
		if (IsAchieved())
			return;

		SetFailed();
	}
};
DECLARE_ACHIEVEMENT(CAchievementTowerEscort, ACHIEVEMENT_TOWER_ESCORT, "TOWER_ESCORT", 1);

//=============================================================================
// Destroy crates
//=============================================================================
class CAchievementTowerDestroyCrates : public CBaseAchievement
{
protected:
	void Init() override
	{
		SetFlags(ACH_SAVE_WITH_GAME);
		SetGoal(32);
		SetMapNameFilter("tower_15_shoes");
	}

	virtual void ListenForEvents() override
	{
		ListenForGameEvent("break_item_crate");
	}

	virtual void FireGameEvent_Internal(IGameEvent* pEvent) override
	{
		if (!FStrEq(pEvent->GetName(), "break_item_crate"))
			return;

		IncrementCount();
	}
};
DECLARE_ACHIEVEMENT(CAchievementTowerDestroyCrates, ACHIEVEMENT_TOWER_DESTROY_CRATES, "TOWER_DESTROY_CRATES", 1);

//=============================================================================
// Beat offices without weapons
//=============================================================================
class CAchievementTowerBeatOfficesNoWeapons : public CFailableAchievement
{
public:
	DECLARE_CLASS(CAchievementTowerBeatOfficesNoWeapons, CFailableAchievement);
	DECLARE_DATADESC();

protected:
	void Init() OVERRIDE
	{
		SetFlags(ACH_LISTEN_MAP_EVENTS | ACH_SAVE_WITH_GAME);
		SetGoal(1);
		m_iInitialAttackCount = 0;
	}

	const char* GetActivationEventName() OVERRIDE
	{
		return "TOWER_BEAT_OFFICES_NOWEAPONS_START";
	}

	const char* GetEvaluationEventName() OVERRIDE
	{
		return "TOWER_BEAT_OFFICES_NOWEAPONS_END";
	}

	void PreRestoreSavedGame() OVERRIDE
	{
		m_iInitialAttackCount = 0;
		BaseClass::PreRestoreSavedGame();
	}

	void OnActivationEvent() OVERRIDE
	{
		m_iInitialAttackCount = CalcPlayerAttacks(false);
		BaseClass::OnActivationEvent();
	}

	void OnEvaluationEvent() OVERRIDE
	{
		if (IsAchieved())
			return;

		const int iCurrentAttackCount = CalcPlayerAttacks(false);

		if (iCurrentAttackCount > m_iInitialAttackCount)
		{
			SetFailed();
		}

		BaseClass::OnEvaluationEvent();
	}

private:
	int	m_iInitialAttackCount;
};
DECLARE_ACHIEVEMENT(CAchievementTowerBeatOfficesNoWeapons, ACHIEVEMENT_TOWER_BEAT_OFFICES_NOWEAPONS, "TOWER_BEAT_OFFICES_NOWEAPONS", 1);

BEGIN_DATADESC(CAchievementTowerBeatOfficesNoWeapons)
	DEFINE_FIELD(m_iInitialAttackCount, FIELD_INTEGER),
END_DATADESC()

//=============================================================================
// Interact with all objects on Floor 13
//=============================================================================
class CAchievementTowerInteractAllFloor13 : public CBaseAchievement
{
public:
	void Init() OVERRIDE
	{
		static const char* rgpszComponentNames[] = {
		   "TOWER_FLOOR13_INTERACT_WINDOW",
		   "TOWER_FLOOR13_INTERACT_BOOKSHELF",
		   "TOWER_FLOOR13_INTERACT_COMPUTER",
		   "TOWER_FLOOR13_INTERACT_BATH_TUB",
		   "TOWER_FLOOR13_INTERACT_TOILET",
		   "TOWER_FLOOR13_INTERACT_MIRROR",
		   "TOWER_FLOOR13_INTERACT_POT",
		   "TOWER_FLOOR13_INTERACT_BRANCH",
		   "TOWER_FLOOR13_INTERACT_TV",
		   "TOWER_FLOOR13_INTERACT_SINK",
		   "TOWER_FLOOR13_INTERACT_TABLE",
		   "TOWER_FLOOR13_INTERACT_BOTTLE",
		   "TOWER_FLOOR13_INTERACT_FRIDGE",
		   "TOWER_FLOOR13_INTERACT_ALCOHOL",
		   "TOWER_FLOOR13_INTERACT_FIREPLACE",
		   "TOWER_FLOOR13_INTERACT_LIGHTBULB",
		   "TOWER_FLOOR13_INTERACT_BOOKSHELF2",
		   "TOWER_FLOOR13_INTERACT_NOTE",
		   "TOWER_FLOOR13_INTERACT_KEY",
		};
		SetFlags(ACH_HAS_COMPONENTS | ACH_LISTEN_COMPONENT_EVENTS | ACH_SAVE_GLOBAL);
		m_pszComponentNames = rgpszComponentNames;
		m_iNumComponents = ARRAYSIZE(rgpszComponentNames);
		SetComponentPrefix("TOWER_FLOOR13_INTERACT");
		SetGoal(m_iNumComponents);
	}

	bool ShouldShowProgressNotification() OVERRIDE
	{
		return false;
	}
};
DECLARE_ACHIEVEMENT(CAchievementTowerInteractAllFloor13, ACHIEVEMENT_TOWER_INTERACT_ALL_FLOOR13, "TOWER_INTERACT_ALL_FLOOR13", 1);

//=============================================================================
// Collect all DVDs
//=============================================================================
class CAchievementTowerCollectAllDiscs : public CBaseAchievement
{
public:
	void Init() OVERRIDE
	{
		static const char* rgpszComponentNames[] = {
			"TOWER_COLLECT_ALL_DISCS_01",
			"TOWER_COLLECT_ALL_DISCS_02",
			"TOWER_COLLECT_ALL_DISCS_03",
			"TOWER_COLLECT_ALL_DISCS_04",
			"TOWER_COLLECT_ALL_DISCS_05",
			"TOWER_COLLECT_ALL_DISCS_06",
			"TOWER_COLLECT_ALL_DISCS_07",
			"TOWER_COLLECT_ALL_DISCS_08",
			"TOWER_COLLECT_ALL_DISCS_09",
			"TOWER_COLLECT_ALL_DISCS_10",
			"TOWER_COLLECT_ALL_DISCS_11",
			"TOWER_COLLECT_ALL_DISCS_12",
			"TOWER_COLLECT_ALL_DISCS_13",
			"TOWER_COLLECT_ALL_DISCS_14",
			"TOWER_COLLECT_ALL_DISCS_15",
			"TOWER_COLLECT_ALL_DISCS_16",
			"TOWER_COLLECT_ALL_DISCS_17",
			"TOWER_COLLECT_ALL_DISCS_18",
		};
		SetFlags(ACH_HAS_COMPONENTS | ACH_SAVE_GLOBAL);
		m_pszComponentNames = rgpszComponentNames;
		m_iNumComponents = ARRAYSIZE(rgpszComponentNames);
		SetComponentPrefix("TOWER_COLLECT_ALL_DISCS");
		SetGoal(m_iNumComponents);
	}

	void ListenForEvents() OVERRIDE
	{
		ListenForGameEvent("disc_pickup");
	}

	bool ShouldShowProgressNotification() OVERRIDE
	{
		return false;
	}

protected:
	void FireGameEvent_Internal(IGameEvent* pEvent) OVERRIDE
	{
		if (!FStrEq(pEvent->GetName(), "disc_pickup"))
			return;

		char mapBuf[3] = { 0 };
		if (V_strlen(m_pAchievementMgr->GetMapName()) >= 6)
		{
			V_strncpy(
				mapBuf,
				m_pAchievementMgr->GetMapName() + 6,
				ARRAYSIZE(mapBuf)
			);
		}

		char buf[32] = {0};
		V_snprintf(buf, ARRAYSIZE(buf), "TOWER_COLLECT_ALL_DISCS_%s", mapBuf);

		OnComponentEvent(buf);
	}
};
DECLARE_ACHIEVEMENT(CAchievementTowerCollectAllDiscs, ACHIEVEMENT_TOWER_COLLECT_ALL_DISCS, "TOWER_COLLECT_ALL_DISCS", 1);

//=============================================================================
// Speedrun maze
//=============================================================================
class CAchievementTowerSpeedrunMaze : public CFailableAchievement
{
public:
	DECLARE_CLASS(CAchievementTowerSpeedrunMaze, CFailableAchievement);

	static float GetGoalTime()
	{
		return 70.0F;	// 1 minute, 10 seconds
	}

	void Init() OVERRIDE
	{
		SetFlags(ACH_LISTEN_MAP_EVENTS | ACH_SAVE_WITH_GAME);
		SetGoal(1);
		m_flStartTime = 0.0F;
	}

	const char* GetActivationEventName() OVERRIDE
	{
		return "TOWER_MAZE_START";
	}

	const char* GetEvaluationEventName() OVERRIDE
	{
		return "TOWER_MAZE_END";
	}

	void PreRestoreSavedGame() OVERRIDE
	{
		m_flStartTime = 0.0F;
		BaseClass::PreRestoreSavedGame();
	}

	void OnActivationEvent() OVERRIDE
	{
		m_flStartTime = gpGlobals->curtime;
		BaseClass::OnActivationEvent();
	}

	void OnEvaluationEvent() OVERRIDE
	{
		if (IsAchieved())
			return;

		if (gpGlobals->curtime > m_flStartTime + GetGoalTime())
			SetFailed();

		BaseClass::OnEvaluationEvent();
	}

	void PrintAdditionalStatus() OVERRIDE
	{
		if (IsActive())
		{
			Msg(
				"Time since start: %2.2f seconds  Goal time: %2.2f seconds",
				gpGlobals->curtime - m_flStartTime,
				GetGoalTime()
			);
		}
	}

	DECLARE_DATADESC();

private:
	float m_flStartTime;
};
DECLARE_ACHIEVEMENT(CAchievementTowerSpeedrunMaze, ACHIEVEMENT_TOWER_SPEEDRUN_MAZE, "TOWER_SPEEDRUN_MAZE", 1);

BEGIN_DATADESC(CAchievementTowerSpeedrunMaze)
	DEFINE_FIELD(m_flStartTime, FIELD_TIME),
END_DATADESC()

//=============================================================================
// Blow up barrels on floor 16
//=============================================================================
class CAchievementTowerBlowUpBarrels : public CBaseAchievement
{
public:
	void Init() OVERRIDE
	{
		SetFlags(ACH_SAVE_WITH_GAME);
		SetGoal(300);
		SetMapNameFilter("tower_16_skorly");
	}

	void ListenForEvents() OVERRIDE
	{
		ListenForGameEvent("break_prop");
	}

	void FireGameEvent_Internal(IGameEvent* event) OVERRIDE
	{
		if (!FStrEq(event->GetName(), "break_prop"))
			return;

		int iEntIndex = event->GetInt("entindex");
		CBaseEntity* pEnt = UTIL_EntityByIndex(iEntIndex);
		if (!pEnt)
			return;

		string_t iszModel = pEnt->GetModelName();
		if (V_stricmp(STRING(iszModel), "models/props_c17/oildrum001_explosive.mdl") != 0)
			return;

		IncrementCount();
	}
};
DECLARE_ACHIEVEMENT(CAchievementTowerBlowUpBarrels, ACHIEVEMENT_TOWER_BLOW_UP_BARRELS, "TOWER_BLOW_UP_BARRELS", 1);

//=============================================================================
// Run past the turrets on floor 1
//=============================================================================
class CAchievementTowerTurretSprint : public CFailableAchievement
{
protected:
	DECLARE_CLASS(CAchievementTowerTurretSprint, CFailableAchievement);

	static float GetGoalTime()
	{
		return 20.0F;
	}

	void Init() OVERRIDE
	{
		SetFlags(ACH_LISTEN_MAP_EVENTS | ACH_SAVE_WITH_GAME);
		SetGoal(1);
		m_flStartTime = 0.0F;
	}

	const char* GetActivationEventName() OVERRIDE
	{
		return "TOWER_TURRET_SPRINT_START";
	}

	const char* GetEvaluationEventName() OVERRIDE
	{
		return "TOWER_TURRET_SPRINT_END";
	}

	void PreRestoreSavedGame() OVERRIDE
	{
		m_flStartTime = 0.0F;
		BaseClass::PreRestoreSavedGame();
	}

	void OnActivationEvent() OVERRIDE
	{
		m_flStartTime = gpGlobals->curtime;
		BaseClass::OnActivationEvent();
	}

	void OnEvaluationEvent() OVERRIDE
	{
		if (IsAchieved())
			return;

		if (gpGlobals->curtime > m_flStartTime + GetGoalTime())
			SetFailed();

		BaseClass::OnEvaluationEvent();
	}

	void PrintAdditionalStatus() OVERRIDE
	{
		if (IsActive())
		{
			Msg(
				"Time since start: %2.2f seconds  Goal time: %2.2f seconds",
				gpGlobals->curtime - m_flStartTime,
				GetGoalTime()
			);
		}
	}

	void ListenForEvents() OVERRIDE
	{
		ListenForGameEvent("entity_damaged");
	}

	void FireGameEvent_Internal(IGameEvent* event) OVERRIDE
	{
		if (!FStrEq(event->GetName(), "entity_damaged"))
			return;

		if (!FStrEq(event->GetString("classname_damaged"), "player"))
			return;

		SetFailed();
	}

	DECLARE_DATADESC();

private:
	float	m_flStartTime;
};
DECLARE_ACHIEVEMENT(CAchievementTowerTurretSprint, ACHIEVEMENT_TOWER_TURRET_SPRINT, "TOWER_TURRET_SPRINT", 1);

BEGIN_DATADESC(CAchievementTowerTurretSprint)
	DEFINE_FIELD(m_flStartTime, FIELD_TIME),
END_DATADESC()

//=============================================================================
// Kill multiple enemies with one attack on floor 4
//=============================================================================
class CAchievementTowerMultikill : public CBaseAchievement
{
protected:
	DECLARE_CLASS(CAchievementTowerMultikill, CBaseAchievement);

	void Init() OVERRIDE
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME);
		SetGoal(1);
		SetMapNameFilter("tower_04_tmob");
		m_hLastInflictor = NULL;
		m_iLocalCount = 0;
	}

	void Event_EntityKilled(CBaseEntity* pVictim, CBaseEntity* pAttacker, CBaseEntity* pInflictor, IGameEvent* pEvent) OVERRIDE
	{
		const bool bIsEnergyBall = pInflictor->ClassMatches("prop_combine_ball");
		const bool bIsBlastDamage = pEvent->GetInt("damagebits") & DMG_BLAST;

		if (!bIsEnergyBall && !bIsBlastDamage)
			return;

		if (m_hLastInflictor == NULL || m_hLastInflictor != pInflictor)
		{
			m_iLocalCount = 1;
			m_hLastInflictor = pInflictor;
			return;
		}

		m_iLocalCount++;

		if (m_iLocalCount == 3)
		{
			IncrementCount();
		}
	}

	DECLARE_DATADESC();

private:
	EHANDLE	m_hLastInflictor;
	int		m_iLocalCount;
};
DECLARE_ACHIEVEMENT(CAchievementTowerMultikill, ACHIEVEMENT_TOWER_MULTIKILL, "TOWER_MULTIKILL", 1);

BEGIN_DATADESC(CAchievementTowerMultikill)
	DEFINE_FIELD(m_hLastInflictor, FIELD_EHANDLE),
	DEFINE_FIELD(m_iLocalCount, FIELD_INTEGER),
END_DATADESC()

//=============================================================================
// Talk to Angel three times on floor 17
//=============================================================================
class CAchievementTowerTalkToAngel : public CBaseAchievement
{
protected:
	void Init() OVERRIDE
	{
		static const char* rgpszComponentNames[] = {
			"TOWER_TALK_TO_ANGEL_1",
			"TOWER_TALK_TO_ANGEL_2",
			"TOWER_TALK_TO_ANGEL_3",
		};
		SetFlags(ACH_HAS_COMPONENTS | ACH_LISTEN_COMPONENT_EVENTS | ACH_SAVE_WITH_GAME);
		m_pszComponentNames = rgpszComponentNames;
		m_iNumComponents = ARRAYSIZE(rgpszComponentNames);
		SetComponentPrefix("TOWER_TALK_TO_ANGEL");
		SetGoal(m_iNumComponents);
	}

	bool ShouldShowProgressNotification() OVERRIDE
	{
		return true;
	}
};
DECLARE_ACHIEVEMENT(CAchievementTowerTalkToAngel, ACHIEVEMENT_TOWER_TALK_TO_ANGEL, "TOWER_TALK_TO_ANGEL", 1);

//=============================================================================
// Map event achievements
//=============================================================================
DECLARE_MAP_EVENT_ACHIEVEMENT_HIDDEN(ACHIEVEMENT_TOWER_BEAT_TUTORIAL, "TOWER_BEAT_TUTORIAL", 1);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TOWER_TROLL_FRIDGE, "TOWER_TROLL_FRIDGE", 1);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TOWER_DESTROY_LIGHTBULB, "TOWER_DESTROY_LIGHTBULB", 1);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TOWER_WORKPLACE_ALTERNATE_ROUTE, "TOWER_WORKPLACE_ALTERNATE_ROUTE", 1);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TOWER_FINISH_BONUS1, "TOWER_FINISH_BONUS1", 1);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TOWER_FINISH_BONUS3, "TOWER_FINISH_BONUS3", 1);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TOWER_FINISH_BONUS4, "TOWER_FINISH_BONUS4", 1);
DECLARE_MAP_EVENT_ACHIEVEMENT_HIDDEN(ACHIEVEMENT_TOWER_FINISH_FLOOR18, "TOWER_FINISH_FLOOR18", 1);

#endif // GAME_DLL
