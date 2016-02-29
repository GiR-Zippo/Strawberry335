
file(GLOB_RECURSE scripts_Ulduar_HallsOfStone_krystallus Northrend/Ulduar/HallsOfStone/Krystallus/*.cpp Northrend/Ulduar/HallsOfStone/Krystallus/*.h)
file(GLOB_RECURSE scripts_Ulduar_HallsOfStone_maiden Northrend/Ulduar/HallsOfStone/MaidenOfGrief/*.cpp Northrend/Ulduar/HallsOfStone/MaidenOfGrief/*.h)
file(GLOB_RECURSE scripts_Ulduar_HallsOfStone_tribunal Northrend/Ulduar/HallsOfStone/TribunalOfTheAges/*.cpp Northrend/Ulduar/HallsOfStone/TribunalOfTheAges/*.h)
file(GLOB_RECURSE scripts_Ulduar_HallsOfStone_sjonnir Northrend/Ulduar/HallsOfStone/SjonnirTheIronshaper/*.cpp Northrend/Ulduar/HallsOfStone/SjonnirTheIronshaper/*.h)
file(GLOB scripts_Ulduar_HallsOfStone Northrend/Ulduar/HallsOfStone/*.cpp Northrend/Ulduar/HallsOfStone/*.h)

source_group(Northrend\\Ulduar\\HallsOfStone\\Krystallus FILES ${scripts_Ulduar_HallsOfStone_krystallus})
source_group(Northrend\\Ulduar\\HallsOfStone\\MaidenOfGrief FILES ${scripts_Ulduar_HallsOfStone_maiden})
source_group(Northrend\\Ulduar\\HallsOfStone\\TribunalOfTheAges FILES ${scripts_Ulduar_HallsOfStone_tribunal})
source_group(Northrend\\Ulduar\\HallsOfStone\\SjonnirTheIronshaper FILES ${scripts_Ulduar_HallsOfStone_sjonnir})
source_group(Northrend\\Ulduar\\HallsOfStone\\Instance FILES ${scripts_Ulduar_HallsOfStone})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_Ulduar_HallsOfStone_krystallus}
  ${scripts_Ulduar_HallsOfStone_maiden}
  ${scripts_Ulduar_HallsOfStone_tribunal}
  ${scripts_Ulduar_HallsOfStone_sjonnir}
  ${scripts_Ulduar_HallsOfStone}
)