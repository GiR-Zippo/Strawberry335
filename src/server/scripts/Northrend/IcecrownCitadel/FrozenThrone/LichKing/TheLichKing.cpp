#include "Achievement_TheLichKing.h"
#include "Boss_TheLichKing.h"
#include "Npc_BrokenFrostmourne.h"
#include "Npc_RagingSpirit.h"
#include "Npc_ShamblingHorror.h"
#include "Npc_SpiritBomb.h"
#include "Npc_SpiritWarden.h"
#include "Npc_StrangulateVehicle.h"
#include "Npc_TerenasMenethil.h"
#include "Npc_TirionFoldring.h"
#include "Npc_ValkyrShadowGuard.h"
#include "Npc_WickedSpirit.h"
#include "Npc_VileSpirit.h"
#include "Spell_TheLichKing.h"

void LoadTheLichKing()
{
    new BossTheLichKing();

    new NpcBrokenFrostmourne();
    new NpcRagingSpirit();
    new NpcShamblingHorror();
    new NpcSpiritBomb();
    new NpcSpiritWarden();
    new NpcStrangulateVehicle();
    new NpcTerenasMenethil();
    new NpcTirionFordring();
    new NpcValkyrShadowguard();
    new NpcWickedSpirit();
    new NpcVileSpirit();

    new SpellTheLichKingCastBackToCaster();
    new SpellTheLichKingDefile();
    new SpellTheLichKingEjectAllPassengers();
    new SpellTheLichKingHarvestSoul();
    new SpellTheLichKingIceBurstTargetSearch();
    new SpellTheLichKingInfest();
    new SpellTheLichKingInFrostmourneRoom();
    new SpellTheLichKingJump();
    new SpellTheLichKingJumpRemoveAura();
    new SpellTheLichKingLifeSiphon();
    new SpellTheLichKingLightsFavor();
    new SpellTheLichKingNecroticPlague();
    new SpellTheLichKingNecroticPlagueJump();
    new SpellTheLichKingPlayMovie();
    new SpellTheLichKingQuake();
    new SpellTheLichKingRagingSpirit();
    new SpellTheLichKingRestoreSoul();
    new SpellTheLichKingShadowTrapPeriodic();
    new SpellTheLichKingShadowTrapVisual();
    new SpellTheLichKingSoulReaper();
    new SpellTheLichKingSoulRip();
    new SpellTheLichKingSummonIntoAir();
    new SpellTheLichKingSummonSpiritBomb();
    new SpellTheLichKingTriggerVileSpirit();
    new SpellTheLichKingValkyrTargetSearch();
    new SpellTheLichKingVileSpiritDamageTargetSearch();
    new SpellTheLichKingVileSpiritMoveTargetSearch();
    new SpellTheLichKingVileSpirits();
    new SpellTheLichKingVileSpiritsVisual();
    new SpellTheLichKingTriggerWickedSpirit();
    new SpellTheLichKingHarvestSoulTeleportHeroic();
    new SpellTriggerSpellFromCaster("spell_the_lich_king_mass_resurrection", SPELL_MASS_RESURRECTION_REAL);

    new AchievementBeenWaitingLongTime();
    new AchievementNeckDeepInVile();

    // hacky fix for spell Harvest Soul vehicle problem
    // players are not able to receive any heals
    if (VehicleSeatEntry* vehSeat = const_cast<VehicleSeatEntry*>(sVehicleSeatStore.LookupEntry(6166)))
        vehSeat->m_flags |= 0x400;
}