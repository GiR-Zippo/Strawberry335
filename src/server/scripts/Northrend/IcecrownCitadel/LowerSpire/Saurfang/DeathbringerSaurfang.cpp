#include "Boss_DeathbringerSaurfang.h"
#include "Npc_HighOverlordSaurfang.h"
#include "Npc_MuradinBronzebeard.h"
#include "Npc_SaurfangEvent.h"
#include "Spell_DeathbringerSaurfang.h"
#include "Achievement_DeathbringerSaurfang.h"


void LoadDeathbringerSaurfang()
{
    new DeathbringerSaurfang();

    new HighOverlordSaurfang();
    new MuradinBronzebeard();
    new SaurfangEvent();

    new SpellDeathbringerBloodLink();
    new SpellDeathbringerBloodLinkAura();
    new SpellDeathbringerBloodNova();
    new SpellDeathbringerBloodNovaTargeting();
    new SpellDeathbringerBloodPower();
    new SpellDeathbringerCentOfBlood();
    new SpellDeathbringerRemoveMarks();
    new SpellDeathbringerRuneOfBlood();

    new AchievementIveGoneAndMadeAMess();
}