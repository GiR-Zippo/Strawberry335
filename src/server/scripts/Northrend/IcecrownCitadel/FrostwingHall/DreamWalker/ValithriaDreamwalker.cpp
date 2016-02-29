#include "Npc_RisenArchMage.h"
#include "Archievement_PortalJockey.h"
#include "Boss_ValithriaDreamwalker.h"
#include "Npc_GluttonousAbomination.h"
#include "ValithriaDreamwalker.h"
#include "Npc_DreamPortal.h"
#include "Spell_ValithriaDreamwalker.h"
#include "Npc_DreamCloud.h"
#include "Npc_Suppresser.h"
#include "Npc_TheLichKingController.h"
#include "Npc_BlisteringZombie.h"
#include "Npc_GreenDragonCombatTrigger.h"
#include "Npc_BlazingSkeleton.h"

void LoadValithriaDreamwalker()
{
    new BossValithriaDreamwalker();

    new NpcBlazingSkeleton();
    new NpcBlisteringZombie();
    new NpcDreamCloud();
    new NpcDreamPortal();
    new NpcGluttonousAbomination();
    new NpcGreenDragonCombatTrigger();
    new NpcRisenArchmage();
    new NpcSuppresser();
    new NpcTheLichKingController();

    new SpellDreamwalkerDecayPeriodicTimer();
    new SpellDreamwalkerManaVoid();
    new SpellDreamwalkerNightmareCloud();
    new SpellDreamwalkerSummonDreamPortal();
    new SpellDreamwalkerSummoner();
    new SpellDreamwalkerSummonNightmarePortal();
    new SpellDreamwalkerSummonSuppresser();
    new SpellDreamwalkerSummonSuppresserEffect();
    new SpellDreamwalkerTwistedNightmares();

    new AchievementPortalJockey();
}