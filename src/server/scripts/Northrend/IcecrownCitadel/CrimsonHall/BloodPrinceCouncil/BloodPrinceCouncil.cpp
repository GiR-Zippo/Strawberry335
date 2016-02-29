#include "Boss_BloodPrinceCouncilController.h"
#include "Boss_BloodPrinceKeleseth.h"
#include "Boss_BloodPrinceTaldaram.h"
#include "Boss_BloodPrinceValanar.h"
#include "Npc_BallOfFlame.h"
#include "Npc_BloodQueenLanaThel.h"
#include "Npc_DarkNucleus.h"
#include "Npc_KineticBomb.h"
#include "Npc_Shockvortex.h"
#include "Spell_BloodPrinceCouncil.h"

void LoadBloodPrinceCouncil()
{
    new BossBloodCouncilController();
    new BossBloodPrinceKeleseth();
    new BossBloodPrinceTaldaram();
    new BossBloodPrinceValanar();

    new NpcBallOfFlame();
    new NpcDarkNucleus();
    new NpcKineticBomb();
    new NpcBloodQueenLanaThel();
    new NpcShockvortex();

    new SpellBloodCouncilShadowPrison();
    new SpellBloodCouncilShadowPrisonDamage();
    new SpellTaldaramBallOfInfernoFlame();
    new SpellTaldaramFlameBallVisual();
    new SpellTaldaramGlitteringSparks();
    new SpellTaldaramSummonFlameBall();
    new SpellValanarKineticBomb();
    new SpellValanarKineticBombAbsorb();
    new SpellValanarKineticBombKnockback();
}