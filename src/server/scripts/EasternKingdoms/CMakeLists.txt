file(GLOB scripts_EasternKingdoms EasternKingdoms/*.cpp EasternKingdoms/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_AlteracValley EasternKingdoms/AlteracValley/*.cpp EasternKingdoms/AlteracValley/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_BlackrockDepths EasternKingdoms/BlackrockDepths/*.cpp EasternKingdoms/BlackrockDepths/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_BlackrockSpire EasternKingdoms/BlackrockSpire/*.cpp EasternKingdoms/BlackrockSpire/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_BlackwingLair EasternKingdoms/BlackwingLair/*.cpp EasternKingdoms/BlackwingLair/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_Deadmines EasternKingdoms/Deadmines/*.cpp EasternKingdoms/Deadmines/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_Gnomeregan EasternKingdoms/Gnomeregan/*.cpp EasternKingdoms/Gnomeregan/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_Karazhan EasternKingdoms/Karazhan/*.cpp EasternKingdoms/Karazhan/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_MagistersTerrace EasternKingdoms/MagistersTerrace/*.cpp EasternKingdoms/MagistersTerrace/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_MoltenCore EasternKingdoms/MoltenCore/*.cpp EasternKingdoms/MoltenCore/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_OperationGnomeregan EasternKingdoms/OperationGnomeregan/*.cpp EasternKingdoms/OperationGnomeregan/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_ScarletEnclave EasternKingdoms/ScarletEnclave/*.cpp EasternKingdoms/ScarletEnclave/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_ScarletMonastery EasternKingdoms/ScarletMonastery/*.cpp EasternKingdoms/ScarletMonastery/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_Scholomance EasternKingdoms/Scholomance/*.cpp EasternKingdoms/Scholomance/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_ShadowfangKeep EasternKingdoms/ShadowfangKeep/*.cpp EasternKingdoms/ShadowfangKeep/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_Stratholme EasternKingdoms/Stratholme/*.cpp EasternKingdoms/Stratholme/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_SunkenTemple EasternKingdoms/SunkenTemple/*.cpp EasternKingdoms/SunkenTemple/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_SunwellPlateau EasternKingdoms/SunwellPlateau/*.cpp EasternKingdoms/SunwellPlateau/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_Uldaman EasternKingdoms/Uldaman/*.cpp EasternKingdoms/Uldaman/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_ZulAman EasternKingdoms/ZulAman/*.cpp EasternKingdoms/ZulAman/*.h)
file(GLOB_RECURSE scripts_EasternKingdoms_ZulGurub EasternKingdoms/ZulGurub/*.cpp EasternKingdoms/ZulGurub/*.h)

source_group(EasternKingdoms\\AlteracValley FILES ${scripts_EasternKingdoms_AlteracValley})
source_group(EasternKingdoms\\BlackrockDepths FILES ${scripts_EasternKingdoms_BlackrockDepths})
source_group(EasternKingdoms\\BlackrockSpire FILES ${scripts_EasternKingdoms_BlackrockSpire})
source_group(EasternKingdoms\\BlackwingLair FILES ${scripts_EasternKingdoms_BlackwingLair})
source_group(EasternKingdoms\\Deadmines FILES ${scripts_EasternKingdoms_Deadmines})
source_group(EasternKingdoms\\Gnomeregan FILES ${scripts_EasternKingdoms_Gnomeregan})
source_group(EasternKingdoms\\Karazhan FILES ${scripts_EasternKingdoms_Karazhan})
source_group(EasternKingdoms\\MagistersTerrace FILES ${scripts_EasternKingdoms_MagistersTerrace})
source_group(EasternKingdoms\\MoltenCore FILES ${scripts_EasternKingdoms_MoltenCore})
source_group(EasternKingdoms\\OperationGnomeregan FILES ${scripts_EasternKingdoms_OperationGnomeregan})
source_group(EasternKingdoms\\ScarletEnclave FILES ${scripts_EasternKingdoms_ScarletEnclave})
source_group(EasternKingdoms\\ScarletMonastery FILES ${scripts_EasternKingdoms_ScarletMonastery})
source_group(EasternKingdoms\\Scholomance FILES ${scripts_EasternKingdoms_Scholomance})
source_group(EasternKingdoms\\ShadowfangKeep FILES ${scripts_EasternKingdoms_ShadowfangKeep})
source_group(EasternKingdoms\\Stratholme FILES ${scripts_EasternKingdoms_Stratholme})
source_group(EasternKingdoms\\SunkenTemple FILES ${scripts_EasternKingdoms_SunkenTemple})
source_group(EasternKingdoms\\SunwellPlateau FILES ${scripts_EasternKingdoms_SunwellPlateau})
source_group(EasternKingdoms\\Uldaman FILES ${scripts_EasternKingdoms_Uldaman})
source_group(EasternKingdoms\\ZulAman FILES ${scripts_EasternKingdoms_ZulAman})
source_group(EasternKingdoms\\ZulGurub FILES ${scripts_EasternKingdoms_ZulGurub})
source_group(EasternKingdoms FILES ${scripts_EasternKingdoms})


set(scripts_STAT_SRCS
    ${scripts_STAT_SRCS}
    ${scripts_EasternKingdoms_AlteracValley}
    ${scripts_EasternKingdoms_BlackrockDepths}
    ${scripts_EasternKingdoms_BlackrockSpire}
    ${scripts_EasternKingdoms_BlackwingLair}
    ${scripts_EasternKingdoms_Deadmines}
    ${scripts_EasternKingdoms_Gnomeregan}
    ${scripts_EasternKingdoms_Karazhan}
    ${scripts_EasternKingdoms_MagistersTerrace}
    ${scripts_EasternKingdoms_MoltenCore}
    ${scripts_EasternKingdoms_OperationGnomeregan}
    ${scripts_EasternKingdoms_ScarletEnclave}
    ${scripts_EasternKingdoms_ScarletMonastery}
    ${scripts_EasternKingdoms_Scholomance}
    ${scripts_EasternKingdoms_ShadowfangKeep}
    ${scripts_EasternKingdoms_Stratholme}
    ${scripts_EasternKingdoms_SunkenTemple}
    ${scripts_EasternKingdoms_SunwellPlateau}
    ${scripts_EasternKingdoms_Uldaman}
    ${scripts_EasternKingdoms_ZulAman}
    ${scripts_EasternKingdoms_ZulGurub}
    ${scripts_EasternKingdoms}
)

message("  -> Prepared: Eastern Kingdoms")
