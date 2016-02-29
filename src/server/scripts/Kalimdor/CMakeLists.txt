file(GLOB scripts_Kalimdor Kalimdor/*.cpp Kalimdor/*.h)
file(GLOB_RECURSE scripts_Kalimdor_BlackfathomDeeps Kalimdor/BlackfathomDeeps/*.cpp Kalimdor/BlackfathomDeeps/*.h)
file(GLOB_RECURSE scripts_Kalimdor_CavernsOfTimes Kalimdor/CavernsOfTime/*.cpp Kalimdor/CavernsOfTime/*.h)
file(GLOB_RECURSE scripts_Kalimdor_Maraudon Kalimdor/Maraudon/*.cpp Kalimdor/Maraudon/*.h)
file(GLOB_RECURSE scripts_Kalimdor_OnyxiasLair Kalimdor/OnyxiasLair/*.cpp Kalimdor/OnyxiasLair/*.h)
file(GLOB_RECURSE scripts_Kalimdor_RazorfenDowns Kalimdor/RazorfenDowns/*.cpp Kalimdor/RazorfenDowns/*.h)
file(GLOB_RECURSE scripts_Kalimdor_RazorfenKraul Kalimdor/RazorfenKraul/*.cpp Kalimdor/RazorfenKraul/*.h)
file(GLOB_RECURSE scripts_Kalimdor_RuinsOfAhnQiraj Kalimdor/RuinsOfAhnQiraj/*.cpp Kalimdor/RuinsOfAhnQiraj/*.h)
file(GLOB_RECURSE scripts_Kalimdor_TempleOfAhnQiraj Kalimdor/TempleOfAhnQiraj/*.cpp Kalimdor/TempleOfAhnQiraj/*.h)
file(GLOB_RECURSE scripts_Kalimdor_WailingCaverns Kalimdor/WailingCaverns/*.cpp Kalimdor/WailingCaverns/*.h)
file(GLOB_RECURSE scripts_Kalimdor_ZulFarrak Kalimdor/ZulFarrak/*.cpp Kalimdor/ZulFarrak/*.h)

source_group(Kalimdor\\BlackfathomDeeps FILES ${scripts_Kalimdor_BlackfathomDeeps})
source_group(Kalimdor\\CavernsOfTimes FILES ${scripts_Kalimdor_CavernsOfTimes})
source_group(Kalimdor\\Maraudon FILES ${scripts_Kalimdor_Maraudon})
source_group(Kalimdor\\OnyxiasLair FILES ${scripts_Kalimdor_OnyxiasLair})
source_group(Kalimdor\\RazorfenDowns FILES ${scripts_Kalimdor_RazorfenDowns})
source_group(Kalimdor\\RazorfenKraul FILES ${scripts_Kalimdor_RazorfenKraul})
source_group(Kalimdor\\RuinsOfAhnQiraj FILES ${scripts_Kalimdor_RuinsOfAhnQiraj})
source_group(Kalimdor\\TempleOfAhnQiraj FILES ${scripts_Kalimdor_TempleOfAhnQiraj})
source_group(Kalimdor\\WailingCaverns FILES ${scripts_Kalimdor_WailingCaverns})
source_group(Kalimdor\\ZulFarrak FILES ${scripts_Kalimdor_ZulFarrak})
source_group(Kalimdor FILES ${scripts_Kalimdor})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_Kalimdor_BlackfathomDeeps}
  ${scripts_Kalimdor_CavernsOfTimes}
  ${scripts_Kalimdor_Maraudon}
  ${scripts_Kalimdor_OnyxiasLair}
  ${scripts_Kalimdor_RazorfenDowns}
  ${scripts_Kalimdor_RazorfenKraul}
  ${scripts_Kalimdor_RuinsOfAhnQiraj}
  ${scripts_Kalimdor_TempleOfAhnQiraj}
  ${scripts_Kalimdor_WailingCaverns}
  ${scripts_Kalimdor_ZulFarrak}
  ${scripts_Kalimdor}
)

message("  -> Prepared: Kalimdor")
