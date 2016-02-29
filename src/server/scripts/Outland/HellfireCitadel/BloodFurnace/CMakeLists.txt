
file(GLOB_RECURSE scripts_BloodFurnace_maker Outland/HellfireCitadel/BloodFurnace/TheMaker/*.cpp Outland/HellfireCitadel/BloodFurnace/TheMaker/*.h)
file(GLOB_RECURSE scripts_BloodFurnace_broggok Outland/HellfireCitadel/BloodFurnace/Broggok/*.cpp Outland/HellfireCitadel/BloodFurnace/Broggok/*.h)
file(GLOB_RECURSE scripts_BloodFurnace_kelidan Outland/HellfireCitadel/BloodFurnace/KelidanTheBreaker/*.cpp Outland/HellfireCitadel/BloodFurnace/KelidanTheBreaker/*.h)
file(GLOB scripts_BloodFurnace Outland/HellfireCitadel/BloodFurnace/*.cpp Outland/HellfireCitadel/BloodFurnace/*.h)

source_group(Outland\\HellfireCitadel\\BloodFurnace\\TheMaker FILES ${scripts_BloodFurnace_maker})
source_group(Outland\\HellfireCitadel\\BloodFurnace\\Broggok FILES ${scripts_BloodFurnace_broggok})
source_group(Outland\\HellfireCitadel\\BloodFurnace\\KelidanTheBreaker FILES ${scripts_BloodFurnace_kelidan})
source_group(Outland\\HellfireCitadel\\BloodFurnace\\Instance FILES ${scripts_BloodFurnace})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_BloodFurnace_maker}
  ${scripts_BloodFurnace_broggok}
  ${scripts_BloodFurnace_kelidan}
  ${scripts_BloodFurnace}
)