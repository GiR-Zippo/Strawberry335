/*
* Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Escort_TribunalOfTheAges.h"
#include "Boss_TribunalOfTheAges.h"
typedef mob_tribuna_controller::mob_tribuna_controllerAI ControllerAI;
typedef npc_brann_hos::npc_brann_hosAI                   BrannAI;
typedef npc_brann_hos                                    BrannScript;


bool BrannScript::OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
{
    player->PlayerTalkClass->ClearMenus();
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
        case GOSSIP_ACTION_INFO_DEF+2:
        {
            player->CLOSE_GOSSIP_MENU();
            creature->AI()->DoAction(ACTION_BRANN_START_ESCORT);
        }break;

        #ifdef DEBUG_TRIBUNAL_OF_AGES
            // start debug
        case GOSSIP_ACTION_INFO_DEF+3:
        {
            player->CLOSE_GOSSIP_MENU();

            debugMode = true;
            HomePosition = creature->GetHomePosition();
            creature->NearTeleportTo(TribunalDebugPos);

            if (debugController = Creature::GetCreature((*creature), creature->GetInstanceScript()->GetData64(DATA_TRIBUNAL_CONTROLLER)))
                creature->MonsterSay("[DEBUG] Controller erfolgreich geladen", LANG_UNIVERSAL, 0);
            else
            {
                creature->MonsterSay("[DEBUG] Controller konnte nicht geladen werden", LANG_UNIVERSAL, 0);
                return true;
            }

            if (!debugController->isAlive())
                debugController->Respawn();
        }break;

        // stop debug
        case GOSSIP_ACTION_INFO_DEF+4:
        {
            player->CLOSE_GOSSIP_MENU();

            if (debugController)
            {
                debugMode = false;
                CAST_AI(ControllerAI, debugController->AI())->debugSpells = false;
                OnGossipSelect(player, creature, GOSSIP_SENDER_MAIN, (GOSSIP_ACTION_INFO_DEF*3));
                OnGossipSelect(player, creature, GOSSIP_SENDER_MAIN, (GOSSIP_ACTION_INFO_DEF*3)+1);
                OnGossipSelect(player, creature, GOSSIP_SENDER_MAIN, (GOSSIP_ACTION_INFO_DEF*3)+2);
                creature->MonsterSay("[DEBUG] Modus wurde erfolgreich gestoppt", LANG_UNIVERSAL, 0);
                creature->NearTeleportTo(HomePosition);
            }
            else
                creature->MonsterSay("[DEBUG] Modus konnte nicht gestoppt werden -> debugController ist ungueltig", LANG_UNIVERSAL, 0);
        }break;

        //activate bosses
        case (GOSSIP_ACTION_INFO_DEF*2):   // boss kaddrak
        case (GOSSIP_ACTION_INFO_DEF*2)+1: // boss marnak
        case (GOSSIP_ACTION_INFO_DEF*2)+2: // boss abedneum
        {
            if (debugController)
            {
                switch (action)
                {
                    case (GOSSIP_ACTION_INFO_DEF*2):  { CAST_AI(ControllerAI, debugController->AI())->ActivateBoss(EVENT_KADRAK_CAST_SPELL);   }break;
                    case (GOSSIP_ACTION_INFO_DEF*2)+1:{ CAST_AI(ControllerAI, debugController->AI())->ActivateBoss(EVENT_MARNAK_CAST_SPELL);   }break;
                    case (GOSSIP_ACTION_INFO_DEF*2)+2:{ CAST_AI(ControllerAI, debugController->AI())->ActivateBoss(EVENT_ABEDNEUM_CAST_SPELL); }break;
                }
            }
            else
                creature->MonsterSay("[DEBUG] Kein Controller vorhanden", LANG_UNIVERSAL, 0);
        }break;

        //dactivate bosses
        case (GOSSIP_ACTION_INFO_DEF*3):   // boss kaddrak
        case (GOSSIP_ACTION_INFO_DEF*3)+1: // boss marnak
        case (GOSSIP_ACTION_INFO_DEF*3)+2: // boss abedneum
        {
            if (debugController)
            {
                switch (action)
                {
                    case (GOSSIP_ACTION_INFO_DEF*3):  { CAST_AI(ControllerAI, debugController->AI())->DeactivateBoss(EVENT_KADRAK_CAST_SPELL);   }break;
                    case (GOSSIP_ACTION_INFO_DEF*3)+1:{ CAST_AI(ControllerAI, debugController->AI())->DeactivateBoss(EVENT_MARNAK_CAST_SPELL);   }break;
                    case (GOSSIP_ACTION_INFO_DEF*3)+2:{ CAST_AI(ControllerAI, debugController->AI())->DeactivateBoss(EVENT_ABEDNEUM_CAST_SPELL); }break;
                }
            }
            else
                creature->MonsterSay("[DEBUG] Kein Controller vorhanden", LANG_UNIVERSAL, 0);
        }break;
        #endif
    }

    player->CLOSE_GOSSIP_MENU();
    return true;
};

#ifdef DEBUG_TRIBUNAL_OF_AGES
bool BrannScript::OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code)
{
    player->PlayerTalkClass->ClearMenus();

    switch (action)
    {
        //set spelltimer
        case GOSSIP_ACTION_INFO_DEF*4:     // boss kaddrak
        case (GOSSIP_ACTION_INFO_DEF*4)+1: // boss marnak
        case (GOSSIP_ACTION_INFO_DEF*4)+2: // boss abedneum
        {
            if (debugController)
            {
                uint32 spellTimer = 0;
                if (spellTimer = atoi(code))
                {
                    switch (action)
                    {
                        case GOSSIP_ACTION_INFO_DEF*4:    { CAST_AI(ControllerAI, debugController->AI())->SetNewSpellTimer(EVENT_KADRAK_CAST_SPELL, spellTimer);   }break;
                        case (GOSSIP_ACTION_INFO_DEF*4)+1:{ CAST_AI(ControllerAI, debugController->AI())->SetNewSpellTimer(EVENT_MARNAK_CAST_SPELL, spellTimer);   }break;
                        case (GOSSIP_ACTION_INFO_DEF*4)+2:{ CAST_AI(ControllerAI, debugController->AI())->SetNewSpellTimer(EVENT_ABEDNEUM_CAST_SPELL, spellTimer); }break;
                        default: 
                        {
                            std::string error = "[DEBUG] Falscher Parameter bei SetNewSpellTimer: action = ";
                            error += code;
                            creature->MonsterSay(error.c_str(), LANG_UNIVERSAL, 0);
                        }
                    }
                }
                else
                    creature->MonsterSay("[DEBUG] Konnte Eingabe nicht verarbeiten", LANG_UNIVERSAL, 0);
            }
            else
                creature->MonsterSay("[DEBUG] Kein Controller vorhanden", LANG_UNIVERSAL, 0);
        }break;

        default: 
        {
            std::string error = "[DEBUG] Falscher Parameter im switch: action = ";
            error += code;
            creature->MonsterSay(error.c_str(), LANG_UNIVERSAL, 0);
        }
    }

    player->CLOSE_GOSSIP_MENU();
    return true;
};
#endif

bool BrannScript::OnGossipHello(Player* player, Creature* creature)
{
    uint32 gossipTextId = player->GetGossipTextId(creature);
    uint32 gossipMenuId = Player::GetDefaultGossipMenuForSource(creature);

    #ifdef DEBUG_TRIBUNAL_OF_AGES
    if (creature->isQuestGiver() && !debugMode)
        player->PrepareQuestMenu(creature->GetGUID());

    if (!debugMode)
        player->ADD_GOSSIP_ITEM_DB(gossipMenuId, GOSSIP_BRANN_OPTIONID_START, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    if (player->isGameMaster())
    {
        if (!debugMode)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DEBUG, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
    }

    if (debugMode)
    {
        uint32 bossState = 0;

        if (getBossStates(bossState))
        {
            std::string gossipOptions[6] ={ 
                GOSSIP_ITEM_DEBUG_0, GOSSIP_ITEM_DEBUG_1, GOSSIP_ITEM_DEBUG_2,
                GOSSIP_ITEM_DEBUG_3, GOSSIP_ITEM_DEBUG_4, GOSSIP_ITEM_DEBUG_5    };
            uint32 optionIndex[3] ={ 0, 1, 2 };

            if ((bossState & BOSS_KADDRAK_ACTIVE) == BOSS_KADDRAK_ACTIVE)
            {
                gossipOptions[0] += " [Active]";
                optionIndex[0] += GOSSIP_ACTION_INFO_DEF;
            }

            if ((bossState & BOSS_MARNAK_ACTIVE) == BOSS_MARNAK_ACTIVE)
            {
                gossipOptions[1] += " [Active]";
                optionIndex[1] += GOSSIP_ACTION_INFO_DEF;
            }
            if ((bossState & BOSS_ABEDNEUM_ACTIVE) == BOSS_ABEDNEUM_ACTIVE)
            {
                gossipOptions[2] += " [Active]";
                optionIndex[2] += GOSSIP_ACTION_INFO_DEF;
            }

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_ITEM_DEBUG_TITLE, GOSSIP_SENDER_MAIN, 0);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, GOSSIP_ITEM_DEBUG_LINE, GOSSIP_SENDER_MAIN, 0);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipOptions[0].c_str(), GOSSIP_SENDER_MAIN, (GOSSIP_ACTION_INFO_DEF*2)+optionIndex[0]);
            if ((bossState & BOSS_KADDRAK_ACTIVE) == BOSS_KADDRAK_ACTIVE)
                player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, gossipOptions[3].c_str(), GOSSIP_SENDER_MAIN, (GOSSIP_ACTION_INFO_DEF*4), "", 0, true);

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, GOSSIP_ITEM_DEBUG_LINE, GOSSIP_SENDER_MAIN, 0);

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipOptions[1].c_str(), GOSSIP_SENDER_MAIN, (GOSSIP_ACTION_INFO_DEF*2)+optionIndex[1]);
            if ((bossState & BOSS_MARNAK_ACTIVE) == BOSS_MARNAK_ACTIVE)
                player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, gossipOptions[4].c_str(), GOSSIP_SENDER_MAIN, (GOSSIP_ACTION_INFO_DEF*4)+1, "", 0, true);

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, GOSSIP_ITEM_DEBUG_LINE, GOSSIP_SENDER_MAIN, 0);

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipOptions[2].c_str(), GOSSIP_SENDER_MAIN, (GOSSIP_ACTION_INFO_DEF*2)+optionIndex[2]);
            if ((bossState & BOSS_ABEDNEUM_ACTIVE) == BOSS_ABEDNEUM_ACTIVE)
                player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, gossipOptions[5].c_str(), GOSSIP_SENDER_MAIN, (GOSSIP_ACTION_INFO_DEF*4)+2, "", 0, true);

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, GOSSIP_ITEM_DEBUG_LINE, GOSSIP_SENDER_MAIN, 0);

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RESET, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
        }
        else
        {
            creature->MonsterSay("[DEBUG] Konnte BossStates nicht abrufen -> debugController ist ungueltig", LANG_UNIVERSAL, 0);
            return true;
        }

        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }
    #else
    if (creature->isQuestGiver())
        player->PrepareQuestMenu(creature->GetGUID());

    player->ADD_GOSSIP_ITEM_DB(gossipMenuId, GOSSIP_BRANN_OPTIONID_START, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    #endif

    player->SEND_GOSSIP_MENU(gossipTextId, creature->GetGUID());

    return true;
};

#ifdef DEBUG_TRIBUNAL_OF_AGES
bool BrannScript::getBossStates(uint32& source)
{
    if (!debugController)
        return false;

    uint32 bossState = 0;

    if (CAST_AI(ControllerAI, debugController->AI())->events.isEventActive(EVENT_KADRAK_CAST_SPELL))
        bossState = BOSS_KADDRAK_ACTIVE;
    if (CAST_AI(ControllerAI, debugController->AI())->events.isEventActive(EVENT_MARNAK_CAST_SPELL))
        bossState |= BOSS_MARNAK_ACTIVE;
    if (CAST_AI(ControllerAI, debugController->AI())->events.isEventActive(EVENT_ABEDNEUM_CAST_SPELL))
        bossState |= BOSS_ABEDNEUM_ACTIVE;

    source = bossState;
    return true;
};
#endif

void BrannAI::Reset()
{
    if (!HasEscortState(STATE_ESCORT_ESCORTING))
    {
        bIsLowHP = false;
        bIsBattle = false;
        uiStep = 0;
        uiPhaseTimer = 0;
        brannSparklinNews = true;
        _addSpawnPos = true;
        _eventStarted = false;

        me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        me->SetStandState(UNIT_STAND_STATE_STAND);

        if (instance)
            instance->SetData(DATA_BRANN_EVENT, NOT_STARTED);
    }
};

void BrannAI::WaypointReached(uint32 waypointId)
{
    switch (waypointId)
    {
        case 7:
        {
            if (Creature* pController = GetBossPointer(BOSS_CONTROLLER))
            {
                if (!pController->isAlive())
                    pController->Respawn();
                pController->AI()->DoAction(ACTION_START_EVENT);
                _eventStarted = true;
            }
            else
                me->Kill(me);
        }break;

        case 13:
        {
            DoScriptText(SAY_EVENT_INTRO_1, me);
            SetEscortPaused(true);
            JumpToNextStep(15000);
        }break;

        case 17:
        {
            DoScriptText(SAY_EVENT_INTRO_2, me);
            if (instance)
                instance->HandleGameObject(instance->GetData64(DATA_GO_TRIBUNAL_CONSOLE), true);
            me->SetStandState(UNIT_STAND_STATE_KNEEL);
            SetEscortPaused(true);
            JumpToNextStep(8500);
        }break;

        case 18:
        {
            SetEscortPaused(true);
        }break;
    }
};

void BrannAI::SpawnDwarf(uint32 uiType)
{
    _addSpawnPos ? _addSpawnPos = false : _addSpawnPos = true;

    Position AddSpawnLocation;
    AddSpawnLocation.Relocate(SpawnLocations[_addSpawnPos]);

    switch (uiType)
    {
        case 1:
        {
            uint32 uiSpawnNumber = DUNGEON_MODE(2, 3);
            for (uint8 i = 0; i < uiSpawnNumber; ++i)
            {
                if (_addSpawnPos)
                    i==0 ? AddSpawnLocation.m_positionY+=3.0f : AddSpawnLocation.m_positionY-=3.0f;
                else
                    i==0 ? AddSpawnLocation.m_positionX+=3.0f : AddSpawnLocation.m_positionX-=3.0f;

                me->SummonCreature(CREATURE_DARK_RUNE_PROTECTOR, AddSpawnLocation, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
            }
            me->SummonCreature(CREATURE_DARK_RUNE_STORMCALLER, SpawnLocations[_addSpawnPos], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
        }break;

        case 2:
        {
            for (uint8 i = 0; i < 2; ++i)
            {
                if (_addSpawnPos)
                    i==0 ? AddSpawnLocation.m_positionY+=3.0f : AddSpawnLocation.m_positionY-=3.0f;
                else
                    i==0 ? AddSpawnLocation.m_positionX+=3.0f : AddSpawnLocation.m_positionX-=3.0f;

                me->SummonCreature(CREATURE_DARK_RUNE_STORMCALLER, AddSpawnLocation, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
            }break;

            case 3:
                me->SummonCreature(CREATURE_IRON_GOLEM_CUSTODIAN, SpawnLocations[_addSpawnPos], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
        }break;
    }

    if (instance)
    {
        Creature* pCreature = NULL;
        uint64 creatureGUID = _addSpawnPos ? instance->GetData64(DATA_LIGHTNING_TRIGGER_RIGHT) : instance->GetData64(DATA_LIGHTNING_TRIGGER_LEFT);
        if (pCreature = me->GetCreature((*me), creatureGUID))
            pCreature->CastSpell(pCreature, SPELL_VISUAL_LIGHTNING, true);
    }
};

Creature* BrannAI::GetBossPointer(BossId boss)
{
    switch (boss)
    {
        case BOSS_KADDRAK:    { return  Creature::GetCreature((*me), instance->GetData64(DATA_KADDRAK_FIRST));       };
        case BOSS_MARNAK:     { return  Creature::GetCreature((*me), instance->GetData64(DATA_MARNAK_FIRST));        };
        case BOSS_ABEDNEUM:   { return  Creature::GetCreature((*me), instance->GetData64(DATA_ABEDNEUM));            };
        case BOSS_CONTROLLER: { return  Creature::GetCreature((*me), instance->GetData64(DATA_TRIBUNAL_CONTROLLER)); };
        default: return NULL;
    }
};

void BrannAI::JustSummoned(Creature* summoned)
{
    summoned->GetMotionMaster()->MovePoint(1, AddPointPos);
};

void BrannAI::JumpToNextStep(uint32 uiTimer)
{
    uiPhaseTimer = uiTimer;
    ++uiStep;
};

void BrannAI::StartWP()
{
    me->GetMotionMaster()->Initialize();
    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    _homePosition.Relocate(me);
    SetEscortPaused(false);
    uiStep = 1;
    Start(true);
};

void BrannAI::JustDied(Unit* killer)
{
    if (!instance)
        return;

    if (instance->GetData(DATA_BRANN_EVENT) == DONE)
        return;

    instance->SetData(DATA_BRANN_EVENT, FAIL);

    if (_eventStarted)
    {
        if (Creature* pController = GetBossPointer(BOSS_CONTROLLER))
            pController->AI()->DoAction(ACTION_RESET_EVENT);
    }
    else
    {
        me->MonsterSay(SAY_CUSTOM_BRANN_RESPAWN_FIRST, LANG_UNIVERSAL, 0);
        me->MonsterSay(SAY_CUSTOM_BRANN_RESPAWN_SECOND, LANG_UNIVERSAL, 0);
    }
};

void BrannAI::DamageTaken(Unit* /*done_by*/, uint32 & /*damage*/)
{
    if (brannSparklinNews)
        brannSparklinNews = false;

    if (!bIsLowHP && HealthBelowPct(30))
    {
        DoScriptText(SAY_LOW_HEALTH, me);
        bIsLowHP = true;
    }
    else if (bIsLowHP && !HealthBelowPct(30))
        bIsLowHP = false;
};

uint32 BrannAI::GetData(uint32 type)
{
    if (type == DATA_BRANN_SPARKLIN_NEWS)
        return brannSparklinNews ? 1 : 0;

    return 0;
};

void BrannAI::DoAction(const int32 action)
{
    switch (action)
    {
        case ACTION_BRANN_START_ESCORT:{ StartWP(); }break;
    }
};

void BrannAI::UpdateEscortAI(const uint32 uiDiff)
{
    if (!me->isAlive())
        return;

    if (uiPhaseTimer <= uiDiff)
    {
        switch (uiStep)
        {
            case 1:
                if (instance)
                {
                    if (instance->GetData(DATA_BRANN_EVENT) != NOT_STARTED)
                        return;
                    instance->SetData(DATA_BRANN_EVENT, IN_PROGRESS);
                }
                bIsBattle = false;
                DoScriptText(SAY_ESCORT_START, me);
                SetRun(true);
                JumpToNextStep(0);
            break;

            case 3:
                SetEscortPaused(false);
                JumpToNextStep(0);
            break;

            case 5:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_ABEDNEUM))
                        DoScriptText(SAY_EVENT_INTRO_3_ABED, pBoss);
                JumpToNextStep(8500);
            break;

            case 6:
                DoScriptText(SAY_EVENT_A_1, me);
                JumpToNextStep(6500);
            break;

            case 7:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_KADDRAK))
                        DoScriptText(SAY_EVENT_A_2_KADD, pBoss);
                JumpToNextStep(12500);
            break;

            case 8:
                DoScriptText(SAY_EVENT_A_3, me);
                if (Creature* pController = GetBossPointer(BOSS_CONTROLLER))
                    pController->AI()->DoAction(ACTION_ENABLE_KADDRAK);
                JumpToNextStep(5000);
            break;

            case 9:
                me->SetReactState(REACT_PASSIVE);
                SpawnDwarf(1);
                JumpToNextStep(20000);
            break;

            case 10:
                DoScriptText(SAY_EVENT_B_1, me);
                JumpToNextStep(6000);
            break;

            case 11:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_MARNAK))
                        DoScriptText(SAY_EVENT_B_2_MARN, pBoss);
                SpawnDwarf(1);
                JumpToNextStep(20000);
            break;

            case 12:
                DoScriptText(SAY_EVENT_B_3, me);
                if (Creature* pController = GetBossPointer(BOSS_CONTROLLER))
                    pController->AI()->DoAction(ACTION_ENABLE_MARNAK);
                JumpToNextStep(10000);
            break;

            case 13:
                SpawnDwarf(1);
                JumpToNextStep(10000);
            break;

            case 14:
                SpawnDwarf(2);
                JumpToNextStep(20000);
            break;

            case 15:
                DoScriptText(SAY_EVENT_C_1, me);
                SpawnDwarf(1);
                JumpToNextStep(10000);
            break;

            case 16:
                SpawnDwarf(2);
                JumpToNextStep(20000);
            break;

            case 17:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_ABEDNEUM))
                        DoScriptText(SAY_EVENT_C_2_ABED, pBoss);
                SpawnDwarf(1);
                JumpToNextStep(20000);
            break;

            case 18:
                DoScriptText(SAY_EVENT_C_3, me);
                if (Creature* pController = GetBossPointer(BOSS_CONTROLLER))
                    pController->AI()->DoAction(ACTION_ENABLE_ABEDNEUM);
                JumpToNextStep(5000);
            break;

            case 19:
                SpawnDwarf(2);
                JumpToNextStep(10000);
            break;

            case 20:
                SpawnDwarf(1);
                JumpToNextStep(15000);
            break;

            case 21:
                DoScriptText(SAY_EVENT_D_1, me);
                SpawnDwarf(3);
                JumpToNextStep(20000);
            break;

            case 22:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_ABEDNEUM))
                        DoScriptText(SAY_EVENT_D_2_ABED, pBoss);
                SpawnDwarf(1);
                JumpToNextStep(5000);
            break;

            case 23:
                SpawnDwarf(2);
                JumpToNextStep(15000);
            break;

            case 24:
                DoScriptText(SAY_EVENT_D_3, me);
                SpawnDwarf(3);
                JumpToNextStep(5000);
            break;

            case 25:
                SpawnDwarf(1);
                JumpToNextStep(5000);
            break;

            case 26:
                SpawnDwarf(2);
                JumpToNextStep(10000);
            break;

            case 27:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_ABEDNEUM))
                        DoScriptText(SAY_EVENT_D_4_ABED, pBoss);
                SpawnDwarf(1);
                JumpToNextStep(10000);
            break;

            case 28:
                me->SetReactState(REACT_DEFENSIVE);
                DoScriptText(SAY_EVENT_END_01, me);
                me->SetStandState(UNIT_STAND_STATE_STAND);
                if (instance)
                    instance->HandleGameObject(instance->GetData64(DATA_GO_SKY_FLOOR), true);
                if (Creature* pController = GetBossPointer(BOSS_CONTROLLER))
                {
                    pController->AI()->DoAction(ACTION_SET_ALL_GO_WHITE);
                    pController->DealDamage(pController, pController->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                }
                bIsBattle = true;
                SetEscortPaused(false);
                JumpToNextStep(6500);
            break;

            case 29:
                DoScriptText(SAY_EVENT_END_02, me);
                if (instance)
                    instance->SetData(DATA_BRANN_EVENT, DONE);
                me->CastSpell(me, SPELL_REWARD_ACHIEVEMENT, true);
                JumpToNextStep(5500);
            break;

            case 30:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_ABEDNEUM))
                        DoScriptText(SAY_EVENT_END_03_ABED, pBoss);
                JumpToNextStep(8500);
            break;

            case 31:
                DoScriptText(SAY_EVENT_END_04, me);
                JumpToNextStep(11500);
            break;

            case 32:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_ABEDNEUM))
                        DoScriptText(SAY_EVENT_END_05_ABED, pBoss);
                JumpToNextStep(11500);
            break;

            case 33:
                DoScriptText(SAY_EVENT_END_06, me);
                JumpToNextStep(4500);
            break;

            case 34:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_ABEDNEUM))
                        DoScriptText(SAY_EVENT_END_07_ABED, pBoss);
                JumpToNextStep(22500);
            break;

            case 35:
                DoScriptText(SAY_EVENT_END_08, me);
                JumpToNextStep(7500);
            break;

            case 36:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_KADDRAK))
                        DoScriptText(SAY_EVENT_END_09_KADD, pBoss);
                JumpToNextStep(18500);
            break;

            case 37:
                DoScriptText(SAY_EVENT_END_10, me);
                JumpToNextStep(5500);
            break;

            case 38:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_KADDRAK))
                        DoScriptText(SAY_EVENT_END_11_KADD, pBoss);
                JumpToNextStep(20500);
            break;

            case 39:
                DoScriptText(SAY_EVENT_END_12, me);
                JumpToNextStep(2500);
            break;

            case 40:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_KADDRAK))
                        DoScriptText(SAY_EVENT_END_13_KADD, pBoss);
                JumpToNextStep(19500);
            break;

            case 41:
                DoScriptText(SAY_EVENT_END_14, me);
                JumpToNextStep(10500);
            break;

            case 42:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_MARNAK))
                        DoScriptText(SAY_EVENT_END_15_MARN, pBoss);
                JumpToNextStep(6500);
            break;

            case 43:
                DoScriptText(SAY_EVENT_END_16, me);
                JumpToNextStep(6500);
            break;

            case 44:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_MARNAK))
                        DoScriptText(SAY_EVENT_END_17_MARN, pBoss);
                JumpToNextStep(25500);
            break;

            case 45:
                DoScriptText(SAY_EVENT_END_18, me);
                JumpToNextStep(23500);
            break;

            case 46:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_MARNAK))
                        DoScriptText(SAY_EVENT_END_19_MARN, pBoss);
                JumpToNextStep(3500);
            break;

            case 47:
                DoScriptText(SAY_EVENT_END_20, me);
                JumpToNextStep(8500);
            break;

            case 48:
                if (instance)
                    if (Creature* pBoss = GetBossPointer(BOSS_ABEDNEUM))
                        DoScriptText(SAY_EVENT_END_21_ABED, pBoss);
                JumpToNextStep(5500);
            break;

            case 49:
            {
                if (instance)
                {
                    instance->HandleGameObject(instance->GetData64(DATA_GO_KADDRAK), false);
                    instance->HandleGameObject(instance->GetData64(DATA_GO_MARNAK), false);
                    instance->HandleGameObject(instance->GetData64(DATA_GO_ABEDNEUM), false);
                    instance->HandleGameObject(instance->GetData64(DATA_GO_TRIBUNAL_CONSOLE), false);
                    instance->HandleGameObject(instance->GetData64(DATA_GO_SKY_FLOOR), false);
                }
                JumpToNextStep(180000);
            }break;

            case 50:
                me->DespawnOrUnsummon();
                SetEscortPaused(false);
            break;
        }
    }else uiPhaseTimer -= uiDiff;

    if (!UpdateVictim())
        return;

    DoMeleeAttackIfReady();
};
