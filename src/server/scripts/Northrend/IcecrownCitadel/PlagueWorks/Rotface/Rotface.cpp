#include "Boss_Rotface.h"
#include "Npc_Ooze.h"
#include "Spell_Rotface.h"

void LoadRotface()
{
    new Rotface();

    new LittleOoze();
    new BigOoze();

    new SpellRotfaceOozeFlood();
    new SpellRotfaceMutatedInfection();
    new SpellRotfaceLittleOozeCombine();
    new SpellRotfaceBigOozeCombine();
    new SpellRotfaceBigOozeBuffCombine();
    new SpellRotfaceUnstableOozeExplosionInit();
    new SpellRotfaceUnstableOozeExplosion();
    new SpellRotfaceUnstableOozeExplosionSuicide();
    new SpellRotfaceOozeSummon();
    new SpellRotfaceSlimeSpray();
    new SpellRotfaceSlimeSprayTick();
    new SpellRotfaceVileGasTrigger();
}
