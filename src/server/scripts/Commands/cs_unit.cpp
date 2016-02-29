#include "ScriptMgr.h"
#include "Chat.h"
#include "CreatureAI.h"


const std::string StatIndexToName[] = { "Strength", "Agility", "Stamina", "Intellect", "Spirit" };
const float ResetAndEvadeRange = 75.0f;

class SetVisibleEvent : public BasicEvent
{
    public:

        explicit SetVisibleEvent(uint64 owner)
            : _owner(owner)
        {
        }

        bool Execute(uint64 /*time*/, uint32 /*diff*/)
        {
            if (Unit* pUnit = sObjectAccessor->FindUnit(_owner))
            {
                pUnit->SetVisible(true);
                pUnit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
            }
            return true;
        }

    private:

        uint64 _owner;
};

class unit_commandscript : public CommandScript
{
    public:

    unit_commandscript() : CommandScript("unit_commandscript") { }

    ChatCommand* GetCommands() const
    {

        static ChatCommand unitCommandTable[] =
        {
            { "infight",        SEC_ADMINISTRATOR,  false, &HandleUnitInfightCommand,          "", NULL },
            { "doaction",       SEC_ADMINISTRATOR,  false, &HandleUnitDoactionCommand,         "", NULL },
            { "auras",          SEC_ADMINISTRATOR,  false, &HandleUnitAurasCommand,            "", NULL },
            { "speed",          SEC_ADMINISTRATOR,  false, &HandleUnitSpeedCommand,            "", NULL },
            { "stats",          SEC_ADMINISTRATOR,  false, &HandleUnitStatsCommand,            "", NULL },
            { "evade",          SEC_GAMEMASTER,     false, &HandleUnitEvadeCommand,            "", NULL },
            { "reset",          SEC_GAMEMASTER,     false, &HandleUnitResetCommand,            "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "unit",           SEC_MODERATOR,      false, NULL,                    "",unitCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleUnitInfightCommand(ChatHandler* handler, const char* /*args*/)
    {
        Unit* target = handler->getSelectedUnit();

        if (!target)
        {
            handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::stringstream str;
        str << "\n";

        Creature* cre = target->ToCreature();
        Unit* u;
        bool comma = false;

        if (cre)
        {
            std::list<HostileReference*> threatlist = cre->getThreatManager().getThreatList();
            if (threatlist.empty())
            {
                handler->PSendSysMessage(LANG_UNIT_OUTFIGHT);
                return true;
            }
            for (std::list<HostileReference*>::iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
            {
                if (!comma)
                    str << ",";
                else
                    comma = true;
                str <<"\n";
                u = (*itr)->getTarget();
                if (cre = u->ToCreature())
                {

                    str << "Creature - " << cre->GetEntry() << " - " << cre->GetNameForLocaleIdx(handler->GetSessionDbcLocale());
                } else {
                    str << "Player - " << u->GetName();
                }
                str << " (" << u->GetGUIDLow() << ")";
            }
            handler->PSendSysMessage(LANG_UNIT_INFIGHTLIST, str.str().c_str());
            return true;
        }

        if (target->getHostileRefManager().isEmpty())
        {
            handler->PSendSysMessage(LANG_UNIT_OUTFIGHT);
            return true;
        }

        HostileReference* ref = target->getHostileRefManager().getFirst();

        while (true)
        {

            str <<"\n";
            u = ref->getSource()->getOwner();
            if (cre = u->ToCreature())
            {
                str << "Creature - " << cre->GetEntry() << " - " << cre->GetNameForLocaleIdx(handler->GetSessionDbcLocale());
            }
            else
            {
                str << "Player - " << u->GetName();
            }
            str << " (" << u->GetGUIDLow() << ")";

            if (ref->hasNext())
            {
                str << ",";
                ref = ref->next();
            }
            else
            {
                break;
            }
        }

        handler->PSendSysMessage(LANG_UNIT_INFIGHTLIST, str.str().c_str());
        return true;
    }

    static bool isNumber(const char* teststring)
    {
        for (uint32 i = 0; i < std::strlen (teststring); i++)
            #if PLATFORM == PLATFORM_WINDOWS
        if (!isdigit (teststring[i]))
            return false;
        #else
            if (!std::isdigit (teststring[i]))
                return false;
            #endif
                return true;
    }

    static uint32 toNumber(const char* numberstring)
    {
        uint32 number = atoi(numberstring);
        return number;
    }

    static bool HandleUnitDoactionCommand(ChatHandler* handler, const char* args)
    {
        Unit* target = handler->getSelectedUnit();

        if (!target)
        {
            handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        char* actionIdStr = strtok((char*)args, " ");
        if (!actionIdStr)
        {
            return false;
        }
        if (isNumber(actionIdStr))
        {
            uint32 actionId = toNumber(actionIdStr);

            if (Creature* creature = target->ToCreature())
            {
                if (CreatureAI* ai = creature->AI())
                {
                    ai->DoAction(actionId);
                    return true;
                }
                else
                    handler->SendSysMessage("The selected creatur doesnt have a AI");
            }
            else
                handler->SendSysMessage("The selected unit is not a creature");
        }
        else
            handler->SendSysMessage("The argument should be a number");
        return false;
    }

    static bool HandleUnitAurasCommand(ChatHandler* handler, const char* args)
    {
        Unit* pTarget = handler->getSelectedUnit();

        if (!pTarget)
        {
            handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (pTarget->GetAppliedAuras().empty())
        {
            handler->SendSysMessage("The selected unit doesn't have any applied auras");
            handler->SetSentErrorMessage(true);
            return false;
        }

        for (Unit::AuraApplicationMap::const_iterator itr = pTarget->GetAppliedAuras().begin();
             itr != pTarget->GetAppliedAuras().end(); ++itr)
        {
            uint32 spellId = itr->second->GetBase()->GetId();
            uint32 stackAmount = itr->second->GetBase()->GetStackAmount();

            SpellInfo const* pSpellInfo = sSpellMgr->GetSpellInfo(spellId);
            if (!pSpellInfo)
            {
                handler->PSendSysMessage("%d - Unknown spell aura [%d]", spellId, stackAmount);
                continue;
            }

            // Get localization of name
            std::string spellName = pSpellInfo->SpellName[handler->GetSessionDbcLocale()];

            if (spellName.empty())
                spellName = pSpellInfo->SpellName[LOCALE_enUS];

            // Build format string
            std::ostringstream oStringStream;
            if (handler->GetSession())
                oStringStream << spellId << " - |cffffffff|Hspell:" << spellId << "|h[" << spellName << "][" << stackAmount  << "]";
            else
                oStringStream << spellId << " - " << spellName << " - [" << stackAmount << "]";;

            handler->SendSysMessage(oStringStream.str().c_str());
        }

        return true;
    }

    static bool HandleUnitSpeedCommand(ChatHandler* handler, const char* args)
    {
        Unit* pTarget = handler->getSelectedUnit();

        if (!pTarget)
        {
            handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage("Speed Walk: %f", pTarget->GetSpeed(MOVE_WALK));
        handler->PSendSysMessage("Speed Run: %f", pTarget->GetSpeed(MOVE_RUN));
        handler->PSendSysMessage("Speed Fly: %f", pTarget->GetSpeed(MOVE_FLIGHT));
        handler->PSendSysMessage(pTarget->CanFreeMove() ? "Unit can free move" : "Unit cannot free move");
        handler->PSendSysMessage(pTarget->CanFly() ? "Unit can fly" : "Unit cannot fly");
        return true;
    }

    static bool HandleUnitStatsCommand(ChatHandler* handler, const char* args)
    {
        Unit* pTarget = handler->getSelectedUnit();

        if (!pTarget)
        {
            handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage("Current Stat | Create Stats");
        handler->PSendSysMessage("Health: %u | %u", pTarget->GetHealth(), pTarget->GetCreateHealth());
        handler->PSendSysMessage("Power %u: %u | %u", pTarget->getPowerType(), pTarget->GetPower(pTarget->getPowerType()), pTarget->GetCreatePowers(pTarget->getPowerType()));

        for (uint8 i = 0; i < 5; ++i)
            handler->PSendSysMessage("%s: %f | %f", StatIndexToName[i].c_str(), pTarget->GetStat(Stats(i)), pTarget->GetCreateStat(Stats(i)));

        handler->PSendSysMessage("AP: %f", pTarget->GetTotalAttackPowerValue(BASE_ATTACK));
        handler->PSendSysMessage("Offhand AP: %f", pTarget->GetTotalAttackPowerValue(OFF_ATTACK));
        handler->PSendSysMessage("Ranged AP: %f", pTarget->GetTotalAttackPowerValue(RANGED_ATTACK));

        if (pTarget->isGuardian())
            handler->PSendSysMessage("Spellpower: %d", pTarget->ToGuardian()->GetBonusDamage());

        handler->PSendSysMessage("Melee Hit: %f", pTarget->m_modMeleeHitChance);
        handler->PSendSysMessage("Ranged Hit: %f", pTarget->m_modRangedHitChance);
        handler->PSendSysMessage("Spell Hit: %f", pTarget->m_modSpellHitChance);

        handler->PSendSysMessage("Melee Crit: %f", pTarget->CalculateCriticalChanceDone(BASE_ATTACK));
        handler->PSendSysMessage("Offhand Crit: %f", pTarget->CalculateCriticalChanceDone(OFF_ATTACK));
        handler->PSendSysMessage("Ranged Crit: %f", pTarget->CalculateCriticalChanceDone(RANGED_ATTACK));

        float spellCrit = 0.0f;

        if (Player* pPlayer = pTarget->ToPlayer())
            spellCrit = pPlayer->GetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + SPELL_SCHOOL_HOLY);
        else
            spellCrit = pTarget->m_baseSpellCritChance + pTarget->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL);

        handler->PSendSysMessage("Spell Crit (Holy): %f", spellCrit);

        handler->PSendSysMessage("Melee Haste: %f", pTarget->GetHaste(CTYPE_BASE));
        handler->PSendSysMessage("Ranged Haste: %f", pTarget->GetHaste(CTYPE_RANGED));
        handler->PSendSysMessage("Spell Haste: %f", pTarget->GetHaste(CTYPE_SPELL));

        if (Player* pPlayer = pTarget->ToPlayer())
        {
            handler->PSendSysMessage("[PSM] Status: %s", pPlayer->GetStatsModifier()->IsActive() ? "active" : "not active");

            if (pPlayer->GetStatsModifier()->IsActive())
            {
                handler->PSendSysMessage("[PSM] Average Itemlevel of Player: %f", pPlayer->GetStatsModifier()->GetAvgItemLevelOfPlayer());
                handler->PSendSysMessage("[PSM] Average Itemlevel of Modifier: %f", pPlayer->GetStatsModifier()->GetAvgItemLevelOfModifier());
                handler->PSendSysMessage("[PSM] Current factor: %f", pPlayer->GetStatsModifier()->GetModifier());
            }
        }

        return true;
    }

    static bool HandleUnitEvadeCommand(ChatHandler* handler, const char* args)
    {
        Creature* pTarget = handler->getSelectedCreature();

        if (Player* pPlayer = handler->GetSession()->GetPlayer())
        {
            if (!pTarget)
            {
                std::list<Creature*> creatureList;
                Trinity::AllCreaturesInRange objects(pPlayer, ResetAndEvadeRange);
                Trinity::CreatureListSearcher<Trinity::AllCreaturesInRange> searcher(pPlayer, creatureList, objects);
                pPlayer->VisitNearbyObject(ResetAndEvadeRange, searcher);

                if (!creatureList.empty())
                {
                    bool found = false;

                    for (std::list<Creature*>::const_iterator itr = creatureList.begin(); itr != creatureList.end(); ++itr)
                    {
                        if ((*itr)->GetCreatureTemplate()->rank == 3)
                        {
                            if (CreatureAI* scriptedAI = dynamic_cast<CreatureAI*>((*itr)->GetAI()))
                            {
                                (*itr)->SetVisible(false);
                                (*itr)->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                                (*itr)->m_Events.AddEvent(new SetVisibleEvent((*itr)->GetGUID()), (*itr)->m_Events.CalculateTime(30 * IN_MILLISECONDS));
                                scriptedAI->EnterEvadeMode();

                                handler->PSendSysMessage("Der Boss '%s' [%u] wurde erfolgreich in den 'EvadeMode' versetzt!", (*itr)->GetName(), (*itr)->GetEntry());

                                if (!found)
                                    found = true;
                            }
                        }
                    }

                    if (found)
                        return true;
                }
            }
            else if (pTarget)
            {
                if (CreatureAI* scriptedAI = dynamic_cast<CreatureAI*>(pTarget->GetAI()))
                {
                    pTarget->SetVisible(false);
                    pTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                    pTarget->m_Events.AddEvent(new SetVisibleEvent(pTarget->GetGUID()), pTarget->m_Events.CalculateTime(30 * IN_MILLISECONDS));
                    scriptedAI->EnterEvadeMode();

                    handler->PSendSysMessage("Die Kreatur '%s' [%u] wurde erfolgreich in den 'EvadeMode' versetzt!", pTarget->GetName(), pTarget->GetEntry());
                    return true;
                }
            }
        }

        handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        handler->SetSentErrorMessage(true);
        return false;
    }

    static bool HandleUnitResetCommand(ChatHandler* handler, const char* args)
    {
        Creature* pTarget = handler->getSelectedCreature();

        if (Player* pPlayer = handler->GetSession()->GetPlayer())
        {
            if (!pTarget)
            {
                std::list<Creature*> creatureList;
                Trinity::AllCreaturesInRange objects(pPlayer, ResetAndEvadeRange);
                Trinity::CreatureListSearcher<Trinity::AllCreaturesInRange> searcher(pPlayer, creatureList, objects);
                pPlayer->VisitNearbyObject(ResetAndEvadeRange, searcher);

                if (!creatureList.empty())
                {
                    bool found = false;

                    for (std::list<Creature*>::const_iterator itr = creatureList.begin(); itr != creatureList.end(); ++itr)
                    {
                        if ((*itr)->GetCreatureTemplate()->rank == 3)
                        {
                            if (CreatureAI* scriptedAI = dynamic_cast<CreatureAI*>((*itr)->GetAI()))
                            {
                                (*itr)->SetVisible(false);
                                (*itr)->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                                (*itr)->m_Events.AddEvent(new SetVisibleEvent((*itr)->GetGUID()), (*itr)->m_Events.CalculateTime(30 * IN_MILLISECONDS));
                                scriptedAI->Reset();

                                handler->PSendSysMessage("Der Boss '%s' [%u] wurde erfolgreich resettet!", (*itr)->GetName(), (*itr)->GetEntry());

                                if (!found)
                                    found = true;
                            }
                        }
                    }

                    if (found)
                        return true;
                }
            }
            else if (pTarget)
            {
                if (CreatureAI* scriptedAI = dynamic_cast<CreatureAI*>(pTarget->GetAI()))
                {
                    pTarget->SetVisible(false);
                    pTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                    pTarget->m_Events.AddEvent(new SetVisibleEvent(pTarget->GetGUID()), pTarget->m_Events.CalculateTime(30 * IN_MILLISECONDS));
                    scriptedAI->Reset();

                    handler->PSendSysMessage("Die Kreatur '%s' [%u] wurde erfolgreich resettet!", pTarget->GetName(), pTarget->GetEntry());
                    return true;
                }
            }
        }

        handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        handler->SetSentErrorMessage(true);
        return false;
    }
};

void AddSC_unit_commandscript()
{
    new unit_commandscript();
}