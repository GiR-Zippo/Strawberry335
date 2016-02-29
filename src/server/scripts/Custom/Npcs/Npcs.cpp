#include "Zerberhorse.h"
#include "Transmog.h"
#include "Challenge.h"
#include "Npc_Customize.h"

void AddSC_npc_survey();

void LoadNpcs()
{
    new NpcChallenge();
    new NpcTransmog();
    new Zerberhorse();
    new Npc_Customize();

    AddSC_npc_survey();
}
