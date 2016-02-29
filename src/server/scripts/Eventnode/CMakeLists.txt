file(GLOB_RECURSE scripts_Test Eventnode/Test/*.cpp Eventnode/Test/*.cpp)

source_group(Eventnode\\Test FILES ${scripts_Test})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_Test}
)

message("  -> Prepared: Eventnode")