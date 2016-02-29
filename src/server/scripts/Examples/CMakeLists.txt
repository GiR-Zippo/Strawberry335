file(GLOB_RECURSE scripts_Examples Examples/*.cpp Examples/*.h)

source_group(Examples FILES ${scripts_Examples})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_Examples}
)

message("  -> Prepared: Examples")
