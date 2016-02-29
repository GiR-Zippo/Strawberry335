#include "Npc_SindragosaWard.h"

NpcSindragosaWardAI::NpcSindragosaWardAI(Creature* creature) : BossAI(creature, DATA_SINDRAGOSA_GAUNTLET)
{
}

void NpcSindragosaWardAI::Reset()
{
    _Reset();
    me->SetReactState(REACT_PASSIVE);
    _isEventInProgressOrDone = false;
    _waveNumber = 0;
    _spawnThirtySeconds = false;
    _thirtySeconds = 0;
    _thirtySecondsSpawns = 15;
    _thirtySecondsSpawnsCount = 0;
}

void NpcSindragosaWardAI::DoAction(int32 action)
{
    if (action == ACTION_START_GAUNTLET)
        if (!_isEventInProgressOrDone)
            if (!IsAnyPlayerOutOfRange())
                DoZoneInCombat(me, 150.0f);
}

void NpcSindragosaWardAI::EnterCombat(Unit* victim)
{
    if (!instance->CheckRequiredBosses(DATA_SINDRAGOSA_GAUNTLET, victim->ToPlayer()))
    {
        EnterEvadeMode();
        instance->DoCastSpellOnPlayers(LIGHT_S_HAMMER_TELEPORT);
        return;
    }

    _EnterCombat();
    _isEventInProgressOrDone = true;
    _waveNumber = 1;
    DoSummonWave(_waveNumber);
    events.ScheduleEvent(EVENT_NEXT_WAVE, 10000);
    events.ScheduleEvent(EVENT_UPDATE_CHECK, 5000);
}

void NpcSindragosaWardAI::DoSummonWave(uint8 number)
{
    switch (number)
    {
        case 1:
            me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[0], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[2], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[4], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[6], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[1], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[3], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[5], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[7], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 9:
        case 10:
        case 11:
            for (uint8 i = 0; i < SINDRAGOSA_GAUNTLET_SPAWN_MAX; i++)
                me->SummonCreature(NPC_NERUBAR_BROODLING, SindragosaGauntletSpawn[i], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            break;
        case 8:
            me->SummonCreature(NPC_FROSTWARDEN_SORCERESS, SindragosaGauntletFrostwarden[0], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_FROSTWARDEN_WARRIOR, SindragosaGauntletFrostwarden[1], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_FROSTWARDEN_WARRIOR, SindragosaGauntletFrostwarden[2], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_FROSTWARDEN_SORCERESS, SindragosaGauntletFrostwarden[3], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_FROSTWARDEN_WARRIOR, SindragosaGauntletFrostwarden[4], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_FROSTWARDEN_WARRIOR, SindragosaGauntletFrostwarden[5], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            break;
        case 13:
            me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[0], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[2], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[4], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[6], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_CHAMPION, SindragosaGauntletSpawn[1], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_CHAMPION, SindragosaGauntletSpawn[3], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_CHAMPION, SindragosaGauntletSpawn[5], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            me->SummonCreature(NPC_NERUBAR_CHAMPION, SindragosaGauntletSpawn[7], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            break;
        case 14:
            for (uint8 i = 0; i < SINDRAGOSA_GAUNTLET_SPAWN_MAX/2; i++)
                me->SummonCreature(NPC_NERUBAR_BROODLING, SindragosaGauntletSpawn[urand(i, i*2)], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            break;
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
            me->SummonCreature(NPC_NERUBAR_BROODLING, SindragosaGauntletSpawn[urand(0, SINDRAGOSA_GAUNTLET_SPAWN_MAX)], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
            break;
        case 12:
        case 15:
        case 21:
        default:
            break;
    }
}

bool NpcSindragosaWardAI::IsAnyPlayerOutOfRange()
{
    if (!me->GetMap())
        return true;

    Map::PlayerList const& playerList = me->GetMap()->GetPlayers();

    if (playerList.isEmpty())
        return true;

    for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
    {
        if (Player* player = itr->getSource())
        {
            if (player->isGameMaster())
            continue;

            if (player->isAlive() && me->GetDistance(player) > 125.0f)
                return true;
        }
    }

    return false;
}

void NpcSindragosaWardAI::JustSummoned(Creature* summon)
{
    DoZoneInCombat(me, 300.0f);
    summons.Summon(summon);
}

void NpcSindragosaWardAI::SummonedCreatureDies(Creature* summon, Unit*)
{
    summon->DespawnOrUnsummon(30000);
}

void NpcSindragosaWardAI::SummonedCreatureDespawn(Creature* summon)
{
    // This one should never happen, if summoned creature despawns alive, reset!
    if (summon->isAlive())
    {
        EnterEvadeMode();
        return;
    }

    summons.Despawn(summon);
}

void NpcSindragosaWardAI::UpdateAI(uint32 diff)
{
    if (!UpdateVictim() || !_isEventInProgressOrDone)
        return;

    events.Update(diff);

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_NEXT_WAVE:
                _waveNumber++;
                DoSummonWave(_waveNumber);
                sLog->outString("Wave Number: %u", _waveNumber);
                switch (_waveNumber)
                {
                    case 2:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 3000);
                        break;
                    case 3:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 3000);
                        break;
                    case 4:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 15000);
                        break;
                    case 5:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 3000);
                        break;
                    case 6:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 3000);
                        break;
                    case 7:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 5000);
                        break;
                    case 8:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 5000);
                        break;
                    case 9:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 1000);
                        break;
                    case 10:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 1000);
                        break;
                    case 11:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 5000);
                        break;
                    case 12:
                        _entrys.erase(_entrys.begin(), _entrys.end());
                        me->GetCreatureListWithEntryInGrid(_entrys, NPC_FROSTWARDEN_SORCERESS, 150.0f);
                        me->GetCreatureListWithEntryInGrid(_entrys, NPC_FROSTWARDEN_WARRIOR, 150.0f);
                        for (std::list<Creature*>::iterator it = _entrys.begin(); it != _entrys.end(); ++it)
                            if (Creature* entry = (*it))
                                if (entry->isAlive())
                                {
                                    --_waveNumber;
                                    break;
                                }
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 5000);
                        break;
                    case 13:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 500);
                        break;
                    case 14:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 3000);
                        break;
                    case 15:
                        ++_thirtySeconds;
                        _thirtySecondsSpawnsCount = 0;
                        if (_thirtySeconds < 30)
                        {
                            _thirtySecondsSpawnsCount = urand(0, 2);
                            if (_thirtySecondsSpawns < _thirtySecondsSpawnsCount)
                                _thirtySecondsSpawnsCount = _thirtySecondsSpawns;
                            _thirtySecondsSpawns -= _thirtySecondsSpawnsCount;
                        }
                        else if (_thirtySeconds == 30)
                        {
                            _thirtySecondsSpawnsCount = _thirtySecondsSpawns;
                            _thirtySecondsSpawns = 0;
                        }
                        else if (_thirtySeconds > 30)
                        {
                            events.ScheduleEvent(EVENT_NEXT_WAVE, 4000);
                            return;
                        }
                        for (uint32 i = 0; i < _thirtySecondsSpawnsCount; i++)
                        {
                            me->GetRandomNearPosition(_randomPos, frand(10.0f, 40.0f));
                            _randomPos.m_positionZ = 310.033f;
                            me->SummonCreature(NPC_NERUBAR_BROODLING, _randomPos, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        }
                        --_waveNumber;
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 1000);
                        break;
                    case 16:
                        if (GetClosestCreatureWithEntry(me, NPC_NERUBAR_CHAMPION, 80.0f, true))
                            --_waveNumber;
                        else if (GetClosestCreatureWithEntry(me, NPC_NERUBAR_WEBWEAVER, 80.0f, true))
                            --_waveNumber;
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 5000);
                        break;
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 2000);
                    case 21:
                        events.ScheduleEvent(EVENT_DONE, 1000);
                        break;
                    default:
                        break;
                }
                break;
            case EVENT_DONE:
                events.Reset();
                me->Kill(me);
                break;
            case EVENT_UPDATE_CHECK:
                if (IsAnyPlayerOutOfRange())
                {
                    EnterEvadeMode();
                    return;
                }
                events.ScheduleEvent(EVENT_UPDATE_CHECK, 5000);
                break;
            default:
                break;
        }
    }
}

void NpcSindragosaWardSpiderAI::Reset()
{
    events.Reset();
    me->SetInFront(GetClosestCreatureWithEntry(me, NPC_SINDRAGOSAS_WARD, 300.0f, true));
    Position pos;
    pos.Relocate(me);
    pos.m_positionZ += 5.0f;
    me->SetCanFly(true);
    me->SetDisableGravity(true);
    me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
    me->SetReactState(REACT_PASSIVE);
    if (stalker = me->SummonCreature(NPC_INVISIBLE_STALKER, pos, TEMPSUMMON_TIMED_DESPAWN, 60000))
        me->CastSpell(stalker, SPELL_WEBROPE);
    pos.m_positionZ = me->GetMap()->GetHeight(me->GetPhaseMask(), me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), true, 500.0f)+17.0f;
    me->GetMotionMaster()->MovePoint(POINT_NPC_DOWN, pos);
}

void NpcSindragosaWardSpiderAI::JustDied(Unit* killer)
{
    DoCast(killer, SPELL_SOULFEAST);
}

void NpcSindragosaWardSpiderAI::MovementInform(uint32 type, uint32 id)
{
    if (type != EFFECT_MOTION_TYPE && type != POINT_MOTION_TYPE)
        return;

    Position pos;
    std::list<Player*> targets;
    switch (id)
    {
        case POINT_NPC_DOWN:
            if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                me->GetMotionMaster()->MovementExpired();
            pos.Relocate(me);
            pos.m_positionZ = me->GetMap()->GetHeight(me->GetPhaseMask(), me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), true, 500.0f);
            me->GetMotionMaster()->MoveLand(POINT_NPC_GROUND, pos);
            break;
        case POINT_NPC_GROUND:
            if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == EFFECT_MOTION_TYPE)
                me->GetMotionMaster()->MovementExpired();
            stalker->DespawnOrUnsummon();
            me->SetCanFly(false);
            me->SetDisableGravity(false);
            me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
            me->SetReactState(REACT_AGGRESSIVE);
            targets = me->GetNearestPlayersList(100.0f, true);
            Trinity::Containers::RandomResizeList(targets, 1);
            if (!targets.empty())
                me->AI()->AttackStart(targets.front());
            switch (me->GetEntry())
            {
                case NPC_NERUBAR_WEBWEAVER:
                    events.ScheduleEvent(EVENT_CAST_CRYPT_SCARABS, urand(1000, 4000));
                    events.ScheduleEvent(EVENT_CAST_WEB, urand(5000, 6000));
                    break;
                case NPC_NERUBAR_CHAMPION:
                    events.ScheduleEvent(EVENT_CAST_RUSH, 1000);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void NpcSindragosaWardSpiderAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    events.Update(diff);

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_CAST_CRYPT_SCARABS:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true))
                    DoCast(target, SPELL_CRYPT_SCARABS);
                events.ScheduleEvent(EVENT_CAST_CRYPT_SCARABS, urand(1000, 4000));
                break;
            case EVENT_CAST_WEB:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true))
                    DoCast(target, SPELL_WEB);
                events.ScheduleEvent(EVENT_CAST_WEB, urand(1000, 4000));
                break;
            case EVENT_CAST_RUSH:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true))
                    DoCast(target, SPELL_RUSH);
                events.ScheduleEvent(EVENT_CAST_RUSH, urand(10000, 14000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}

void NpcSindragosaWardFrostwardenAI::EnterCombat(Unit *target)
{
    ScriptedAI::EnterCombat(target);

    switch (me->GetEntry())
    {
        case NPC_FROSTWARDEN_WARRIOR:
            events.ScheduleEvent(EVENT_CAST_FROST_NOVA, urand(5000, 15000));
            break;
        case NPC_FROSTWARDEN_SORCERESS:
            if (me->GetMap()->Is25ManRaid())
                events.ScheduleEvent(EVENT_CAST_ICE_TOMB, urand(15000, 20000));
            events.ScheduleEvent(EVENT_CAST_FROST_NOVA, urand(10000, 15000));
            break;
    }
}

void NpcSindragosaWardFrostwardenAI::Reset()
{
    ScriptedAI::Reset();
    me->SetReactState(REACT_AGGRESSIVE);
    events.Reset();
}

void NpcSindragosaWardFrostwardenAI::JustDied(Unit* killer)
{
    DoCast(killer, SPELL_SOULFEAST);
}

void NpcSindragosaWardFrostwardenAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    events.Update(diff);

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_CAST_FROST_BLADE:
                DoCast(SPELL_FROST_BLADE);
                events.ScheduleEvent(EVENT_CAST_FROST_NOVA, urand(10000, 25000));
                break;
            case EVENT_CAST_ICE_TOMB:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 80.0f, true))
                    DoCast(target, SPELL_ICE_TOMB);
                events.ScheduleEvent(EVENT_CAST_ICE_TOMB, urand(10000, 25000));
                break;
            case EVENT_CAST_FROST_NOVA:
                DoCast(SPELL_FROST_NOVA);
                events.ScheduleEvent(EVENT_CAST_FROST_NOVA, urand(10000, 25000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}

bool AtStartSindragosaGauntlet::OnTrigger(Player* player, const AreaTriggerEntry*)
{
    if (InstanceScript* instance = player->GetInstanceScript())
        if (Creature* ward = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_SINDRAGOSA_GAUNTLET)))
            ward->AI()->DoAction(ACTION_START_GAUNTLET);
    return true;
}
