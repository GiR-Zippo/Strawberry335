file(GLOB_RECURSE scripts_Custom_Events Custom/Events/*.cpp Custom/Events/*.h)
file(GLOB_RECURSE scripts_Custom_Events_CityBosses Custom/Events/CityBosses/*.cpp Custom/Events/CityBosses/*.h)
file(GLOB_RECURSE scripts_Custom_Misc Custom/Misc/*.cpp Custom/Misc/*.h)
file(GLOB_RECURSE scripts_Custom_Npcs Custom/Npcs/*.cpp Custom/Npcs/*.h)
file(GLOB_RECURSE scripts_Custom_Spells Custom/Spells/*.cpp Custom/Spells/*.h)
file(GLOB scripts_Custom Custom/*.cpp Custom/*.h)

source_group(Custom\\Events FILES ${scripts_Custom_Events})
source_group(Custom\\Events\\CityBosses FILES ${scripts_Custom_Events_CityBosses})
source_group(Custom\\Misc FILES ${scripts_Custom_Misc})
source_group(Custom\\Npcs FILES ${scripts_Custom_Npcs})
source_group(Custom\\Spells FILES ${scripts_Custom_Spells})
source_group(Custom FILES ${scripts_Custom})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_Custom_Events}
  ${scripts_Custom_Events_CityBosses}
  ${scripts_Custom_Misc}
  ${scripts_Custom_Npcs}
  ${scripts_Custom_Spells}
  ${scripts_Custom}
)

message("  -> Prepared: Custom")
