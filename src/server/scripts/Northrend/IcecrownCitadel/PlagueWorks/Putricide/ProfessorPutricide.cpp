#include "Boss_ProfessorPutricide.h"
#include "Npc_GasCloud.h"
#include "Npc_VolatileOoze.h"
#include "Spell_ProfessorPutricide.h"

void LoadProfessorPutricide()
{
    new ProfessorPutricide();

    new GasCloud();
    new VolatileOoze();

    new SpellPutricideGaseousBloat();
    new SpellPutricideOozeChannel();
    new SpellPutricideSlimePuddle();
    new SpellPutricideSlimePuddleAura();
    new SpellPutricideOozeEruptionSearcher();
    new SpellPutricideChokingGasBomb();
    new SpellPutricideUnboundPlague();
    new SpellPutricideEatOoze();
    new SpellPutricideMutatedPlague();
    new SpellPutricideMutationInit();
    new SpellPutricideMutatedTransformationDismiss();
    new SpellPutricideMutatedTransformation();
    new SpellPutricideMutatedTransformationDmg();
    new SpellPutricideRegurgitatedOoze();
    new SpellPutricideClearAuraEffectValue();
    new SpellPutricideUnstableExperiment();
}
