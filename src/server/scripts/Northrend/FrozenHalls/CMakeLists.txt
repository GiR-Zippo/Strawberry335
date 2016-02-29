file(GLOB_RECURSE scripts_FrozenHalls_ForgeOfSouls Northrend/FrozenHalls/ForgeOfSouls/*.cpp Northrend/FrozenHalls/ForgeOfSouls/*.h)
file(GLOB_RECURSE scripts_FrozenHalls_PitOfSaron Northrend/FrozenHalls/PitOfSaron/*.cpp Northrend/FrozenHalls/PitOfSaron/*.h)

source_group(Northrend\\FrozenHalls\\ForgeOfSouls FILES ${scripts_FrozenHalls_ForgeOfSouls})
source_group(Northrend\\FrozenHalls\\PitOfSaron FILES ${scripts_FrozenHalls_PitOfSaron})

file(GLOB scripts_FrozenHalls_HallsOfReflection_FalricMarwynEvent Northrend/FrozenHalls/HallsOfReflection/FalricMarwynEvent/*.cpp Northrend/FrozenHalls/HallsOfReflection/FalricMarwynEvent/*.h)
file(GLOB_RECURSE scripts_FrozenHalls_HallsOfReflection_FalricMarwynEvent_Falric Northrend/FrozenHalls/HallsOfReflection/FalricMarwynEvent/Falric/*.cpp Northrend/FrozenHalls/HallsOfReflection/FalricMarwynEvent/Falric/*.h)
file(GLOB_RECURSE scripts_FrozenHalls_HallsOfReflection_FalricMarwynEvent_Marwyn Northrend/FrozenHalls/HallsOfReflection/FalricMarwynEvent/Marwyn/*.cpp Northrend/FrozenHalls/HallsOfReflection/FalricMarwynEvent/Marwyn/*.h)
file(GLOB_RECURSE scripts_FrozenHalls_HallsOfReflection_Frostworn Northrend/FrozenHalls/HallsOfReflection/Frostworn/*.cpp Northrend/FrozenHalls/HallsOfReflection/Frostworn/*.h)
file(GLOB_RECURSE scripts_FrozenHalls_HallsOfReflection_TheLichKingHor Northrend/FrozenHalls/HallsOfReflection/TheLichKingHor/*.cpp Northrend/FrozenHalls/HallsOfReflection/TheLichKingHor/*.h)
file(GLOB_RECURSE scripts_FrozenHalls_HallsOfReflection_Special Northrend/FrozenHalls/HallsOfReflection/Special/*.cpp Northrend/FrozenHalls/HallsOfReflection/Special/*.h)
file(GLOB_RECURSE scripts_FrozenHalls_HallsOfReflection_Instance Northrend/FrozenHalls/HallsOfReflection/*.cpp Northrend/FrozenHalls/HallsOfReflection/*.h)

source_group(Northrend\\FrozenHalls\\HallsOfReflection\\FalricMarwynEvent\\ FILES ${scripts_FrozenHalls_HallsOfReflection_FalricMarwynEvent})
source_group(Northrend\\FrozenHalls\\HallsOfReflection\\FalricMarwynEvent\\Falric FILES ${scripts_FrozenHalls_HallsOfReflection_FalricMarwynEvent_Falric})
source_group(Northrend\\FrozenHalls\\HallsOfReflection\\FalricMarwynEvent\\Marwyn FILES ${scripts_FrozenHalls_HallsOfReflection_FalricMarwynEvent_Marwyn})
source_group(Northrend\\FrozenHalls\\HallsOfReflection\\Frostworn FILES ${scripts_FrozenHalls_HallsOfReflection_Frostworn})
source_group(Northrend\\FrozenHalls\\HallsOfReflection\\TheLichKingHor FILES ${scripts_FrozenHalls_HallsOfReflection_TheLichKingHor})
source_group(Northrend\\FrozenHalls\\HallsOfReflection\\Special FILES ${scripts_FrozenHalls_HallsOfReflection_Special})
source_group(Northrend\\FrozenHalls\\HallsOfReflection\\Instance FILES ${scripts_FrozenHalls_HallsOfReflection_Instance})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_FrozenHalls_ForgeOfSouls}
  ${scripts_FrozenHalls_PitOfSaron}
  ${scripts_FrozenHalls_HallsOfReflection_FalricMarwynEvent}
  ${scripts_FrozenHalls_HallsOfReflection_FalricMarwynEvent_Falric}
  ${scripts_FrozenHalls_HallsOfReflection_FalricMarwynEvent_Marwyn}
  ${scripts_FrozenHalls_HallsOfReflection_Frostworn}
  ${scripts_FrozenHalls_HallsOfReflection_TheLichKingHor}
  ${scripts_FrozenHalls_HallsOfReflection_Instance}
)