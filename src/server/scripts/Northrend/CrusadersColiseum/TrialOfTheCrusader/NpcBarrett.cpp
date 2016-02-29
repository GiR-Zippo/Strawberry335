#include "TrialCrusader.h"
#include "InstanceTrialCrusader.h"

struct MenuEntry
{
    uint32 reqEncounter;
    uint32 reqStatus;
    const char *msg;
};

static MenuEntry entry[]=
{
    { ENCOUNTER_NORTHREND_BEASTS,   NOT_STARTED,    "[DEBUG] Initiate NorthrendBeasts Encounter!"   },
    { ENCOUNTER_NORTHREND_BEASTS,   DONE,           "[DEBUG] Initiate Lord Jaraxxus Encounter!"     },
    { ENCOUNTER_LORD_JARAXXUS,      DONE,           "[DEBUG] Initiate Faction Champions Encounter!" },
    { ENCOUNTER_FACTION_CHAMPIONS,  DONE,           "[DEBUG] Initiate Twin Valkyr Encounter!"       },
    { ENCOUNTER_TWIN_VALKYR,        DONE,           "[DEBUG] Initiate Anubarak Encounter!"          },
};

#define MSG_NUM 5
#define MENU_ID 724001
#define STD_MSG (player->GetSession()->GetSessionDbcLocale() == LOCALE_deDE) ? STD_MSG_DE : STD_MSG_EN
#define STD_MSG_EN "We are ready!"
#define STD_MSG_DE "Wir sind bereit!"

static Position BarrettPosition[]=
{
    {559.172f, 90.582f, 395.273f, 5.061f},
    {563.419f, 82.023f, 395.211f, 4.691f},
    {563.419f, 62.023f, 395.211f, 1.479f},
};

enum
{
    MOVE_OUTSIDE                            = MAX_ENCOUNTERS,
    MOVE_INSIDE                             = MAX_ENCOUNTERS + 1,
    MOVE_INSIDE_ARRIVED                     = MAX_ENCOUNTERS + 2,
};

class NpcBarrettTrialCrusaderAI : public ScriptedAI
{
    public:
        NpcBarrettTrialCrusaderAI(Creature* creature) : ScriptedAI(creature),
            m_MoveAction(-1)
        {
            m_Instance = creature->GetInstanceScript();
        }

        void AttackStart(Unit* /*who*/) { }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
                case ENCOUNTER_NORTHREND_BEASTS:
                case ENCOUNTER_LORD_JARAXXUS:
                case ENCOUNTER_FACTION_CHAMPIONS:
                case ENCOUNTER_TWIN_VALKYR:
                case ENCOUNTER_ANUBARAK:
                case MOVE_INSIDE:
                {
                    m_MoveAction = id;
                    break;
                }
                case MOVE_OUTSIDE:
                {
                    if (m_Instance)
                    {
                        m_Instance->SetData(TYPE_ENCOUNTER_CURRENT, m_MoveAction);
                        if (m_MoveAction != ENCOUNTER_ANUBARAK)
                            m_Instance->SetData(GO_EAST_PORTCULLIS, CLOSE);
                        m_MoveAction = -1;
                    }

                    me->SetVisible(false);
                    break;
                }
                case MOVE_INSIDE_ARRIVED:
                {
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    m_MoveAction = -1;
                    break;
                }
            }
        }

        void DoAction(const int32 action)
        {
            if (action == 1)
            {
                if (m_Instance)
                {
                    m_Instance->SetData(GO_EAST_PORTCULLIS, OPEN);

                    if (m_Instance->GetData(TYPE_TRIAL_COUNTER) <= 0 && m_Instance->instance && m_Instance->instance->IsHeroic())
                        return;
                    me->SetVisible(true);
                    me->GetMotionMaster()->MovePoint(MOVE_INSIDE, BarrettPosition[1]);
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            switch (m_MoveAction)
            {
                case ENCOUNTER_NORTHREND_BEASTS:
                case ENCOUNTER_LORD_JARAXXUS:
                case ENCOUNTER_FACTION_CHAMPIONS:
                case ENCOUNTER_TWIN_VALKYR:
                case ENCOUNTER_ANUBARAK:
                {
                    me->GetMotionMaster()->MovePoint(MOVE_OUTSIDE, BarrettPosition[2]);
                    break;
                }
                case MOVE_INSIDE:
                {
                    me->GetMotionMaster()->MovePoint(MOVE_INSIDE_ARRIVED, BarrettPosition[0]);
                    break;
                }
            }
        }

    private:
        InstanceScript *m_Instance;
        int32 m_MoveAction;
};

class NpcBarrettTrialCrusader : public CreatureScript
{
    public:
        NpcBarrettTrialCrusader() : CreatureScript("npc_announcer_toc10") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            InstanceScript* instance = creature->GetInstanceScript();
            if (!instance)
                return true;

            if (player->isInCombat() || instance->IsEncounterInProgress())
                return true;

            if (instance->GetData(TYPE_TRIAL_COUNTER) <= 0 && instance->instance && instance->instance->IsHeroic())
            {
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                return true;
            }

            if (player->isGameMaster())
            {
                for (uint8 i = 0; i < MSG_NUM; ++i)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, entry[i].msg, GOSSIP_SENDER_MAIN, i + GOSSIP_ACTION_INFO_DEF);
            }
            else
            {
                uint32 status = instance->GetData(TYPE_ENCOUNTER_STATUS);
                uint32 encounter = instance->GetData(TYPE_ENCOUNTER_CURRENT);

                for (uint8 i = 0; i < MSG_NUM; ++i)
                {
                    if (encounter == entry[i].reqEncounter && status == entry[i].reqStatus)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, STD_MSG, GOSSIP_SENDER_MAIN, i + GOSSIP_ACTION_INFO_DEF);
                        player->SEND_GOSSIP_MENU(MENU_ID, creature->GetGUID());
                        return true;
                    }
                }

                // Instance sets 'status' from "FAIL" to "NOT_STARTED" for save operation
                if (status == NOT_STARTED)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, STD_MSG, GOSSIP_SENDER_MAIN, encounter + GOSSIP_ACTION_INFO_DEF);
            }

            player->SEND_GOSSIP_MENU(MENU_ID, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            player->CLOSE_GOSSIP_MENU();

            creature->GetMotionMaster()->MovePoint(action - GOSSIP_ACTION_INFO_DEF, BarrettPosition[1]);
            creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new NpcBarrettTrialCrusaderAI(creature);
        }
};

void AddNpcBarrettTrialCrusader()
{
    new NpcBarrettTrialCrusader();
}
