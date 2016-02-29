#include "Spell_Sindragosa.h"
#include "Achievement_AllYouCanEat.h"
#include "Boss_Sindragosa.h"
#include "At_SindragosasLair.h"
#include "Npc_Spinestalker.h"
#include "Npc_IceTomb.h"
#include "Npc_Rimefang.h"
#include "Npc_SindragosaTrash.h"
#include "Sindragosa.h"

void LoadSindragosa()
{
    new BossSindragosa();

    new NpcIceTomb();
    new NpcRimefang();
    new NpcSindragosaTrash();
    new NpcSpinestalker();

    new SpellFrostwardenHandlerFocusFire();
    new SpellFrostwardenHandlerOrderWhelp();
    new SpellRimefangIcyBlast();
    new SpellSindragosaCollisionFilter();
    new SpellSindragosaFrostBeacon();
    new SpellSindragosaIceTomb();
    new SpellSindragosaIcyGrip();
    new SpellSindragosaInstability();
    new SpellSindragosaMysticBuffet();
    new SpellSindragosasFury();
    new SpellSindragosaUnchainedMagic();

    new SpellTriggerSpellFromCaster("spell_sindragosa_ice_tomb", SPELL_ICE_TOMB_DUMMY, TRIGGERED_IGNORE_SET_FACING);
    new SpellTriggerSpellFromCaster("spell_sindragosa_ice_tomb_dummy", SPELL_FROST_BEACON);

    new AtSindragosaLair();

    new AchievementAllYouCanEat();
}