#include "Npc_PlagueWorksTrap.h"
#include "Npc_Precious.h"
#include "Npc_Stinky.h"
#include "Spell_PlagueWorks.h"
#include "AreaTrigger_PlagueWorks.h"


void LoadPlagueWorksTrash()
{
    new PlagueWorksTrap();
    new Precious();
    new NpcStinky();

    new SpellPlagueWorksDecimate();

    new AreaTriggerPlagueWorksTrap();
}