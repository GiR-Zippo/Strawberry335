#include "Npc_TerenasMenethil.h"

bool NpcTerenasMenethilAI::CanAIAttack ( const Unit* target ) const
{
    return target->GetEntry() != NPC_THE_LICH_KING;
}

void NpcTerenasMenethilAI::DoAction ( const int32 action )
{
    switch (action)
    {
        case ACTION_FROSTMOURNE_INTRO:
            me->setActive(true);
            if (!IsHeroic())
                me->SetHealth(me->GetMaxHealth() / 2);
            DoCast(me, SPELL_LIGHTS_FAVOR);
            _events.Reset();
            _events.ScheduleEvent(EVENT_FROSTMOURNE_TALK_1, 2000, PHASE_FROSTMOURNE);
            _events.ScheduleEvent(EVENT_FROSTMOURNE_TALK_2, 11000, PHASE_FROSTMOURNE);
            if (!IsHeroic())
            {
                _events.ScheduleEvent(EVENT_DESTROY_SOUL, 60000, PHASE_FROSTMOURNE);
                _events.ScheduleEvent(EVENT_FROSTMOURNE_TALK_3, 25000);
            }
            break;
        case ACTION_TELEPORT_BACK:
            me->CastSpell((Unit*)NULL, SPELL_RESTORE_SOUL, TRIGGERED_NONE);
            me->DespawnOrUnsummon(3000);
            break;
        default:
            break;
    }
}

void NpcTerenasMenethilAI::EnterEvadeMode()
{
    // no running back home
    if (!me->isAlive())
        return;

    me->DeleteThreatList();
    me->CombatStop(false);
}

void NpcTerenasMenethilAI::DamageTaken ( Unit* attacker, uint32& damage )
{
    if (attacker->GetEntry() == NPC_WICKED_SPIRIT)
    {
        if (Creature* pCreature = attacker->ToCreature())
        {
            pCreature->Kill(pCreature);
            pCreature->SetPosition(pCreature->GetHomePosition());
            pCreature->Respawn();
        }
    }

    if (damage >= me->GetHealth())
    {
        damage = me->GetHealth() - 1;
        if (!me->HasAura(SPELL_TERENAS_LOSES_INSIDE) && !IsHeroic())
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            DoCast(SPELL_TERENAS_LOSES_INSIDE);
            _events.ScheduleEvent(EVENT_TELEPORT_BACK, 1000);
            if (Creature* warden = me->FindNearestCreature(NPC_SPIRIT_WARDEN, 20.0f))
            {
                warden->CastSpell((Unit*)NULL, SPELL_DESTROY_SOUL, TRIGGERED_NONE);
                warden->DespawnOrUnsummon(2000);
            }

            me->DespawnOrUnsummon(2000);
        }
    }
}

void NpcTerenasMenethilAI::IsSummonedBy ( Unit* )
{
    _events.Reset();
    _events.SetPhase(PHASE_OUTRO);
    if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
        me->SetFacingToObject(lichKing);

    _events.ScheduleEvent(EVENT_OUTRO_TERENAS_TALK_1, 2000, 0, PHASE_OUTRO);
    _events.ScheduleEvent(EVENT_OUTRO_TERENAS_TALK_2, 14000, 0, PHASE_OUTRO);
}

void NpcTerenasMenethilAI::UpdateAI ( const uint32 diff )
{
    UpdateVictim();

    _events.Update(diff);

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_FROSTMOURNE_TALK_1:
                Talk(SAY_TERENAS_INTRO_1);
                if (IsHeroic())
                    DoCastAOE(SPELL_RESTORE_SOULS);
                else
                {
                    // Trigger 100% damage aura
                    if (Player* pPlayer = me->FindNearestPlayer(100.0f))
                        pPlayer->AddAura(SPELL_HARVESTED_SOUL_PLAYER, pPlayer);
                }
                break;
            case EVENT_FROSTMOURNE_TALK_2:
                Talk(SAY_TERENAS_INTRO_2);
                break;
            case EVENT_FROSTMOURNE_TALK_3:
                Talk(SAY_TERENAS_INTRO_3);
                break;
            case EVENT_OUTRO_TERENAS_TALK_1:
                Talk(SAY_TERENAS_OUTRO_1);
                break;
            case EVENT_OUTRO_TERENAS_TALK_2:
                Talk(SAY_TERENAS_OUTRO_2);
                DoCastAOE(SPELL_MASS_RESURRECTION);
                if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
                {
                    lichKing->AI()->DoAction(ACTION_FINISH_OUTRO);
                    lichKing->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    if (Creature* tirion = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HIGHLORD_TIRION_FORDRING)))
                        tirion->AI()->AttackStart(lichKing);
                }
                break;
            case EVENT_DESTROY_SOUL:
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                if (Creature* warden = me->FindNearestCreature(NPC_SPIRIT_WARDEN, 20.0f))
                    warden->CastSpell((Unit*)NULL, SPELL_DESTROY_SOUL, TRIGGERED_NONE);
                DoCast(SPELL_TERENAS_LOSES_INSIDE);
                _events.ScheduleEvent(EVENT_TELEPORT_BACK, 1000);
                break;
            case EVENT_TELEPORT_BACK:
                if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
                    lichKing->AI()->DoAction(ACTION_TELEPORT_BACK);
                break;
            default:
                break;
        }
    }

    // fighting Spirit Warden
    if (me->isInCombat())
        DoMeleeAttackIfReady();
}