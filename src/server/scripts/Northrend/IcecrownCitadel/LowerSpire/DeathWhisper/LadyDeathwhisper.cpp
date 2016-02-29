#include "Boss_LadyDeathwhisper.h"
#include "Npc_CultAdherent.h"
#include "Npc_CultFanatic.h"
#include "Npc_VengefulShade.h"
#include "Npc_Darnavan.h"
#include "Spell_LadyDeathwhisper.h"

void LoadLadyDeathwhisper()
{
    new LadyDeathwhisper();

    new CultAdherent();
    new CultFanatic();
    new VengefulShade();
    new Darnavan();

    new SpellDeathwhisperManaBarrier();
    new SpellCultistDarkMartyrdom();
}