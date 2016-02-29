#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Chat.h"

enum vs
{
    PVE = 0,
    PVP = 1
};
enum spec
{
    SPEC_1 = 0,
    SPEC_2 = 1,
    SPEC_3 = 2,
    SPEC_4 = 3,
    SPEC_5 = 4,
    SPEC_6 = 5,
    SPEC_MAX
};
enum level
{
    LOW = 226,
    MID = 232,
    HIGH = 245
};

enum bags
{
    BAG = 23162
};

enum armor
{
};

enum spells
{
    PLATE       = 750,
    MAIL        = 8737,

    SCHIELD     = 9116,
    AXE_1H      = 196,
    AXE_2H      = 197,
    MACE_1H     = 198,
    MACE_2H     = 199,
    SWORD_1H    = 201,
    SWORD_2H    = 202,
    DAGGER      = 1180,
    BOW         = 264,
    CROSSBOW    = 5011,
    GUN         = 266,
    THROW       = 2567,
    STAFF       = 227,
    WAND        = 5009,
    FIST        = 15590,
    POLEARM     = 200,

    MOUNT       = 60114,
};

class trans_commandscript : public CommandScript
{
    static uint32 specs[2][MAX_CLASSES][SPEC_MAX];
    static char* specnames[2][MAX_CLASSES][SPEC_MAX];

public:
    trans_commandscript() : CommandScript("trans_commandscript")
    {
    }

    ChatCommand* GetCommands() const
    {
        static ChatCommand gearTable[] =
        {
            { "pve",            SEC_ADMINISTRATOR,      false, &HandleTransGearPveCommand,      "", NULL },
            { "0",              SEC_ADMINISTRATOR,      false, &HandleTransGearPveCommand,      "", NULL },
            { "pvp",            SEC_ADMINISTRATOR,      false, &HandleTransGearPvpCommand,      "", NULL },
            { "1",              SEC_ADMINISTRATOR,      false, &HandleTransGearPvpCommand,      "", NULL },
            { "",               SEC_ADMINISTRATOR,      false, &HandleTransGearCommand,         "", NULL },
            { NULL,             0,                      false, NULL,                            "", NULL }
        };
        static ChatCommand transTable[] =
        {
            { "gear",           SEC_ADMINISTRATOR,      true,       NULL,                       "", gearTable },
            { NULL,             0,                      false,      NULL,                       "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "trans",          SEC_ADMINISTRATOR,      true,       NULL,                       "", transTable },
            { NULL,             0,                      false,      NULL,                       "", NULL }
        };
        return commandTable;
    }

    static bool levelUp(ChatHandler* handler)
    {
        Player* target = handler->getSelectedPlayer();
        if (!target)
            return false;
        target->GiveLevel(80);
        return true;
    }

    static bool learnSpells(ChatHandler* handler)
    {
        Player* target = handler->getSelectedPlayer();
        if (!target)
            return false;
        switch (Classes(target->getClass()))
        {
            case CLASS_WARRIOR:
                if (!target->HasSpell(MAIL))
                    target->learnSpellHighRank(MAIL);
                if (!target->HasSpell(PLATE))
                    target->learnSpellHighRank(PLATE);
                if (!target->HasSpell(BOW))
                    target->learnSpellHighRank(BOW);
                if (!target->HasSpell(CROSSBOW))
                    target->learnSpellHighRank(CROSSBOW);
                if (!target->HasSpell(THROW))
                    target->learnSpellHighRank(THROW);
                if (!target->HasSpell(GUN))
                    target->learnSpellHighRank(GUN);
                if (!target->HasSpell(DAGGER))
                    target->learnSpellHighRank(DAGGER);
                if (!target->HasSpell(SWORD_1H))
                    target->learnSpellHighRank(SWORD_1H);
                if (!target->HasSpell(SWORD_2H))
                    target->learnSpellHighRank(SWORD_2H);
                if (!target->HasSpell(AXE_1H))
                    target->learnSpellHighRank(AXE_1H);
                if (!target->HasSpell(AXE_2H))
                    target->learnSpellHighRank(AXE_2H);
                if (!target->HasSpell(MACE_1H))
                    target->learnSpellHighRank(MACE_1H);
                if (!target->HasSpell(MACE_2H))
                    target->learnSpellHighRank(MACE_2H);
                if (!target->HasSpell(STAFF))
                    target->learnSpellHighRank(STAFF);
                if (!target->HasSpell(POLEARM))
                    target->learnSpellHighRank(POLEARM);
                if (!target->HasSpell(FIST))
                    target->learnSpellHighRank(FIST);
                if (!target->HasSpell(SCHIELD))
                    target->learnSpellHighRank(SCHIELD);
                break;
            case CLASS_PALADIN:
                if (!target->HasSpell(MAIL))
                    target->learnSpellHighRank(MAIL);
                if (!target->HasSpell(PLATE))
                    target->learnSpellHighRank(PLATE);
                if (!target->HasSpell(SWORD_1H))
                    target->learnSpellHighRank(SWORD_1H);
                if (!target->HasSpell(SWORD_2H))
                    target->learnSpellHighRank(SWORD_2H);
                if (!target->HasSpell(AXE_1H))
                    target->learnSpellHighRank(AXE_1H);
                if (!target->HasSpell(AXE_2H))
                    target->learnSpellHighRank(AXE_2H);
                    target->learnSpellHighRank(MACE_1H);
                if (!target->HasSpell(MACE_2H))
                    target->learnSpellHighRank(MACE_2H);
                if (!target->HasSpell(POLEARM))
                    target->learnSpellHighRank(POLEARM);
                if (!target->HasSpell(SCHIELD))
                    target->learnSpellHighRank(SCHIELD);
                break;
            case CLASS_DEATH_KNIGHT:
                if (!target->HasSpell(MAIL))
                    target->learnSpellHighRank(MAIL);
                if (!target->HasSpell(PLATE))
                    target->learnSpellHighRank(PLATE);
                if (!target->HasSpell(SWORD_1H))
                    target->learnSpellHighRank(SWORD_1H);
                if (!target->HasSpell(SWORD_2H))
                    target->learnSpellHighRank(SWORD_2H);
                if (!target->HasSpell(AXE_1H))
                    target->learnSpellHighRank(AXE_1H);
                if (!target->HasSpell(AXE_2H))
                    target->learnSpellHighRank(AXE_2H);
                if (!target->HasSpell(MACE_1H))
                    target->learnSpellHighRank(MACE_1H);
                if (!target->HasSpell(MACE_2H))
                    target->learnSpellHighRank(MACE_2H);
                if (!target->HasSpell(POLEARM))
                    target->learnSpellHighRank(POLEARM);
                break;
            case CLASS_HUNTER:
                if (!target->HasSpell(MAIL))
                    target->learnSpellHighRank(MAIL);
                if (!target->HasSpell(BOW))
                    target->learnSpellHighRank(BOW);
                if (!target->HasSpell(CROSSBOW))
                    target->learnSpellHighRank(CROSSBOW);
                if (!target->HasSpell(GUN))
                    target->learnSpellHighRank(GUN);
                if (!target->HasSpell(DAGGER))
                    target->learnSpellHighRank(DAGGER);
                if (!target->HasSpell(AXE_1H))
                    target->learnSpellHighRank(AXE_1H);
                if (!target->HasSpell(SWORD_1H))
                    target->learnSpellHighRank(SWORD_1H);
                if (!target->HasSpell(SWORD_2H))
                    target->learnSpellHighRank(SWORD_2H);
                if (!target->HasSpell(FIST))
                    target->learnSpellHighRank(FIST);
                if (!target->HasSpell(POLEARM))
                    target->learnSpellHighRank(POLEARM);
                if (!target->HasSpell(STAFF))
                    target->learnSpellHighRank(STAFF);
                if (!target->HasSpell(THROW))
                    target->learnSpellHighRank(THROW);
                break;
            case CLASS_SHAMAN:
                if (!target->HasSpell(MAIL))
                    target->learnSpellHighRank(MAIL);
                if (!target->HasSpell(DAGGER))
                    target->learnSpellHighRank(DAGGER);
                if (!target->HasSpell(AXE_1H))
                    target->learnSpellHighRank(AXE_1H);
                if (!target->HasSpell(AXE_2H))
                    target->learnSpellHighRank(AXE_2H);
                if (!target->HasSpell(MACE_1H))
                    target->learnSpellHighRank(MACE_1H);
                if (!target->HasSpell(MACE_2H))
                    target->learnSpellHighRank(MACE_2H);
                if (!target->HasSpell(FIST))
                    target->learnSpellHighRank(FIST);
                if (!target->HasSpell(STAFF))
                    target->learnSpellHighRank(STAFF);
                break;
            case CLASS_ROGUE:
                if (!target->HasSpell(DAGGER))
                    target->learnSpellHighRank(DAGGER);
                if (!target->HasSpell(AXE_1H))
                    target->learnSpellHighRank(AXE_1H);
                if (!target->HasSpell(SWORD_1H))
                    target->learnSpellHighRank(SWORD_1H);
                if (!target->HasSpell(MACE_1H))
                    target->learnSpellHighRank(MACE_1H);
                if (!target->HasSpell(FIST))
                    target->learnSpellHighRank(FIST);
                if (!target->HasSpell(CROSSBOW))
                    target->learnSpellHighRank(CROSSBOW);
                if (!target->HasSpell(BOW))
                    target->learnSpellHighRank(BOW);
                if (!target->HasSpell(THROW))
                    target->learnSpellHighRank(THROW);
                if (!target->HasSpell(GUN))
                    target->learnSpellHighRank(GUN);
                break;
            case CLASS_DRUID:
                if (!target->HasSpell(DAGGER))
                    target->learnSpellHighRank(DAGGER);
                if (!target->HasSpell(MACE_1H))
                    target->learnSpellHighRank(MACE_1H);
                if (!target->HasSpell(MACE_2H))
                    target->learnSpellHighRank(MACE_2H);
                if (!target->HasSpell(FIST))
                    target->learnSpellHighRank(FIST);
                if (!target->HasSpell(STAFF))
                    target->learnSpellHighRank(STAFF);
                if (!target->HasSpell(POLEARM))
                    target->learnSpellHighRank(POLEARM);
                break;
            case CLASS_PRIEST:
                if (!target->HasSpell(DAGGER))
                    target->learnSpellHighRank(DAGGER);
                if (!target->HasSpell(MACE_1H))
                    target->learnSpellHighRank(MACE_1H);
                if (!target->HasSpell(STAFF))
                    target->learnSpellHighRank(STAFF);
                if (!target->HasSpell(WAND))
                    target->learnSpellHighRank(WAND);
                break;
            case CLASS_MAGE:
                if (!target->HasSpell(SWORD_1H))
                    target->learnSpellHighRank(SWORD_1H);
                if (!target->HasSpell(DAGGER))
                    target->learnSpellHighRank(DAGGER);
                if (!target->HasSpell(STAFF))
                    target->learnSpellHighRank(STAFF);
                if (!target->HasSpell(WAND))
                    target->learnSpellHighRank(WAND);
                break;
            case CLASS_WARLOCK:
                if (!target->HasSpell(DAGGER))
                    target->learnSpellHighRank(DAGGER);
                if (!target->HasSpell(SWORD_1H))
                    target->learnSpellHighRank(SWORD_1H);
                if (!target->HasSpell(STAFF))
                    target->learnSpellHighRank(STAFF);
                if (!target->HasSpell(WAND))
                    target->learnSpellHighRank(WAND);
                break;
        }

        if (!target->HasSpell(MOUNT))
            target->learnSpellHighRank(MOUNT);
        target->UpdateSkillsToMaxSkillsForLevel();
        return true;
    }

    static void realAddItem(uint32 itemid, Player* target, uint32& countAdded, uint32& countEquiped, uint32& countSkipped)
    {
        ItemTemplate const* pSrcItem = sObjectMgr->GetItemTemplate(itemid);
        if (!pSrcItem)
        {
            ++countSkipped;
            return;
        }
        if (target->HasItemCount(pSrcItem->ItemId, 1, true))
        {
            ++countSkipped;
            return;
        }
        uint16 dest = 0;
        InventoryResult msg = target->CanEquipNewItem(NULL_SLOT, dest, pSrcItem->ItemId, itemid == BAG ? false : true);
        if (msg != EQUIP_ERR_OK)
        {
            target->AddItem(pSrcItem->ItemId, 1);
            ++countAdded;
            return;
        }
        Item* pDstItem = target->GetItemByPos(dest);
        if (!pDstItem)
        {
            target->EquipNewItem(dest, pSrcItem->ItemId, true);
            target->AutoUnequipOffhandIfNeed();
            ++countEquiped;
            return;
        }
        else
        {
            uint8 dstbag = pDstItem->GetBagSlot();
            uint8 dstslot = pDstItem->GetSlot();
            msg = target->CanUnequipItem(dest, false);
            if (msg != EQUIP_ERR_OK)
            {
                ++countSkipped;
                return;
            }
            ItemPosCountVec sSrc;
            uint16 eSrc = 0;
            msg = target->CanStoreItem(NULL_BAG, NULL_SLOT, sSrc, pDstItem, true);
            if (msg != EQUIP_ERR_OK)
            {
                ++countSkipped;
                return;
            }
            target->RemoveItem(dstbag, dstslot, false);
            target->EquipNewItem(dest, pSrcItem->ItemId, true);
            target->StoreItem(sSrc, pDstItem, true);
            target->AutoUnequipOffhandIfNeed();
            ++countEquiped;
        }
    }

    static bool addItems(uint8 vs, uint8 targetclass, uint8 spec, uint8 setid, ChatHandler* handler)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_TRANSGEAR_ITEMS);
        stmt->setUInt8(0, targetclass);
        stmt->setUInt8(1, spec);
        stmt->setUInt8(2, vs);
        stmt->setUInt8(3, setid);
        PreparedQueryResult res = WorldDatabase.Query(stmt);
        if (!res)
            return false;
        Field* fields = res->Fetch();
        if (!fields)
            return false;
        const char* items = fields[0].GetCString();
        if (!items)
            return false;
        std::istringstream itemstream(items);

        Player* target = handler->getSelectedPlayer();
        uint32 itemid;

        uint32 countAdded = 0;
        uint32 countEquiped = 0;
        uint32 countSkipped = 0;

        std::stringstream str;
        str << "\n";

        realAddItem(BAG, target, countAdded, countEquiped, countSkipped);
        realAddItem(BAG, target, countAdded, countEquiped, countSkipped);

        itemstream >> itemid;
        do
        {
            realAddItem(itemid, target, countAdded, countEquiped, countSkipped);
        }
        while (itemstream >> itemid);
        str << "\n";
        str << countAdded << " Items wurden ins Inventar geaddet.\n";
        str << countEquiped << " Items wurden ausgerüstet.\n";
        str << countSkipped << " Items wurden übersprungen.\n";
        handler->SendSysMessage(str.str().c_str());
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

    static bool isSpecAvailable(uint8 vs, uint8 targetclass, uint8 spec)
    {
        for (uint32 i = 0; i < SPEC_MAX; i++)
        {
            if (specs[vs][targetclass][i] == 0)
                return false;
            if (specs[vs][targetclass][i] == spec)
                return true;
        }
        return false;
    }

    static void sendErrorSpecPvp(uint8 targetclass, ChatHandler* handler)
    {
        handler->SendSysMessage("Bitte wähle einen Spec");
        handler->SendSysMessage("Beispiele:");
        std::stringstream str1;
        std::stringstream str2;
        str1 << ".trans gear pvp [ ";
        bool first = true;
        for (uint32 i = 0; i < SPEC_MAX; i++)
        {
            if (specs[PVP][targetclass][i] == 0)
                break;
            if (!first)
                str1 << " / ";
            str1 << specs[PVP][targetclass][i];
            first = false;
        }
        str1 << " ] ...\n";
        str1 << ".trans gear 1 [ ";
        first = true;
        for (uint32 i = 0; i < SPEC_MAX; i++)
        {
            if (specs[PVP][targetclass][i] == 0)
                break;
            if (!first)
                str1 << " / ";
            str1 << specs[PVP][targetclass][i];
            first = false;
            str2 << " - " << specs[PVP][targetclass][i] << " => " << specnames[PVP][targetclass][i] << "\n";
        }
        str1 << " ] ...";
        handler->SendSysMessage(str1.str().c_str());
    }

    static void sendErrorSpecPve(uint8 targetclass, ChatHandler* handler)
    {
        handler->SendSysMessage("Bitte wähle einen Spec");
        handler->SendSysMessage("Beispiele:");
        std::stringstream str1;
        std::stringstream str2;
        str1 << ".trans gear pve [ ";
        bool first = true;
        for (uint32 i = 0; i < SPEC_MAX; i++)
        {
            if (specs[PVE][targetclass][i] == 0)
                break;
            if (!first)
                str1 << " / ";
            str1 << specs[PVE][targetclass][i];
            first = false;
        }
        str1 << " ] ...\n";
        str1 << ".trans gear 0 [ ";
        first = true;
        for (uint32 i = 0; i < SPEC_MAX; i++)
        {
            if (specs[PVE][targetclass][i] == 0)
                break;
            if (!first)
                str1 << " / ";
            str1 << specs[PVE][targetclass][i];
            first = false;
            str2 << " - " << specs[PVE][targetclass][i] << " => " << specnames[PVE][targetclass][i] << "\n";
        }
        str1 << " ] ...\n" << str2.str();
        handler->SendSysMessage(str1.str().c_str());
    }

    static void sendErrorItemlevelPvp(uint32 spec, ChatHandler* handler)
    {
        handler->SendSysMessage("Bitte wähle das Item level");
        handler->SendSysMessage("Beispiele:");
        std::stringstream str;
        str << ".trans gear pvp ";
        str << spec;
        str << " [ ";
        str << (uint32)LOW;
        str << " / ";
        str << (uint32)MID;
        str << " / ";
        str << (uint32)HIGH;
        str << " ]\n";
        str << ".trans gear 1 ";
        str << spec;
        str << " [ ";
        str << (uint32)LOW;
        str << " / ";
        str << (uint32)MID;
        str << " / ";
        str << (uint32)HIGH;
        str << " ]";
        handler->SendSysMessage(str.str().c_str());
    }

    static void sendErrorItemlevelPve(uint32 spec, ChatHandler* handler)
    {
        handler->SendSysMessage("Bitte wähle das Item level");
        handler->SendSysMessage("Beispiele:");
        std::stringstream str;
        str << ".trans gear pve ";
        str << spec;
        str << " [ ";
        str << (uint32)LOW;
        str << " / ";
        str << (uint32)MID;
        str << " / ";
        str << (uint32)HIGH;
        str << " ]\n";
        str << ".trans gear 0 ";
        str << spec;
        str << " [ ";
        str << (uint32)LOW;
        str << " / ";
        str << (uint32)MID;
        str << " / ";
        str << (uint32)HIGH;
        str << " ]";
        handler->SendSysMessage(str.str().c_str());
    }

    static bool HandleTransGearCommand(ChatHandler* handler, char const* args)
    {
        if (!handler->getSelectedPlayer())
        {
            handler->SendSysMessage("Du solltest einen Spieler auswählen.");
            return true;
        }
        if (handler->getSelectedPlayer() == handler->GetSession()->GetPlayer())
        {
            handler->SendSysMessage("Du solltest einen Spieler auswählen.");
            return true;
        }
        handler->SendSysMessage("Bitte wähle den Set Typ");
        handler->SendSysMessage("Beispiele:");
        handler->SendSysMessage(".trans gear [ pve / pvp ] ...");
        handler->SendSysMessage(".trans gear [ 0 / 1 ] ...");
        return true;
    }
    static bool HandleTransGearPvpCommand(ChatHandler* handler, char const* args)
    {
        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage("Du solltest einen Spieler auswählen.");
            return true;
        }
        if (target == handler->GetSession()->GetPlayer())
        {
            handler->SendSysMessage("Du solltest einen Spieler auswählen.");
            return true;
        }
        char* specId = strtok((char*)args, " ");
        if (!specId)
        {
            sendErrorSpecPvp(target->getClass(), handler);
            return true;
        }
        if (isNumber(specId))
        {
            uint32 specid = toNumber(specId);
            if (!isSpecAvailable(PVP, target->getClass(), specid))
            {
                sendErrorSpecPvp( target->getClass(), handler);
                return true;
            }
            char* itemLevel = strtok(NULL, " ");
            if (!itemLevel)
            {
                sendErrorItemlevelPvp(specid, handler);
                return true;
            }
            if (isNumber(itemLevel))
            {
                uint32 itemlevel = toNumber(itemLevel);
                switch (itemlevel)
                {
                    case LOW:
                    case MID:
                    case HIGH:
                        if (!levelUp(handler))
                        {
                            handler->SendSysMessage("Es trat ein Fehler bei ändern des Levels auf.");
                            return true;
                        }
                        if (!learnSpells(handler))
                        {
                            handler->SendSysMessage("Es trat ein Fehler bei Lernen von Spells auf.");
                            return true;
                        }
                        if (!addItems(PVP, target->getClass(), specid, itemlevel, handler))
                        {
                            handler->SendSysMessage("Es trat ein Fehler bei adden der Items auf.");
                            return true;
                        }
                        target->SaveToDB();
                        return true;
                    default:
                        sendErrorItemlevelPvp(specid, handler);
                        return true;
                }
            }
            else
            {
                sendErrorItemlevelPvp(specid, handler);
                return true;
            }
        }
        else
        {
            handler->SendSysMessage("Die Spec ID sollte erstmal ne zahl sein.");
            return true;
        }
    }
    static bool HandleTransGearPveCommand(ChatHandler* handler, char const* args)
    {
        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage("Du solltest einen Spieler auswählen.");
            return true;
        }
        if (target == handler->GetSession()->GetPlayer())
        {
            handler->SendSysMessage("Du solltest einen Spieler auswählen.");
            return true;
        }
        char* specId = strtok((char*)args, " ");
        if (!specId)
        {
            sendErrorSpecPve(target->getClass(), handler);
            return true;
        }
        if (isNumber(specId))
        {
            uint32 specid = toNumber(specId);
            if (!isSpecAvailable(PVE, target->getClass(), specid))
            {
                sendErrorSpecPve(target->getClass(), handler);
                return true;
            }
            char* itemLevel = strtok(NULL, " ");
            if (!itemLevel)
            {
                sendErrorItemlevelPve(specid, handler);
                return true;
            }
            if (isNumber(itemLevel))
            {
                uint32 itemlevel = toNumber(itemLevel);
                switch (itemlevel)
                {
                    case LOW:
                    case MID:
                    case HIGH:
                        if (!levelUp(handler))
                        {
                            handler->SendSysMessage("Es trat ein Fehler bei ändern des Levels auf.");
                            return true;
                        }
                        if (!learnSpells(handler))
                        {
                            handler->SendSysMessage("Es trat ein Fehler bei Lernen von Spells auf.");
                            return true;
                        }
                        if (!addItems(PVE, target->getClass(), specid, itemlevel, handler))
                        {
                            handler->SendSysMessage("Es trat ein Fehler bei adden der Items auf.");
                            return true;
                        }
                        target->SaveToDB();
                        return true;
                    default:
                        sendErrorItemlevelPve(specid, handler);
                        return true;
                }
            }
            else
            {
                sendErrorItemlevelPve(specid, handler);
                return true;
            }
        }
        else
        {
            handler->SendSysMessage("Die Spec ID sollte erstmal ne zahl sein.");
            return true;
        }
    }
};

uint32 trans_commandscript::specs[2][MAX_CLASSES][SPEC_MAX]  =
{
    {   //PVE
        {               0},     //None
        {1, 2,          0},     //Warrior
        {1, 2, 3,       0},     //Paladin
        {1,             0},     //Hunter
        {1, 2,          0},     //Rouge
        {1, 2, 3,       0},     //Priest
        {1, 2, 3, 4, 5, 0},     //DK
        {1, 2, 3,       0},     //Shaman
        {1,             0},     //Mage
        {1,             0},     //Warlock
        {               0},     //Unk
        {1, 2, 3, 4,    0}      //Druid
    },
    {   //PVP
        {               0},     //None
        {3,             0},     //Warrior
        {4, 5,          0},     //Paladin
        {2,             0},     //Hunter
        {3,             0},     //Rouge
        {4, 5, 6,       0},     //Priest
        {6, 7,          0},     //DK
        {4, 5, 6,       0},     //Shaman
        {2,             0},     //Mage
        {2,             0},     //Warlock
        {               0},     //Unk
        {5, 6, 7,       0}      //Druid
    }
};

char* trans_commandscript::specnames[2][MAX_CLASSES][SPEC_MAX]  =
{
    {   //PVE
        {                                                                               },     //None
        {"Tank",        "Fury/Waffen",                                                  },     //Warrior
        {"Tank",        "Heal",         "DD",                                           },     //Paladin
        {"DD",                                                                          },     //Hunter
        {"Muti",        "Kampf",                                                        },     //Rouge
        {"DD",          "Holy",         "Diszi",                                        },     //Priest
        {"Tank/Frost",  "Tank/Blut",    "Frost",        "Unholy",       "Blut",         },     //DK
        {"Ele",         "Vs",           "Heal",                                         },     //Shaman
        {"DD",                                                                          },     //Mage
        {"DD",                                                                          },     //Warlock
        {                                                                               },     //Unk
        {"Tank",        "Heal",         "Feral",        "Moonkin",                      }      //Druid
    },
    {   //PVP
        {                                                                               },     //None
        {"Fury/Waffen",                                                                 },     //Warrior
        {"Heal",        "DD",                                                           },     //Paladin
        {"DD",                                                                          },     //Hunter
        {"DD",                                                                          },     //Rouge
        {"DD",          "Holy",         "Diszi",                                        },     //Priest
        {"Frost",       "Unholy",                                                       },     //DK
        {"Ele",         "Vs",           "Heal",                                         },     //Shaman
        {"DD",                                                                          },     //Mage
        {"DD",                                                                          },     //Warlock
        {                                                                               },     //Unk
        {"Heal",        "Feral",        "Moonkin",                                      }      //Druid
    }
};

void AddSC_trans_commandscript()
{
    new trans_commandscript();
}