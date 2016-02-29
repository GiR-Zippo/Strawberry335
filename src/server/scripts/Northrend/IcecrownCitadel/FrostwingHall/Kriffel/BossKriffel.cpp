#include "Kriffel.h"

class KriffelAI : public BossAI
{
public:
    KriffelAI(Creature* creature) : BossAI(creature, DATA_KRIFFEL)
    {
    }

    void KilledUnit(Unit* who)
    {
        DoScriptText(TEXT_SLAY, me);
        BossAI::KilledUnit(who);
    }


    void EnterEvadeMode()
    {
        _DespawnAtEvade();
        BossAI::EnterEvadeMode();
    }


    void Reset()
    {
        creatures.clear();
        creatures2.clear();
        creatures3.clear();
        phase = 0;
        phase2 = 0;
        enrageCount = 0;
        phase3 = 0;
        me->RemoveAllAuras();
        me->SetReactState(REACT_DEFENSIVE);

        BossAI::Reset();
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(TEXT_DEATH, me);
        // SpawnChest();
        instance->DoCompleteAchievement(RAID_MODE(4531, 4604, 4628, 4632));
        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LEGION_FLAME);
        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PACKT_OF_THE_DARKFALLEN);
        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PFAEHLEN);
        BossAI::JustDied(killer);
    }

    void DamageTaken(Unit* damageDealer, uint32& damage)
    {
        if (damageDealer == me)
            return;

        if (damageDealer->HasAura(SPELL_AURA_OF_DESIRE) && !damageDealer->HasAura(SPELL_INCITE_TERROR))
        {
            int32 bp0 = damage * 0.25;
            if (damageDealer->HasAura(SPELL_LEGION_FLAME) || damageDealer->HasAura(SPELL_PACKT_OF_THE_DARKFALLEN))
                bp0 = damage * 0.10;
            me->CastCustomSpell(damageDealer, SPELL_AURA_OF_DESIRE_DAMAGE, &bp0, NULL, NULL, true);
        }

        BossAI::DamageTaken(damageDealer, damage);
    }

    void MovementInform(uint32 type, uint32 point)
    {
        if (type != POINT_MOTION_TYPE && type != EFFECT_MOTION_TYPE)
            return;

        switch (point)
        {
        case POINT_MID:
            // me->SetFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE );
            break;
        case POINT_PHASE_2:
        {
            energys.clear();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            DoCast(me, SPELL_AURA_OF_DESIRE);
            DoCast(me, SPELL_FROST_AURA);
            DoSummon(NPC_VISUAL_STORM, midPoint, 56000, TEMPSUMMON_TIMED_DESPAWN);
            Creature* storm = me->FindNearestCreature(NPC_VISUAL_STORM, 30.0f);
            if (storm)
                storm->CastSpell(storm, SPELL_WINTER_CHANNEL);
            events.ScheduleEvent(EVENT_FIRE, 7000, 0, PHASE_2);
            if (IsHeroic())
                events.ScheduleEvent(EVENT_FIRE_2, 20000, 0, PHASE_2);
            events.ScheduleEvent(EVENT_FIRE_3, 33000, 0, PHASE_2);
            events.ScheduleEvent(EVENT_PACKT, 18000, 0, PHASE_2);
            events.ScheduleEvent(EVENT_SUMMON_FROSTORB, 10000, 0, PHASE_2);
            events.ScheduleEvent(EVENT_PACKT_2, 45000, 0, PHASE_2);
            break;
        }
        }

        BossAI::MovementInform(type, point);
    }

    void EnterCombat(Unit* who)
    {
        DoScriptText(TEXT_AGGRO, me);
        DoScriptText(TEXT_EMOTE_START, me);
        me->SetInCombatWithZone();

        events.SetPhase(PHASE_1);
        events.ScheduleEvent(EVENT_KRACHEN, 16000, EVENT_GROUP_P1, PHASE_1);
        events.ScheduleEvent(EVENT_BERSERK, 360000);
        events.ScheduleEvent(EVENT_PFAEHLEN, 2000);
        me->GetMotionMaster()->MovePoint(POINT_MID, midPoint);

        BossAI::EnterCombat(who);
    }

    void DoAction(const int32 action)
    {
        switch (action)
        {
        case ACTION_INTRO:
            DoCast(SPELL_ARRIVAL);
            DoScriptText(TEXT_INTRO, me);
            break;
        }

        BossAI::DoAction(action);
    }

    void ExecuteEvent(uint32 const eventId)
    {
        switch (eventId)
        {
        case EVENT_BERSERK:
            DoScriptText(TEXT_EMOTE_BERSERK, me);
            DoCast(me, SPELL_BERSERK);
            break;
        case EVENT_PFAEHLEN:
        {
            DoCastVictim(SPELL_PFAEHLEN);

            if (IsHeroic())
            {
                uint32 num[3] = { urand(0, 1), urand(2, 3), urand(4, 5) };
                for (uint32 i = 0; i < static_cast<uint32>(Is25ManRaid() ? 9 : 4); i++)
                {
                    Position point;
                    if ((i % 2) == 0)
                        me->GetRandomPoint(energyPoints[num[i % 3]], 13.0f, point);
                    else
                        me->GetRandomPoint(midPoint, 33.0f, point);
                    DoSummon(NPC_VERDERBNIS, point, 0, TEMPSUMMON_MANUAL_DESPAWN);
                }
            }
            events.ScheduleEvent(EVENT_PFAEHLEN, urand(5000, 8000));
            break;
        }
        case EVENT_INTRO_0:
            events.DelayEvents(20000);
            DoScriptText(TEXT_INTRO, me);
            events.ScheduleEvent(EVENT_INTRO_1, 20000);
            break;
        case EVENT_INTRO_1:
            justSummoned = false;
            me->SetReactState(REACT_AGGRESSIVE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            break;
        case EVENT_KRACHEN:
            me->NearTeleportTo(midPoint);
            DoScriptText(TEXT_EMOTE_KRACHEN, me);
            phase = urand(0, 6);
            me->SetReactState(REACT_PASSIVE);
            me->SetFacingTo(dir[phase]);
            GetCreatureListWithEntryInGrid(creatures, me, creatureKrachen[phase], 70.0f);
            for (std::list<Creature*>::iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
            {
                Creature* stalker = *itr;
                stalker->GetAI()->DoAction(ACTION_EXPLODE_START);
            }
            DoCast(SPELL_KRACHEN);
            events.ScheduleEvent(EVENT_RESET, 3500);
            events.ScheduleEvent(EVENT_KRACHEN_2, 20000, EVENT_GROUP_P1, PHASE_1);
            events.ScheduleEvent(EVENT_MARK_ENERGY, 1000, EVENT_GROUP_P1, PHASE_1);
            break;
        case EVENT_KRACHEN_2:
            me->NearTeleportTo(midPoint);
            DoScriptText(TEXT_EMOTE_KRACHEN, me);
            do
            {
                phase2 = urand(0, 6);
            } while (phase == phase2);
            me->SetReactState(REACT_PASSIVE);
            me->SetFacingTo(dir[phase2]);
            GetCreatureListWithEntryInGrid(creatures2, me, creatureKrachen[phase2], 70.0f);
            for (std::list<Creature*>::iterator itr = creatures2.begin(); itr != creatures2.end(); ++itr)
            {
                Creature* stalker = *itr;
                stalker->GetAI()->DoAction(ACTION_EXPLODE_START);
            }
            DoCast(SPELL_KRACHEN);
            events.ScheduleEvent(EVENT_RESET, 3500);
            events.ScheduleEvent(EVENT_KRACHEN_3, 20000, EVENT_GROUP_P1, PHASE_1);
            events.ScheduleEvent(EVENT_MARK_ENERGY, 1000, EVENT_GROUP_P1, PHASE_1);
            break;
        case EVENT_KRACHEN_3:
            me->NearTeleportTo(midPoint);
            DoScriptText(TEXT_EMOTE_KRACHEN, me);
            do
            {
                phase3 = urand(0, 6);
            } while (phase2 == phase3 || phase == phase3);
            me->SetReactState(REACT_PASSIVE);
            me->SetFacingTo(dir[phase3]);
            GetCreatureListWithEntryInGrid(creatures3, me, creatureKrachen[phase3], 70.0f);
            for (std::list<Creature*>::iterator itr = creatures3.begin(); itr != creatures3.end(); ++itr)
            {
                Creature* stalker = *itr;
                stalker->GetAI()->DoAction(ACTION_EXPLODE_START);
            }
            DoCast(SPELL_KRACHEN);

            events.ScheduleEvent(EVENT_RESET, 3500);
            events.ScheduleEvent(EVENT_BIG_KRACHEN, 20000, EVENT_GROUP_P1, PHASE_1);
            events.ScheduleEvent(EVENT_MARK_ENERGY, 1000, EVENT_GROUP_P1, PHASE_1);
            break;
        case EVENT_BIG_KRACHEN:
            me->NearTeleportTo(midPoint);
            DoScriptText(TEXT_SPECIAL, me);
            creatures.merge(creatures2);
            creatures.merge(creatures3);

            for (std::list<Creature*>::iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
            {
                Creature* stalker = *itr;
                stalker->GetAI()->DoAction(ACTION_WARN_EXPLODE);
            }

            for (std::list<Creature*>::iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
            {
                Creature* stalker = *itr;
                stalker->GetAI()->DoAction(ACTION_EXPLODE_BIG);
            }
            DoCast(SPELL_BIG_PRE);
            phase = 0;
            phase2 = 0;
            phase3 = 0;
            creatures.clear();
            creatures2.clear();
            creatures3.clear();
            events.ScheduleEvent(EVENT_KRACHEN, 20000, EVENT_GROUP_P1, PHASE_1);
            events.ScheduleEvent(EVENT_MARK_ENERGY, 5000, EVENT_GROUP_P1, PHASE_1);
            events.ScheduleEvent(EVENT_CHANGE_P2, 15000, EVENT_GROUP_P1, PHASE_1);
            break;
        case EVENT_RESET:
            me->SetReactState(REACT_AGGRESSIVE);
            me->SetFacingToObject(me->getVictim());
            break;
        case EVENT_MARK_ENERGY:
        {
                                  DoScriptText(TEXT_EMOTE_ENERGY, me);
                                  if (Is25ManRaid())
                                  {
                                      Position point1, point2, point3, point4, point5, point6;
                                      me->GetRandomPoint(energyPoints[0], 11.0f, point1);
                                      me->GetRandomPoint(energyPoints[1], 11.0f, point2);
                                      me->GetRandomPoint(energyPoints[2], 11.0f, point3);
                                      me->GetRandomPoint(energyPoints[3], 11.0f, point4);
                                      me->GetRandomPoint(energyPoints[4], 11.0f, point5);
                                      me->GetRandomPoint(energyPoints[5], 11.0f, point6);
                                      DoSummon(NPC_PRE_ENERGY, point1, 9000);
                                      DoSummon(NPC_PRE_ENERGY, point2, 9000);
                                      DoSummon(NPC_PRE_ENERGY, point3, 9000);
                                      DoSummon(NPC_PRE_ENERGY, point4, 9000);
                                      DoSummon(NPC_PRE_ENERGY, point5, 9000);
                                      DoSummon(NPC_PRE_ENERGY, point6, 9000);
                                  }
                                  else
                                  {
                                      Position point1, point2, point3;
                                      uint32 randomizer = urand(0, 1);
                                      me->GetRandomPoint(energyPoints[randomizer], 15.0f, point1);
                                      me->GetRandomPoint(energyPoints[randomizer + 2], 15.0f, point2);
                                      me->GetRandomPoint(energyPoints[randomizer + 4], 15.0f, point3);
                                      DoSummon(NPC_PRE_ENERGY, point1, 9000);
                                      DoSummon(NPC_PRE_ENERGY, point2, 9000);
                                      DoSummon(NPC_PRE_ENERGY, point3, 9000);
                                  }
                                  std::list<Creature*> energy;
                                  GetCreatureListWithEntryInGrid(energy, me, NPC_PRE_ENERGY, 100.0f);
                                  for (std::list<Creature*>::iterator itr = energy.begin(); itr != energy.end(); ++itr)
                                  {
                                      Creature* stalker = *itr;
                                      DoCast(stalker, SPELL_SPIRAL, true);
                                      stalker->GetAI()->DoAction(ACTION_APPEAR);
                                  }
                                  energy.clear();
                                  break;
        }
        case EVENT_RECKONING:
        {
                                if (Unit* player = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me, true)))
                                    DoCast(player, SPELL_DARK_RECKONING);
                                events.ScheduleEvent(EVENT_RECKONING, 10000, 0, PHASE_2);
                                break;
        }
        case EVENT_CHANGE_P2:
            GetCreatureListWithEntryInGrid(energys, me, NPC_PRE_ENERGY, 70.0f);
            for (std::list<Creature*>::iterator itr = energys.begin(); itr != energys.end(); ++itr)
            {
                Creature* stalker = *itr;
                stalker->DespawnOrUnsummon(100);
            }
            DoScriptText(TEXT_PHASE, me);
            DoScriptText(TEXT_EMOTE_PHASE_2, me);
            DoCast(me, SPELL_INCITE_TERROR, true);
            events.SetPhase(PHASE_2);
            events.ScheduleEvent(EVENT_CHANGE_P1, 60000, 0, PHASE_2);
            events.DelayEvents(65000, EVENT_GROUP_P1);
            me->GetMotionMaster()->MovePoint(POINT_PHASE_2, midPoint);
            break;
        case EVENT_CHANGE_P1:
            DoScriptText(TEXT_PHASE_1, me);
            DoScriptText(TEXT_EMOTE_PHASE_1, me);
            events.SetPhase(PHASE_1);
            me->RemoveAllAuras();
            me->RemoveAurasDueToSpell(SPELL_FROST_AURA);
            me->RemoveAurasDueToSpell(SPELL_AURA_OF_DESIRE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->GetMotionMaster()->MoveChase(me->getVictim());
            break;
        case EVENT_SUMMON_FROSTORB:
        {
                                      Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, -5.0f, true);
                                      if (target)
                                      {
                                          me->SummonCreature(NPC_FROSTORB, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                                          target->ToPlayer()->Yell("Frostessenz auf mir!", 0);
                                      }
                                      events.ScheduleEvent(EVENT_SUMMON_FROSTORB, 20000, 0, PHASE_2);
                                      break;
        }
        case EVENT_FIRE_3:
        case EVENT_FIRE_2:
        case EVENT_FIRE:
        {
                           std::list<Player*> targets;
                           SelectRandomTarget(&targets);
                           uint32 targetCount = 1;
                           if (Is25ManRaid())
                               targetCount += 1;
                           if (IsHeroic())
                               targetCount += 1;
                           Trinity::Containers::RandomResizeList<Player*>(targets, targetCount);
                           for (std::list<Player*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                           {
                               Player* player = *itr;
                               DoCast(*itr, SPELL_LEGION_FLAME, true);
                               player->Yell("Brennendes Eis auf mir!", 0);
                           }
                           break;
        }
        case EVENT_PACKT_2:
        case EVENT_PACKT:
        {
                            std::list<Player*> targets;
                            SelectRandomTarget(&targets);
                            uint32 targetCount = 2;
                            if (Is25ManRaid())
                                ++targetCount;
                            if (IsHeroic())
                                ++targetCount;
                            Trinity::Containers::RandomResizeList<Player*>(targets, targetCount);
                            if (targets.size() > 1)
                            for (std::list<Player*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                                DoCast(*itr, SPELL_PACKT_OF_THE_DARKFALLEN, true);
                            break;
        }
        }
    }

    void SpawnChest()
    {
        uint32 chestEntry;
        if (instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
            chestEntry = RAID_MODE(GO_LOOT_10N_A, GO_LOOT_25N_A, GO_LOOT_10H_A, GO_LOOT_25H_A);
        else
            chestEntry = RAID_MODE(GO_LOOT_10N_H, GO_LOOT_25N_H, GO_LOOT_10H_H, GO_LOOT_25H_H);
        me->SummonGameObject(chestEntry, loot.GetPositionX(), loot.GetPositionY(), loot.GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
    }

private:
    uint32 phase;
    uint32 phase2;
    uint32 phase3;
    uint32 enrageCount;
    std::list<Creature*> creatures;
    std::list<Creature*> creatures2;
    std::list<Creature*> creatures3;
    std::list<Creature*> energys;
    std::list<Creature*> enrage;
    bool justSummoned;
    Player* SelectRandomTarget(std::list<Player*>* targetList = NULL)
    {
        std::list<HostileReference*> const& threatlist = me->getThreatManager().getThreatList();
        std::list<Player*> tempTargets;

        if (threatlist.empty())
            return NULL;

        for (std::list<HostileReference*>::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
        if (Unit* refTarget = (*itr)->getTarget())
        if (refTarget != me->getVictim() && refTarget->GetTypeId() == TYPEID_PLAYER && !refTarget->HasAura(SPELL_PACKT_OF_THE_DARKFALLEN) && !refTarget->HasAura(SPELL_LEGION_FLAME))
            tempTargets.push_back(refTarget->ToPlayer());

        if (tempTargets.empty())
            return NULL;

        if (targetList)
        {
            *targetList = tempTargets;
            return NULL;
        }

        return Trinity::Containers::SelectRandomContainerElement(tempTargets);
    }
};

class Kriffel : public CreatureScript
{
public:
    Kriffel() : CreatureScript("custom_kriffel") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new KriffelAI(creature);
    };
};

void LoadBossKriffel()
{
    new Kriffel();
}
