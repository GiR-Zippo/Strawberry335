
file(GLOB_RECURSE sources_Battlegrounds_Zones Battlegrounds/Zones/*.cpp Battlegrounds/Zones/*.h)
file(GLOB sources_Battlegrounds_Zones_AV Battlegrounds/Zones/BattlegroundAV.cpp Battlegrounds/Zones/BattlegroundAV.h)
file(GLOB sources_Battlegrounds_Zones_AB Battlegrounds/Zones/BattlegroundAB.cpp Battlegrounds/Zones/BattlegroundAB.h)
file(GLOB sources_Battlegrounds_Zones_EY Battlegrounds/Zones/BattlegroundEY.cpp Battlegrounds/Zones/BattlegroundEY.h)
file(GLOB sources_Battlegrounds_Zones_IC Battlegrounds/Zones/BattlegroundIC.cpp Battlegrounds/Zones/BattlegroundIC.h)
file(GLOB sources_Battlegrounds_Zones_SA Battlegrounds/Zones/BattlegroundSA.cpp Battlegrounds/Zones/BattlegroundSA.h)
file(GLOB sources_Battlegrounds_Zones_WS Battlegrounds/Zones/BattlegroundWS.cpp Battlegrounds/Zones/BattlegroundWS.h)
file(GLOB sources_Battlegrounds Battlegrounds/Battleground.cpp Battlegrounds/Battleground.h Battlegrounds/BattlegroundMgr.cpp Battlegrounds/BattlegroundMgr.h Battlegrounds/BattlegroundQueue.cpp Battlegrounds/BattlegroundQueue.h)
file(GLOB sources_Battlegrounds_Arena Battlegrounds/ArenaTeam.cpp Battlegrounds/ArenaTeam.h Battlegrounds/ArenaTeamMgr.cpp Battlegrounds/ArenaTeamMgr.h)


set(game_STAT_SRCS
  ${game_STAT_SRCS}
  ${sources_Battlegrounds_Zones_AV}
  ${sources_Battlegrounds_Zones_AB}
  ${sources_Battlegrounds_Zones_EY}
  ${sources_Battlegrounds_Zones_IC}
  ${sources_Battlegrounds_Zones_SA}
  ${sources_Battlegrounds_Zones_WS}
  ${sources_Battlegrounds_Arena}
  ${sources_Battlegrounds_Zones}
  ${sources_Battlegrounds}
)

source_group(Battlegrounds\\Zones\\AlteracValley FILES ${sources_Battlegrounds_Zones_AV})
source_group(Battlegrounds\\Zones\\ArathiBasin FILES ${sources_Battlegrounds_Zones_AB})
source_group(Battlegrounds\\Zones\\EyeOfTheStorm FILES ${sources_Battlegrounds_Zones_EY})
source_group(Battlegrounds\\Zones\\IsleOfConquest FILES ${sources_Battlegrounds_Zones_IC})
source_group(Battlegrounds\\Zones\\StrandOfTheAncients FILES ${sources_Battlegrounds_Zones_SA})
source_group(Battlegrounds\\Zones\\WarsongGulch FILES ${sources_Battlegrounds_Zones_WS})
source_group(Battlegrounds\\Zones\\Various FILES ${sources_Battlegrounds_Zones})
source_group(Battlegrounds\\Arena FILES ${sources_Battlegrounds_Arena})
source_group(Battlegrounds FILES ${sources_Battlegrounds})

