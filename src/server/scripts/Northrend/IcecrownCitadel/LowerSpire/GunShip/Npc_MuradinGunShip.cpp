#include "Npc_MuradinGunShip.h"

bool NpcMuradinGunship::OnGossipHello(Player* player, Creature* pCreature)
{
    InstanceScript* pInstance = pCreature->GetInstanceScript();
    if (pInstance && pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
    {
        if ((!player->GetGroup() || !player->GetGroup()->IsLeader(player->GetGUID())) && !player->isGameMaster())
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I'm not the raid leader...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            return true;
        }

        player->ADD_GOSSIP_ITEM(0, "My companions are all accounted for, Muradin. Let's go!", 631, 1001);
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }
    return false;
}

bool NpcMuradinGunship::OnGossipSelect(Player* player, Creature* pCreature, uint32 /*sender*/, uint32 action)
{
    InstanceScript* instance = pCreature->GetInstanceScript();
    player->PlayerTalkClass->ClearMenus();
    player->CLOSE_GOSSIP_MENU();
    
    if (action == GOSSIP_ACTION_INFO_DEF+2)
        pCreature->MonsterSay("I'll wait for the raid leader", LANG_UNIVERSAL, player->GetGUID());
    
    if (action == 1001)
    {
        pCreature->AI()->DoAction(ACTION_INTRO_START);
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }
    return true;
}

void NpcMuradinGunshipAI::Reset()
{
    if (_instance->GetBossState(DATA_GUNSHIP_EVENT) == IN_PROGRESS)
        return;
    me->SetReactState(REACT_PASSIVE);
    me->setFaction(1802);
    events.Reset();
    map = me->GetMap();
    skybreaker = me->GetTransport();
    UpdateTransportMotionInMap(skybreaker);
    SummonCount = RAID_MODE(3, 5, 4, 6);
    count = 0;
    RocketerCount = RAID_MODE(2, 4, 2, 4);
    RiflCount = RAID_MODE(4, 6, 4, 6);
    RocketerDieCount = 0;
    RiflDieCount = 0;
    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    EventScheduled = false;
}

void NpcMuradinGunshipAI::SendMusicToPlayers(uint32 musicId) const
{
    WorldPacket data(SMSG_PLAY_MUSIC, 4);
    data << uint32(musicId);
    SendPacketToPlayers(&data);
}

void NpcMuradinGunshipAI::SendPacketToPlayers(WorldPacket const* data) const
{
    Map::PlayerList const& players = me->GetMap()->GetPlayers();
    if (!players.isEmpty())
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            if (Player* player = itr->getSource())
                player->GetSession()->SendPacket(data);
}

bool NpcMuradinGunshipAI::CanAIAttack(Unit const* target) const
{
    if (target->GetEntry() == NPC_GB_KORKRON_SERGANTE || target->GetEntry() == NPC_GB_KORKRON_REAVERS)
        return true;

    return false;
}

void NpcMuradinGunshipAI::DoAction(int32 const action)
{
    switch (action)
    {
        case ACTION_INTRO_START:
            events.ScheduleEvent(EVENT_INTRO_ALLIANCE_1, 1000);
            events.ScheduleEvent(EVENT_START_FLY, 2500);
            events.ScheduleEvent(EVENT_INTRO_ALLIANCE_2, 7000);
            events.ScheduleEvent(EVENT_INTRO_ALLIANCE_3, 28000);
            events.ScheduleEvent(EVENT_INTRO_ALLIANCE_4, 35000);
            events.ScheduleEvent(EVENT_INTRO_ALLIANCE_5, 40000);
            events.ScheduleEvent(EVENT_INTRO_ALLIANCE_6, 47000);
            events.ScheduleEvent(EVENT_INTRO_ALLIANCE_7, 53000);
            events.ScheduleEvent(EVENT_INTRO_ALLIANCE_8, 58900);
            break;
        case ACTION_BATTLE_EVENT:
        {
            if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                events.ScheduleEvent(EVENT_WIPE_CHECK, 5000);

            Creature* pAllianceBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SKYBREAKER_BOSS));
            Creature* pHordeBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_ORGRIMMAR_HAMMER_BOSS));

            if (pHordeBoss && pAllianceBoss)
            {
                me->SetReactState(REACT_AGGRESSIVE);
                pHordeBoss->SetInCombatWith(pAllianceBoss);
                pAllianceBoss->SetInCombatWith(pHordeBoss);
                pAllianceBoss->AddThreat(pHordeBoss, 0.0f);
                pHordeBoss->AddThreat(pAllianceBoss, 0.0f);
                _instance->SetBossState(DATA_GUNSHIP_EVENT, IN_PROGRESS);
                events.ScheduleEvent(EVENT_SUMMON_PORTAL, 30000);
                RelocateTransport(skybreaker);
                RelocateTransport(CheckUnfriendlyShip(me,_instance, DATA_GB_HIGH_OVERLORD_SAURFANG));
            }
            else
                me->AI()->DoAction(ACTION_FAIL);
            break;
        }
        case ACTION_DONE:
            if (Creature* pAllianceBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SKYBREAKER_BOSS)))
            {
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, pAllianceBoss);
            }
            if (Creature* pHordeBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_ORGRIMMAR_HAMMER_BOSS)))
            {
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, pHordeBoss);
            }

            me->SetReactState(REACT_PASSIVE);
            _instance->SetBossState(DATA_GUNSHIP_EVENT, DONE);
            _instance->DoCastSpellOnPlayers(SPELL_REMOVE_ROCKET_PACK);
            Talk(SAY_ALLIANCE_VICTORY);
            StartFlyShip(CheckUnfriendlyShip(me, _instance, DATA_GB_HIGH_OVERLORD_SAURFANG));
            StopFight(skybreaker, CheckUnfriendlyShip(me, _instance, DATA_GB_HIGH_OVERLORD_SAURFANG));
            events.CancelEvent(EVENT_WIPE_CHECK);
            events.ScheduleEvent(EVENT_OUTRO_ALLIANCE_1, 3500);
            events.ScheduleEvent(EVENT_OUTRO_ALLIANCE_2, 23000);
            events.ScheduleEvent(EVENT_OUTRO_ALLIANCE_3, 32000);
            break;
        case ACTION_FAIL:
            events.CancelEvent(EVENT_WIPE_CHECK);
            Talk(SAY_ALLIANCE_DEFEAT);
            if (Creature* pAllianceBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SKYBREAKER_BOSS)))
            {
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, pAllianceBoss);
            }
            if (Creature* pHordeBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_ORGRIMMAR_HAMMER_BOSS)))
            {
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, pHordeBoss);
            }
            StopFight(skybreaker, CheckUnfriendlyShip(me, _instance, DATA_GB_HIGH_OVERLORD_SAURFANG));
            events.ScheduleEvent(EVENT_FAIL, 10000);
            break;
        case ACTION_MAGE_DIE:
            //Check, if there really is no mage
            if (me->GetEntry() != NPC_GB_SKYBREAKER_SORCERERS){
                if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE)
                    events.ScheduleEvent(EVENT_SPAWN_MAGE, 33000);
            }
            break;
        case ACTION_ROCK_DIE:
            ++RocketerDieCount;
            if (RocketerDieCount == RocketerCount)
                if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE)
                    events.ScheduleEvent(EVENT_RESPAWN_ROCKETEER, 60000);
                break;
        case ACTION_AXES_RIFL_DIE:
            ++RiflDieCount;
            if (RiflDieCount == RiflCount)
                if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE)
                    events.ScheduleEvent(EVENT_RESPAWN_AXES_RIFLEMEN, 60000);
                break;
    }
}

void NpcMuradinGunshipAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE && me->GetHealthPct() < 2.0f )
    {
        damage = 0;
    }

    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE && me->GetHealthPct() < 2.0f )
    {
        damage = 0;
        me->AI()->DoAction(ACTION_FAIL);
    }
}

void NpcMuradinGunshipAI::MovementInform(uint32 type, uint32 pointId)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (pointId = 1)
        me->DespawnOrUnsummon(1000);
}

void NpcMuradinGunshipAI::UpdateAI(const uint32 diff)
{/*
    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (!HealthAbovePct(75))
    {
        me->SetHealth(me->GetMaxHealth() / 100 * 76); // find a better way to avoid the hardcore spell spam ....
        DoCast(me, SPELL_TASTE_OF_BLOOD);
    }

    if (UpdateVictim())
    {
        if (!EventScheduled)
        {
            EventScheduled = true; // should temp fix the hardcore casting
            events.ScheduleEvent(EVENT_RENDING_THROW, 3000);
        }
    }

    events.Update(diff);

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_WIPE_CHECK:
                DoCheckFallingPlayer(me);
                if (DoWipeCheck(skybreaker))
                    events.ScheduleEvent(EVENT_WIPE_CHECK, 3000);
                else
                    me->AI()->DoAction(ACTION_FAIL);
                break;
            case EVENT_INTRO_ALLIANCE_1:
                Talk(SAY_INTRO_ALLIANCE_0);
                break;
            case EVENT_START_FLY:
                StartFlyShip(skybreaker);
                break;
            case EVENT_INTRO_ALLIANCE_2:
                Talk(SAY_INTRO_ALLIANCE_1);
                break;
            case EVENT_INTRO_ALLIANCE_3:
                Talk(SAY_INTRO_ALLIANCE_2);
                StartFlyShip(CheckUnfriendlyShip(me, _instance, DATA_GB_HIGH_OVERLORD_SAURFANG));
                break;
            case EVENT_INTRO_ALLIANCE_4:
                Talk(SAY_INTRO_ALLIANCE_3);
                break;
            case EVENT_INTRO_ALLIANCE_5:
                StopFlyShip(skybreaker);
                StopFlyShip(CheckUnfriendlyShip(me, _instance, DATA_GB_HIGH_OVERLORD_SAURFANG));
                Talk(SAY_INTRO_ALLIANCE_4);
                break;
            case EVENT_INTRO_ALLIANCE_6:
                Talk(SAY_INTRO_ALLIANCE_5);
                SendMusicToPlayers(17289);
                me->AI()->DoAction(ACTION_BATTLE_EVENT);
                break;
            case EVENT_INTRO_ALLIANCE_7:
                if (Creature* pSaurfang = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HIGH_OVERLORD_SAURFANG_NOT_VISUAL)))
                {
                    pSaurfang->AI()->Talk(SAY_HIGH_OVERLORD_SAURFANG_NOT_VISUAL);
                    pSaurfang->SetReactState(REACT_PASSIVE);
                    pSaurfang->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                }
                break;
            case EVENT_INTRO_ALLIANCE_8:
                Talk(SAY_INTRO_ALLIANCE_7);
                break;
            case EVENT_SUMMON_PORTAL:
                if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE)
                {
                    events.ScheduleEvent(EVENT_BOARDING_TALK, 10000);
                    events.ScheduleEvent(EVENT_BOARDING_GUNSHIP, 2500); // TODO: Fix the timers
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_PORTAL, -15.51547f, -0.160213f, 28.87252f, 1.56211f);
                    CheckUnfriendlyShip(me, _instance, DATA_GB_HIGH_OVERLORD_SAURFANG)->AddNPCPassengerInInstance(NPC_GB_PORTAL, 47.55099f, -0.101778f, 37.61111f, 1.55138f);
                }
                break;
            case EVENT_RENDING_THROW:
                if (UpdateVictim())
                    if (me->getVictim()->IsWithinDistInMap(me, 30.0f, false))
                    {
                        DoCastVictim(SPELL_RENDING_THROW);
                        EventScheduled = false;
                    }
                    else
                        events.CancelEvent(EVENT_RENDING_THROW);
                    break;
            case EVENT_TASTE_OF_BLOOD:
                DoCast(me, SPELL_TASTE_OF_BLOOD);
                break;
            case EVENT_BOARDING_TALK:
                Talk(SAY_BOARDING_SKYBREAKER_1);
                break;
            case EVENT_BOARDING_GUNSHIP:
                if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE){
                    if (Creature* pSaurfang = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HIGH_OVERLORD_SAURFANG_NOT_VISUAL)))
                    {
                        pSaurfang->AI()->Talk(SAY_BOARDING_SKYBREAKER_SAURFANG);
                    }
                    if (Creature* Sergante = skybreaker->AddNPCPassengerInInstance(NPC_GB_KORKRON_SERGANTE, -15.51547f, -0.160213f, 20.87252f, 1.56211f))
                    {
                        Sergante->CastSpell(Sergante, SPELL_TELEPORT_VISUAL, true);
                    }
                    events.ScheduleEvent(EVENT_SUMMON_PORTAL, 90000);
                    events.ScheduleEvent(EVENT_BOARDING_REAVERS_MARINE, 3000);
                    count = 0;
                }
                break;
            case EVENT_BOARDING_REAVERS_MARINE:
                if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE){
                    if (count <= SummonCount)
                    {
                        if (Creature* Reavers = skybreaker->AddNPCPassengerInInstance(NPC_GB_KORKRON_REAVERS, -15.51547f, -0.160213f, 20.87252f, 1.56211f))
                        {
                            Reavers->CastSpell(Reavers, SPELL_TELEPORT_VISUAL, true);
                            events.ScheduleEvent(EVENT_BOARDING_REAVERS_MARINE, 21000 / SummonCount);
                            ++count;
                        }
                    }
                }
                break;
            case EVENT_OUTRO_ALLIANCE_1:
                _instance->DoCompleteAchievement(RAID_MODE(IM_ON_A_BOAT_10,IM_ON_A_BOAT_25,IM_ON_A_BOAT_10,IM_ON_A_BOAT_25));
                _instance->DoCastSpellOnPlayers(SPELL_ACHIEVEMENT_CHECK);
                StartFlyShip(skybreaker);
                StopFlyShip(CheckUnfriendlyShip(me,_instance,DATA_GB_HIGH_OVERLORD_SAURFANG));
                break;
            case EVENT_OUTRO_ALLIANCE_2:
                StopFlyShip(skybreaker);
                me->SummonGameObject(RAID_MODE(GO_CAPITAN_CHEST_A_10N, GO_CAPITAN_CHEST_A_25N, GO_CAPITAN_CHEST_A_10H, GO_CAPITAN_CHEST_A_25H), -590.200022f, 2241.193115f, 538.588269f, 0, 0, 0, 0, 0, 100000);
                me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
                me->GetMotionMaster()->MovePoint(0, -590.700f, 2213.01f, 539.1f);
                break;
            case EVENT_OUTRO_ALLIANCE_3:
                me->GetMotionMaster()->MovePoint(1, -555.59f, 2213.01f, 539.28f);
                break;
            case EVENT_FAIL:
                TeleportPlayers(map, ALLIANCE);
                events.ScheduleEvent(EVENT_RESTART_EVENT, 2000);
                _instance->SetBossState(DATA_GUNSHIP_EVENT, NOT_STARTED);
                break;
            case EVENT_RESTART_EVENT:
                if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE){
                    _instance->SetBossState(DATA_GUNSHIP_EVENT, FAIL);
                    RestartEvent(skybreaker, CheckUnfriendlyShip(me,_instance,DATA_GB_HIGH_OVERLORD_SAURFANG), map, ALLIANCE);
                }
                break;
            case EVENT_SPAWN_MAGE:
                if (me->GetEntry() != NPC_GB_SKYBREAKER_SORCERERS){
                    Talk(SAY_NEW_MAGE_SPAWNED);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_SORCERERS, -17.8356f, 0.031688f, 20.823f, 4.73231f);
                }
                break;
            case EVENT_RESPAWN_ROCKETEER:
                Talk(SAY_NEW_MORTAR_TEAM_SPAWNED);
                if (RocketerCount == 2)
                {
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MORTAR_SOLDIER, -8.61003f, 15.483f, 20.4158f, 4.69854f);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MORTAR_SOLDIER, -27.9583f, 14.8875f, 20.4428f, 4.77865f);
                }
                if (RocketerCount == 4)
                {
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MORTAR_SOLDIER, -8.61003f, 15.483f, 20.4158f, 4.69854f);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MORTAR_SOLDIER, -27.9583f, 14.8875f, 20.4428f, 4.77865f);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MORTAR_SOLDIER, -15.61003f, 15.483f, 20.4158f, 4.69854f);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MORTAR_SOLDIER, -20.9583f, 14.8875f, 20.4428f, 4.77865f);
                }
                break;
            case EVENT_RESPAWN_AXES_RIFLEMEN:
                Talk(SAY_NEW_RIFLEMEN_SPAWNED);
                if (RiflCount == 4)
                {
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -28.0876f, -22.9462f, 21.659f, 4.72416f);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -21.7406f, -22.9462f, 21.659f, 4.72416f);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -14.9806f, -22.9462f, 21.659f, 4.72416f);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -5.15231f, -22.9462f, 21.659f, 4.72416f);
                }
                if (RiflCount == 6)
                {
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -33.0876f, -22.9462f, 21.659f, 4.72416f);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -28.0876f, -22.9462f, 21.659f, 4.72416f);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -21.7406f, -22.9462f, 21.659f, 4.72416f);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -14.9806f, -22.9462f, 21.659f, 4.72416f);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -5.15231f, -22.9462f, 21.659f, 4.72416f);
                    skybreaker->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, 0.15231f, -22.9462f, 21.659f, 4.72416f);
                }
                break;
        }
    }

    if (!me->GetCurrentSpell(CURRENT_MELEE_SPELL))
        DoCastVictim(SPELL_CLEAVE);

    DoMeleeAttackIfReady();
*/}