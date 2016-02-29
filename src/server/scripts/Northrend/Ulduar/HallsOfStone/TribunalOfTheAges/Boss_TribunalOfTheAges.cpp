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

#include "Boss_TribunalOfTheAges.h"
typedef mob_tribuna_controller::mob_tribuna_controllerAI ControllerAI;


void ControllerAI::Reset()
{
    events.Reset();

    #ifdef DEBUG_TRIBUNAL_OF_AGES
    debugSpells = false;
    debugSpellTimer[0] = 0;
    debugSpellTimer[1] = 0;
    debugSpellTimer[2] = 0;
    #endif

    KaddrakEye = false;
    MarnakEye = false;

    if (instance)
    {
        instance->HandleGameObject(instance->GetData64(DATA_GO_SKY_FLOOR), false);
        instance->HandleGameObject(instance->GetData64(DATA_GO_TRIBUNAL_CONSOLE), false);
    }
}

void ControllerAI::ResetEvent(bool finished)
{
    if (!instance)
        return;

    Creature* pCreature = NULL;
    if (pCreature = me->FindNearestCreature(CREATURE_SEARING_GAZE_TARGET, 100.0f))
        pCreature->DespawnOrUnsummon();

    if (pCreature = me->FindNearestCreature(CREATURE_SHADOW_ORB, 100.0f))
        pCreature->DespawnOrUnsummon();

    for (uint8 i = 0; i < 2; ++i)
    {
        if (pCreature = me->GetCreature((*me), instance->GetData64(DATA_LIGHTNING_TRIGGER_LEFT+i)))
        {
            if (finished)
                pCreature->DealDamage(pCreature, pCreature->GetHealth());
            else
                pCreature->RemoveAurasDueToSpell(SPELL_VISUAL_LIGHTNING_TRIGGER);
        }
    }

    if (!finished)
    {
        ActivateOrSwitchBossGO(BOSS_ALL, BOSS_GO_STATE_NOTHING);
        me->CastSpell(me, SPELL_KILL_TRIBUNAL_ADD, true);
        Reset();
    }
}

void ControllerAI::JustDied(Unit* killer)
{
    ResetEvent(true);
}

void ControllerAI::UpdateAI(const uint32 diff)
{
    if (!events.Empty())
    {
        events.Update(diff);
        ExecuteEvents();
    }
}

void ControllerAI::ExecuteEvents()
{
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_KADDRAK_CAST_SPELL:
            {
                KaddrakSpell();
                #ifdef DEBUG_TRIBUNAL_OF_AGES
                events.ScheduleEvent(EVENT_KADRAK_CAST_SPELL, debugSpellTimer[0]!=0?debugSpellTimer[0] : TIMER_SPELL_KADRAK);
                #else
                events.ScheduleEvent(EVENT_KADDRAK_CAST_SPELL, TIMER_SPELL_KADDRAK);
                #endif
            }break;

            case EVENT_MARNAK_CAST_SPELL:
            {
                MarnakSpell();

                #ifdef DEBUG_TRIBUNAL_OF_AGES
                events.ScheduleEvent(EVENT_MARNAK_CAST_SPELL, debugSpellTimer[1]!=0?debugSpellTimer[1] : TIMER_SPELL_MARNAK);
                #else
                events.ScheduleEvent(EVENT_MARNAK_CAST_SPELL, TIMER_SPELL_MARNAK);
                #endif
            }break;

            case EVENT_ABEDNEUM_CAST_SPELL:
            {
                AbedneumSpell();
                #ifdef DEBUG_TRIBUNAL_OF_AGES
                events.ScheduleEvent(EVENT_ABEDNEUM_CAST_SPELL, debugSpellTimer[2]!=0?debugSpellTimer[2] : TIMER_SPELL_ABEDNEUM);
                #else
                events.ScheduleEvent(EVENT_ABEDNEUM_CAST_SPELL, TIMER_SPELL_ABEDNEUM);
                #endif
            }break;

            case EVENT_CHECK_PLAYERS:
            {
                if (!CheckPlayers())
                {
                    Creature* pCreature = NULL;
                    if (pCreature = me->FindNearestCreature(CREATURE_BRANN, 50.0f))
                    {
                        pCreature->Kill(pCreature);
                        Reset();
                    }
                }
                else
                    events.ScheduleEvent(EVENT_CHECK_PLAYERS, TIMER_EVENT_CHECK_PLAYERS);
            }break;
        }
    }
}

void ControllerAI::DoAction(const int32 action)
{
    switch (action)
    {
        case ACTION_START_EVENT:     { StartEvent(); }break;
        case ACTION_RESET_EVENT:     { ResetEvent(); }break;
        case ACTION_ENABLE_KADDRAK:  { ActivateBoss(EVENT_KADDRAK_CAST_SPELL);  }break;
        case ACTION_ENABLE_MARNAK:   { ActivateBoss(EVENT_MARNAK_CAST_SPELL);   }break;
        case ACTION_ENABLE_ABEDNEUM: { ActivateBoss(EVENT_ABEDNEUM_CAST_SPELL); }break;
        case ACTION_SET_ALL_GO_WHITE:{ ActivateOrSwitchBossGO(BOSS_ALL, BOSS_GO_STATE_ACTIVE_WHITE); }break;
    }
}

void ControllerAI::StartEvent()
{
    DoZoneInCombat();
    events.ScheduleEvent(EVENT_CHECK_PLAYERS, TIMER_EVENT_CHECK_PLAYERS);
}

void ControllerAI::ActivateBoss(Events bossEvent)
{
    switch (bossEvent)
    {
        case EVENT_KADDRAK_CAST_SPELL:
        {
            events.ScheduleEvent(bossEvent, 1.5*IN_MILLISECONDS);
            if (instance)
            {
                Creature* pCreature = NULL;
                for (uint8 i = 0; i < 2; ++i)
                {
                    if (pCreature = me->GetCreature((*me), instance->GetData64(DATA_LIGHTNING_TRIGGER_LEFT+i)))
                        pCreature->CastSpell(pCreature, SPELL_VISUAL_LIGHTNING_TRIGGER, true);
                }
            }
        }break;
        case EVENT_MARNAK_CAST_SPELL:
        case EVENT_ABEDNEUM_CAST_SPELL: { events.ScheduleEvent(bossEvent, 10*IN_MILLISECONDS);  }break;
    }

    ActivateOrSwitchBossGO(BossId(bossEvent-1), BOSS_GO_STATE_ACTIVE_RED);
}

void ControllerAI::KaddrakSpell()
{
    if (!instance)
        return;

    uint64 currentEye = 0;
    if (KaddrakEye)
    {
        currentEye = instance->GetData64(DATA_KADDRAK_FIRST);
        KaddrakEye = false;
    }
    else
    {
        currentEye = instance->GetData64(DATA_KADDRAK_SECOND);
        KaddrakEye = true;
    }

    if (Creature* pKaddrak = me->GetCreature((*me), currentEye))
    {
        if (pKaddrak->isAlive())
            pKaddrak->CastSpell(pKaddrak, DUNGEON_MODE(SPELL_GLARE_OF_THE_TRIBUNAL, H_SPELL_GLARE_OF_THE_TRIBUNAL), true);
    }
}

void ControllerAI::MarnakSpell()
{
    if (!instance)
        return;

    uint64 currentEye = 0;
    if (MarnakEye)
    {
        currentEye = instance->GetData64(DATA_MARNAK_FIRST);
        MarnakEye = false;
    }
    else
    {
        currentEye = instance->GetData64(DATA_MARNAK_SECOND);
        MarnakEye = true;
    }

    if (Creature* pMarnak = me->GetCreature((*me), currentEye))
    {
        if (Creature* summon = me->SummonCreature(CREATURE_SHADOW_ORB, pMarnak->GetPositionX(), pMarnak->GetPositionY(), pMarnak->GetPositionZ()-3.0f, 0.0f))
        {
            summon->CastSpell(summon, SPELL_VISUAL_SHADOW_ORB, true);
            summon->GetMotionMaster()->MovePoint(1, TribunalShadowOrb);
        }
    }
}

void ControllerAI::AbedneumSpell()
{
    std::list<Player*> PlList = me->GetNearestPlayersList(100.0f);
    std::list<Player*>::const_iterator itr = PlList.begin();
    if (PlList.size() > 1)
        advance(itr, rand()%PlList.size());

    if (Player* target = (*itr))
    {
        if (Creature* summon = 
            me->SummonCreature(CREATURE_SEARING_GAZE_TARGET, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.0f,
            TEMPSUMMON_TIMED_DESPAWN, TIMER_SPELL_ABEDNEUM))
        {
            #ifdef DEBUG_TRIBUNAL_OF_AGES
            if (debugSpells)
                summon->SetDisplayId(169);
            else
                summon->SetDisplayId(11686);
            #else
            summon->SetDisplayId(11686);
            #endif

            summon->CastSpell(summon, DUNGEON_MODE(SPELL_SEARING_GAZE, H_SPELL_SEARING_GAZE), true);
        }
    }
}

void ControllerAI::ActivateOrSwitchBossGO(BossId bossId, BossGOState state)
{
    if (!instance)
        return;

    GameObject* pObject;
    uint8 maxCount = 1;
    uint8 dataIndex = 0;

    switch (bossId)
    {
        case BOSS_ALL:      { maxCount  = 3; }break;
        case BOSS_MARNAK:   { dataIndex = 1; }break;
        case BOSS_ABEDNEUM: { dataIndex = 2; }break;
    }

    for (uint8 i = 0; i < maxCount; ++i)
    {
        if (pObject = GameObject::GetGameObject((*me), instance->GetData64(DATA_GO_KADDRAK+i+dataIndex)))
        {
            switch (state)
            {
                case BOSS_GO_STATE_NOTHING:
                {
                    WorldPacket data(SMSG_GAMEOBJECT_CUSTOM_ANIM, 8+4);
                    data << pObject->GetGUID();
                    data << (uint32)(1);
                    pObject->SendMessageToSet(&data, true);
                    pObject->SetByteValue(GAMEOBJECT_BYTES_1, 0, 1);
                }break;

                case BOSS_GO_STATE_ACTIVE_RED:
                {
                    WorldPacket data(SMSG_GAMEOBJECT_CUSTOM_ANIM, 8+4);
                    data << pObject->GetGUID();
                    data << (uint32)(0);
                    pObject->SendMessageToSet(&data, true);
                }break;

                case BOSS_GO_STATE_ACTIVE_WHITE: { pObject->SetByteValue(GAMEOBJECT_BYTES_1, 0, 0); }break;
            }
        }
    }
}

bool ControllerAI::CheckPlayers()
{
    if (!me->FindNearestPlayer(100.0f))
        return false;
    return true;
}

#ifdef DEBUG_TRIBUNAL_OF_AGES
void ControllerAI::DeactivateBoss(Events bossEvent)
{
    events.CancelEvent(bossEvent);
    ActivateOrSwitchBossGO(BossId(bossEvent-1), BOSS_GO_STATE_NOTHING);
}

void ControllerAI::SetNewSpellTimer(Events bossEvent, uint32 timer)
{
    events.CancelEvent(bossEvent);
    events.ScheduleEvent(bossEvent, timer);
    debugSpellTimer[bossEvent-1] = timer;
    debugSpells = true;
}
#endif

void AddSC_tribunal_of_the_ages()
{
    new mob_tribuna_controller();
}
