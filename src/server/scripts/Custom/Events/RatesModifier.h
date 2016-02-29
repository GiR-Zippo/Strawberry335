#ifndef __EXPERIENCE_MODIFIER_H_
#define __EXPERIENCE_MODIFIER_H_

#include "ScriptMgr.h"

enum ModEvents
{
    MOD_EVENT_XP,
    MAX_MOD_EVENT
};

class RatesModifier : public CreatureScript
{
public:
    RatesModifier() : CreatureScript("custom_experience_modifier") { }

    bool OnGossipHello(Player* player, Creature* creature);
    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action);
};

class RatesModifierPlayer : public PlayerScript
{
public:
    RatesModifierPlayer() : PlayerScript("custom_experience_modifier_player") {}

    void OnLogin(Player* player);
    void OnLogout(Player* player);

private:
    static uint32 EventIds[MAX_MOD_EVENT];
};

#endif