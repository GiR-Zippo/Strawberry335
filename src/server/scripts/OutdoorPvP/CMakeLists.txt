file(GLOB_RECURSE scripts_OutdoorPvP OutdoorPvP/*.cpp OutdoorPvP/*.h)

source_group(OutdoorPvP FILES ${scripts_OutdoorPvP})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_OutdoorPvP}
)

message("  -> Prepared: Outdoor PVP Zones")
