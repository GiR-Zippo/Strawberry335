#include "Boss_LordMarrowgar.h"
#include "Npc_BoneSpike.h"
#include "Npc_ColdFlame.h"
#include "Spell_LordMarrowgar.h"

void LoadLordMarrowgar()
{
    new BossLordMarrowgar();

    new NpcBoneSpike();
    new NpcColdFlame();

    new SpellBoneSpikeGraveyard();
    new SpellBoneStorm();
    new SpellColdflame();
    new SpellColdflameBonestorm();
    new SpellColdflameDamage();
}