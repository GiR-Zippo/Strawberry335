include(Northrend/CrusadersColiseum/CMakeLists.txt)
include(Northrend/IcecrownCitadel/CMakeLists.txt)
include(Northrend/Ulduar/HallsOfStone/CMakeLists.txt)
include (Northrend/FrozenHalls/CMakeLists.txt)

file(GLOB scripts_Northrend Northrend/*.cpp Northrend/*.h)
file(GLOB_RECURSE scripts_AzjolNerub_Ahnkahet Northrend/AzjolNerub/Ahnkahet/*.cpp Northrend/AzjolNerub/Ahnkahet/*.h)
file(GLOB_RECURSE scripts_AzjolNerub_AzjolNerub Northrend/AzjolNerub/AzjolNerub/*.cpp Northrend/AzjolNerub/AzjolNerub/*.h)
file(GLOB_RECURSE scripts_ChamberOfAspects_ObsidianSanctum Northrend/ChamberOfAspects/ObsidianSanctum/*.cpp Northrend/ChamberOfAspects/ObsidianSanctum/*.h)
file(GLOB_RECURSE scripts_ChamberOfAspects_RubySanctum Northrend/ChamberOfAspects/RubySanctum/*.cpp Northrend/ChamberOfAspects/RubySanctum/*.h)
file(GLOB_RECURSE scripts_DraktharonKeep Northrend/DraktharonKeep/*.cpp Northrend/DraktharonKeep/*.h)

file(GLOB_RECURSE scripts_Gundrak Northrend/Gundrak/*.cpp Northrend/Gundrak/*.h)
file(GLOB_RECURSE scripts_Naxxramas Northrend/Naxxramas/*.cpp Northrend/Naxxramas/*.h)
file(GLOB_RECURSE scripts_Nexus_EyeOfEternity Northrend/Nexus/EyeOfEternity/*.cpp Northrend/Nexus/EyeOfEternity/*.h)
file(GLOB_RECURSE scripts_Nexus_Nexus Northrend/Nexus/Nexus/*.cpp Northrend/Nexus/Nexus/*.h)
file(GLOB_RECURSE scripts_Nexus_Oculus Northrend/Nexus/Oculus/*.cpp Northrend/Nexus/Oculus/*.h)
file(GLOB_RECURSE scripts_ObsidianSanctum Northrend/ChamberOfAspects/ObsidianSanctum/*.cpp Northrend/ChamberOfAspects/ObsidianSanctum/*.h)
file(GLOB_RECURSE scripts_ObsidianSanctum Northrend/ChamberOfAspects/RubySanctum/*.cpp Northrend/ChamberOfAspects/RubySanctum/*.h)
file(GLOB_RECURSE scripts_StrandOfTheAncients Northrend/StrandOfTheAncients/*.cpp Northrend/StrandOfTheAncients/*.h)
file(GLOB_RECURSE scripts_Ulduar_HallsOfLightning Northrend/Ulduar/HallsOfLightning/*.cpp Northrend/Ulduar/HallsOfLightning/*.h)
file(GLOB_RECURSE scripts_Ulduar_Ulduar Northrend/Ulduar/Ulduar/*.cpp Northrend/Ulduar/Ulduar/*.h)
file(GLOB_RECURSE scripts_UtgardeKeep_UtgardeKeep Northrend/UtgardeKeep/UtgardeKeep/*.cpp Northrend/UtgardeKeep/UtgardeKeep/*.h)
file(GLOB_RECURSE scripts_UtgardeKeep_UtgardePinnacle Northrend/UtgardeKeep/UtgardePinnacle/*.cpp Northrend/UtgardeKeep/UtgardePinnacle/*.h)
file(GLOB_RECURSE scripts_VaultOfArchavon Northrend/VaultOfArchavon/*.cpp Northrend/VaultOfArchavon/*.h)
file(GLOB_RECURSE scripts_VioletHold Northrend/VioletHold/*.cpp Northrend/VioletHold/*.h)
file(GLOB_RECURSE scripts_Wintergrasp Northrend/Wintergrasp/*.cpp Northrend/Wintergrasp/*.h)
file(GLOB_RECURSE scripts_ArgentTournament Northrend/ArgentTournament/*.cpp Northrend/ArgentTournament/*.h)

source_group(Northrend\\ArgentTournament FILES ${scripts_ArgentTournament})
source_group(Northrend\\AzjolNerub\\Ahnkahet FILES ${scripts_AzjolNerub_Ahnkahet})
source_group(Northrend\\AzjolNerub\\AzjolNerub FILES ${scripts_AzjolNerub_AzjolNerub})
source_group(Northrend\\ChamberOfAspects\\ObsidianSanctum FILES ${scripts_ChamberOfAspects_ObsidianSanctum})
source_group(Northrend\\ChamberOfAspects\\RubySanctum FILES ${scripts_ChamberOfAspects_RubySanctum})
source_group(Northrend\\DraktharonKeep FILES ${scripts_DraktharonKeep})
source_group(Northrend\\Gundrak FILES ${scripts_Gundrak})
source_group(Northrend\\Naxxramas FILES ${scripts_Naxxramas})
source_group(Northrend\\Nexus\\EyeOfEternity FILES ${scripts_Nexus_EyeOfEternity})
source_group(Northrend\\Nexus\\Nexus FILES ${scripts_Nexus_Nexus})
source_group(Northrend\\Nexus\\Oculus FILES ${scripts_Nexus_Oculus})
source_group(Northrend\\ObsidianSanctum FILES ${scripts_ObsidianSanctum})
source_group(Northrend\\StrandOfTheAncients FILES ${scripts_StrandOfTheAncients})
source_group(Northrend\\Ulduar\\HallsOfLightning FILES ${scripts_Ulduar_HallsOfLightning})
source_group(Northrend\\Ulduar\\Ulduar FILES ${scripts_Ulduar_Ulduar})
source_group(Northrend\\UtgardeKeep\\UtgardeKeep FILES ${scripts_UtgardeKeep_UtgardeKeep})
source_group(Northrend\\UtgardeKeep\\UtgardePinnacle FILES ${scripts_UtgardeKeep_UtgardePinnacle})
source_group(Northrend\\VaultOfArchavon FILES ${scripts_VaultOfArchavon})
source_group(Northrend\\VioletHold FILES ${scripts_VioletHold})
source_group(Northrend\\Wintergrasp FILES ${scripts_Wintergrasp})
source_group(Northrend FILES ${scripts_Northrend})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_ArgentTournament}
  ${scripts_AzjolNerub_Ahnkahet}
  ${scripts_AzjolNerub_AzjolNerub}
  ${scripts_ChamberOfAspects_ObsidianSanctum}
  ${scripts_ChamberOfAspects_RubySanctum}
  ${scripts_DraktharonKeep}
  ${scripts_FrozenHalls_ForgeOfSouls}
  ${scripts_FrozenHalls_HallsOfReflection}
  ${scripts_FrozenHalls_PitOfSaron}
  ${scripts_Gundrak}
  ${scripts_Naxxramas}
  ${scripts_Nexus_EyeOfEternity}
  ${scripts_Nexus_Nexus}
  ${scripts_Nexus_Oculus}
  ${scripts_ObsidianSanctum}
  ${scripts_StrandOfTheAncients}
  ${scripts_Ulduar_HallsOfLightning}
  ${scripts_Ulduar_Ulduar}
  ${scripts_UtgardeKeep_UtgardeKeep}
  ${scripts_UtgardeKeep_UtgardePinnacle}
  ${scripts_VaultOfArchavon}
  ${scripts_VioletHold}
  ${scripts_Wintergrasp}
  ${scripts_Northrend}
)

message("  -> Prepared: Northrend")
