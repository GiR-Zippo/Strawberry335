file(GLOB_RECURSE scripts_World World/*.cpp World/*.h)

source_group(World FILES ${scripts_World})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_World}
)

message("  -> Prepared: World")
