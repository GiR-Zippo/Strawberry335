
file(GLOB sources_Battlefields Battlefields/*.cpp Battlefields/*.h)
file(GLOB_RECURSE sources_Battlefields_Wintergrasp Battlefields/Zones/Wintergrasp/*.cpp Battlefields/Zones/Wintergrasp/*.h)

source_group(Battlefields FILES ${sources_Battlefields})
source_group(Battlefields\\Wintergrasp FILES ${sources_Battlefields_Wintergrasp})

set(game_STAT_SRCS
  ${game_STAT_SRCS}
  ${sources_Battlefields}
  ${sources_Battlefields_Wintergrasp}
)