file(GLOB scripts_Events Events/*.cpp Events/*.h)
file(GLOB_RECURSE scripts_Events_RottenHollow Events/RottenHollow/*.cpp Events/RottenHollow/*.h)

source_group(Events\\RottenHollow FILES ${scripts_Events_RottenHollow})
source_group(Events FILES ${scripts_Events})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_Events_RottenHollow}
  ${scripts_Events}
)

message("  -> Prepared: Events")