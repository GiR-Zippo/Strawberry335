#include "Npc_SaurfangGunShip.h"

bool NpcSaurfangGunship::OnGossipHello(Player* player, Creature* pCreature)
{
    InstanceScript* pInstance = pCreature->GetInstanceScript();
    if (pInstance && pInstance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
    {
        if ((!player->GetGroup() || !player->GetGroup()->IsLeader(player->GetGUID())) && !player->isGameMaster())
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I'm not the raid leader...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            return true;
        }
        
        player->ADD_GOSSIP_ITEM(0, "My companions are all accounted for, Saurfang. Let's go!", 631, 1001);
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }
    return false;
}

bool NpcSaurfangGunship::OnGossipSelect(Player* player, Creature* pCreature, uint32 , uint32 action)
{
    InstanceScript* instance = pCreature->GetInstanceScript();
    player->PlayerTalkClass->ClearMenus();
    player->CLOSE_GOSSIP_MENU();
    
    if (action == GOSSIP_ACTION_INFO_DEF+2)
        pCreature->MonsterSay("I'll wait for the raid leader.", LANG_UNIVERSAL, player->GetGUID());
    
    if (action == 1001)
    {
        pCreature->AI()->DoAction(ACTION_INTRO_START);
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }
    return true;
}

void NpcSaurfangGunshipAI::Reset()
{
    if (_instance->GetBossState(DATA_GUNSHIP_EVENT) == IN_PROGRESS)
        return;
    me->SetReactState(REACT_PASSIVE);
    me->setFaction(1801);
    events.Reset();
    map = me->GetMap();
    orgrimmar = me->GetTransport();
    UpdateTransportMotionInMap(orgrimmar);
    RocketerDieCount = 0;
    AxesDieCount = 0;
    SummonCount = RAID_MODE(3, 5, 4, 6);
    count = 0;
    RocketerCount = RAID_MODE(2, 4, 2, 4);
    AxesCount = RAID_MODE(4, 6, 4, 6);
    EventScheduled = false;
}

void NpcSaurfangGunshipAI::SendMusicToPlayers(uint32 musicId) const
{
    WorldPacket data(SMSG_PLAY_MUSIC, 4);
    data << uint32(musicId);
    SendPacketToPlayers(&data);
}

void NpcSaurfangGunshipAI::SendPacketToPlayers(WorldPacket const* data) const
{
    Map::PlayerList const& players = me->GetMap()->GetPlayers();
    if (!players.isEmpty())
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            if (Player* player = itr->getSource())
                player->GetSession()->SendPacket(data);
}

bool NpcSaurfangGunshipAI::CanAIAttack(Unit const* target) const
{
    if (target->GetEntry() == NPC_GB_SKYBREAKER_SERGANTE || target->GetEntry() == NPC_GB_SKYBREAKER_MARINE)
        return true;
    
    return false;
}

void NpcSaurfangGunshipAI::DoAction(int32 const action)
{
    switch (action)
    {
        case ACTION_INTRO_START:
            events.ScheduleEvent(EVENT_INTRO_HORDE_1, 1000);
            events.ScheduleEvent(EVENT_INTRO_HORDE_1_1, 11000);
            events.ScheduleEvent(EVENT_START_FLY, 5000);
            events.ScheduleEvent(EVENT_INTRO_HORDE_2, 32000);
            events.ScheduleEvent(EVENT_INTRO_HORDE_3, 51000);
            events.ScheduleEvent(EVENT_INTRO_HORDE_4, 57000);
            events.ScheduleEvent(EVENT_INTRO_HORDE_5, 62000);
            break;
        case ACTION_BATTLE_EVENT:
        {
            if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
                events.ScheduleEvent(EVENT_WIPE_CHECK, 5000);
            
            Creature* pHordeBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_ORGRIMMAR_HAMMER_BOSS));
            Creature* pAllianceBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SKYBREAKER_BOSS));
            
            if (pHordeBoss && pAllianceBoss)
            {
                me->SetReactState(REACT_AGGRESSIVE);
                pHordeBoss->SetInCombatWith(pAllianceBoss);
                pAllianceBoss->SetInCombatWith(pHordeBoss);
                pAllianceBoss->AddThreat(pHordeBoss, 0.0f);
                pHordeBoss->AddThreat(pAllianceBoss, 0.0f);
                _instance->SetBossState(DATA_GUNSHIP_EVENT, IN_PROGRESS);
                events.ScheduleEvent(EVENT_SUMMON_PORTAL, 30000);
                RelocateTransport(orgrimmar);
                RelocateTransport(CheckUnfriendlyShip(me,_instance, DATA_GB_MURADIN_BRONZEBEARD));
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
            Talk(SAY_HORDE_VICTORY);
            StartFlyShip(CheckUnfriendlyShip(me, _instance, DATA_GB_MURADIN_BRONZEBEARD));
            StopFight(orgrimmar,CheckUnfriendlyShip(me,_instance, DATA_GB_MURADIN_BRONZEBEARD));
            events.CancelEvent(EVENT_WIPE_CHECK);
            events.ScheduleEvent(EVENT_OUTRO_HORDE_1, 3500);
            events.ScheduleEvent(EVENT_OUTRO_HORDE_2, 21000);
            events.ScheduleEvent(EVENT_OUTRO_HORDE_3, 32000);
            break;
        case ACTION_FAIL:
            events.CancelEvent(EVENT_WIPE_CHECK);
            Talk(SAY_HORDE_DEFEAT);
            if (Creature* pHordeBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_ORGRIMMAR_HAMMER_BOSS)))
            {
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, pHordeBoss);
            }
            if (Creature* pAllianceBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SKYBREAKER_BOSS)))
            {
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, pAllianceBoss);
            }
            StopFight(orgrimmar,CheckUnfriendlyShip(me,_instance, DATA_GB_MURADIN_BRONZEBEARD));
            events.ScheduleEvent(EVENT_FAIL, 10000);
            break;
        case ACTION_MAGE_DIE:
            //Check, if there really is no mage
            if (me->GetEntry() != NPC_GB_KORKRON_BATTLE_MAGE){
                events.ScheduleEvent(EVENT_SPAWN_MAGE, 60000);
            }
            break;
        case ACTION_ROCK_DIE:
            ++RocketerDieCount;
            if (RocketerDieCount == RocketerCount)
                events.ScheduleEvent(EVENT_RESPAWN_ROCKETEER, 60000);
            break;
        case ACTION_AXES_RIFL_DIE:
            ++AxesDieCount;
            if (AxesDieCount == AxesCount)
                events.ScheduleEvent(EVENT_RESPAWN_AXES_RIFLEMEN, 60000);
            break;
    }
}

void NpcSaurfangGunshipAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE && me->GetHealthPct() < 2.0f )
    {
        damage = 0;
    }
    
    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE && me->GetHealthPct() < 2.0f )
    {
        damage = 0;
        me->AI()->DoAction(ACTION_FAIL);
    }
}

void NpcSaurfangGunshipAI::MovementInform(uint32 type, uint32 pointId)
{
    if (type != POINT_MOTION_TYPE)
        return;
    
    if (pointId = 1)
        me->DespawnOrUnsummon(1000);
}

void NpcSaurfangGunshipAI::UpdateAI(const uint32 diff)
{
    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;
    
    if (_instance->GetBossState(DATA_GUNSHIP_EVENT) == IN_PROGRESS)
    {
        if (!HealthAbovePct(75))
        {
            me->SetHealth(me->GetMaxHealth() / 100 * 76); // find a better way to avoid the hardcore spell spam ....
            DoCast(me, SPELL_TASTE_OF_BLOOD);
        }
        
        if (UpdateVictim())
        {
            if (!EventScheduled)
            {
                events.ScheduleEvent(EVENT_RENDING_THROW, 1500);
                EventScheduled = true;
            }
        }
    }
    
    events.Update(diff);
    
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_WIPE_CHECK:
                DoCheckFallingPlayer(me);
                if (DoWipeCheck(orgrimmar))
                    events.ScheduleEvent(EVENT_WIPE_CHECK, 3000);
                else
                    me->AI()->DoAction(ACTION_FAIL);
                break;
            case EVENT_INTRO_HORDE_1:
                Talk(SAY_INTRO_HORDE_0);
                StartFlyShip(orgrimmar);
                break;
            case EVENT_INTRO_HORDE_1_1:
                Talk(SAY_INTRO_HORDE_0_1);
                break;
            case EVENT_START_FLY:
                StartFlyShip(orgrimmar); // Try
                break;
            case EVENT_INTRO_HORDE_2:
                StopFlyShip(orgrimmar);
                StartFlyShip(CheckUnfriendlyShip(me, _instance, DATA_GB_MURADIN_BRONZEBEARD));
                Talk(SAY_INTRO_HORDE_1);
                break;
            case EVENT_INTRO_HORDE_3:
                StopFlyShip(CheckUnfriendlyShip(me, _instance, DATA_GB_MURADIN_BRONZEBEARD));
                Talk(SAY_INTRO_HORDE_2);
                break;
            case EVENT_INTRO_HORDE_4:
                SendMusicToPlayers(17289);
                me->AI()->DoAction(ACTION_BATTLE_EVENT);
                if (Creature* pMuradin = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_MURADIN_BRONZEBEARD_NOT_VISUAL)))
                {
                    pMuradin->AI()->Talk(SAY_MURADIN_BRONZEBEARD_NOT_VISUAL);
                    pMuradin->SetReactState(REACT_PASSIVE);
                    pMuradin->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                }
                break;
            case EVENT_INTRO_HORDE_5:
                Talk(SAY_INTRO_HORDE_4);
                break;
            case EVENT_SUMMON_PORTAL:
                if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE){
                    //orgrimmar->AddNPCPassengerInInstance(NPC_GB_PORTAL, 15.03016f, -7.00016f, 37.70952f, 1.55138f);
                    //CheckUnfriendlyShip(me, _instance, DATA_MURADIN_BRONZEBEARD_NOT_VISUAL)->AddNPCPassengerInInstance(NPC_GB_PORTAL, 3.51547f, -0.160213f, 20.87252f, 3.05033f);
                    events.ScheduleEvent(EVENT_BOARDING_TALK, 10000);
                    events.ScheduleEvent(EVENT_BOARDING_GUNSHIP, 2500); // TODO: Fix the timers
                }
                break;
            case EVENT_BOARDING_TALK:
                Talk(SAY_BOARDING_ORGRIMS_HAMMER_1);
                break;
            case EVENT_BOARDING_GUNSHIP:
                if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE){
                    count = 0;
                    if (Creature* pSaurfang = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_MURADIN_BRONZEBEARD_NOT_VISUAL)))
                    {
                        pSaurfang->AI()->Talk(SAY_BOARDING_SKYBREAKER_MURADIN);
                    }
                    /*if (Creature* Sergante = orgrimmar->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_SERGANTE, 15.03016f, -7.00016f, 37.70952f, 1.55138f))
                    {
                        Sergante->CastSpell(Sergante, SPELL_TELEPORT_VISUAL, true);
                    }*/
                    events.ScheduleEvent(EVENT_BOARDING_REAVERS_MARINE, 3000);
                    events.ScheduleEvent(EVENT_SUMMON_PORTAL, 90000);
                }
                break;
            case EVENT_BOARDING_REAVERS_MARINE:
                if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE){
                    if (count <= SummonCount)
                    {/*
                        if (Creature* Marine = orgrimmar->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MARINE, 15.03016f, -7.00016f, 37.70952f, 1.55138f))
                        {
                            Marine->CastSpell(Marine, SPELL_TELEPORT_VISUAL, true);
                            count++;
                            events.ScheduleEvent(EVENT_BOARDING_REAVERS_MARINE, 2500);
                        }
                    */}
                }
                break;
            case EVENT_OUTRO_HORDE_1:
                _instance->DoCompleteAchievement(RAID_MODE(IM_ON_A_BOAT_10,IM_ON_A_BOAT_25,IM_ON_A_BOAT_10,IM_ON_A_BOAT_25));
                _instance->DoCastSpellOnPlayers(SPELL_ACHIEVEMENT_CHECK);
                StartFlyShip(orgrimmar);
                StopFlyShip(CheckUnfriendlyShip(me,_instance,DATA_GB_MURADIN_BRONZEBEARD));
                break;
            case EVENT_OUTRO_HORDE_2:
                StopFlyShip(orgrimmar);
                me->SummonGameObject(RAID_MODE(GO_CAPITAN_CHEST_H_10N, GO_CAPITAN_CHEST_H_25N, GO_CAPITAN_CHEST_H_10H, GO_CAPITAN_CHEST_H_25H), -590.200022f, 2241.193115f, 539.588269f, 0, 0, 0, 0, 0, 100000);
                me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
                me->GetMotionMaster()->MovePoint(0, -590.700f, 2213.01f, 539.1f);
                break;
            case EVENT_OUTRO_HORDE_3:
                me->GetMotionMaster()->MovePoint(1, -555.59f, 2213.01f, 539.28f);
                break;
            case EVENT_FAIL:
                TeleportPlayers(map, HORDE);
                events.ScheduleEvent(EVENT_RESTART_EVENT, 2000);
                _instance->SetBossState(DATA_GUNSHIP_EVENT, NOT_STARTED);
                break;
            case EVENT_RESTART_EVENT:
                if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE){
                    _instance->SetBossState(DATA_GUNSHIP_EVENT, FAIL);
                    RestartEvent(orgrimmar, CheckUnfriendlyShip(me,_instance,DATA_GB_MURADIN_BRONZEBEARD), map, HORDE);
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
            case EVENT_SPAWN_MAGE:
                //Prevent from spawning multiple Mages
                if (me->GetEntry() != NPC_GB_KORKRON_BATTLE_MAGE){
                    Talk(SAY_NEW_BATTLE_MAGE_SPAWNED);
                    //orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_BATTLE_MAGE, 15.03016f, 0.00016f, 37.70952f, 1.55138f);
                }
                break;
            case EVENT_RESPAWN_ROCKETEER:
                Talk(SAY_NEW_ROCKETEERS_SPAWNED);
                if (RocketerCount == 2)
                {/*
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_ROCKETEER, -11.44849f, -25.71838f, 33.64343f, 1.49248f);
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_ROCKETEER, 12.30336f, -25.69653f, 35.32373f, 1.49248f);
                */}
                if (RocketerCount == 4)
                {/*
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_ROCKETEER, -11.44849f, -25.71838f, 33.64343f, 1.49248f);
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_ROCKETEER, 12.30336f, -25.69653f, 35.32373f, 1.49248f);
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_ROCKETEER, -3.44849f, -25.71838f, 34.21082f, 1.49248f);
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_ROCKETEER, 3.30336f, -25.69653f, 35.32373f, 1.49248f);
                */}
                break;
            case EVENT_RESPAWN_AXES_RIFLEMEN:
                Talk(SAY_NEW_AXETHROWER_SPAWNED);
                if (AxesCount == 4)
                {/*
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, -3.170555f, 28.30652f, 34.21082f, 1.66527f);
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, -12.0928f, 27.65942f, 33.58557f, 1.66527f);
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, 14.92804f, 26.18018f, 35.47803f, 1.66527f);
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, 24.70331f, 25.36584f, 35.97845f, 1.66527f);
                */}
                if (AxesCount == 6)
                {/*
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, -3.170555f, 28.30652f, 34.21082f, 1.66527f);
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, -12.0928f, 27.65942f, 33.58557f, 1.66527f);
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, 14.92804f, 26.18018f, 35.47803f, 1.66527f);
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, 24.70331f, 25.36584f, 35.97845f, 1.66527f);
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, 19.92804f, 27.18018f, 35.47803f, 1.66527f);
                    orgrimmar->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, -7.70331f, 28.36584f, 33.88557f, 1.66527f);
                */}
                break;
        }
    }
    
    if (!me->GetCurrentSpell(CURRENT_MELEE_SPELL))
        DoCastVictim(SPELL_CLEAVE);
    
    DoMeleeAttackIfReady();
}