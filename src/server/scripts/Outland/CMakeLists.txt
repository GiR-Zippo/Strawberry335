include(Outland/HellfireCitadel/BloodFurnace/CMakeLists.txt)

file(GLOB scripts_Outland Outland/*.cpp Outland/*.h)
file(GLOB_RECURSE scripts_Auchindoun_AuchenaiCrypts Outland/Auchindoun/AuchenaiCrypts/*.cpp Outland/Auchindoun/AuchenaiCrypts/*.h)
file(GLOB_RECURSE scripts_Auchindoun_ManaTombs Outland/Auchindoun/ManaTombs/*.cpp Outland/Auchindoun/ManaTombs/*.h)
file(GLOB_RECURSE scripts_Auchindoun_SethekkHalls Outland/Auchindoun/SethekkHalls/*.cpp Outland/Auchindoun/SethekkHalls/*.h)
file(GLOB_RECURSE scripts_Auchindoun_ShadowLabyrinth Outland/Auchindoun/ShadowLabyrinth/*.cpp Outland/Auchindoun/ShadowLabyrinth/*.h)
file(GLOB_RECURSE scripts_BlackTemple Outland/BlackTemple/*.cpp Outland/BlackTemple/*.h)
file(GLOB_RECURSE scripts_CoilfangReservoir_SerpentShrine Outland/CoilfangReservoir/SerpentShrine/*.cpp Outland/CoilfangReservoir/SerpentShrine/*.h)
file(GLOB_RECURSE scripts_CoilfangReservoir_SteamVault Outland/CoilfangReservoir/SteamVault/*.cpp Outland/CoilfangReservoir/SteamVault/*.h)
file(GLOB_RECURSE scripts_CoilfangReservoir_underbog Outland/CoilfangReservoir/underbog/*.cpp Outland/CoilfangReservoir/underbog/*.h)
file(GLOB_RECURSE scripts_CoilfangReservoir_TheSlavePens Outland/CoilfangReservoir/TheSlavePens/*.cpp Outland/CoilfangReservoir/TheSlavePens/*.h)
file(GLOB_RECURSE scripts_GruulsLair Outland/GruulsLair/*.cpp Outland/GruulsLair/*.h)
file(GLOB_RECURSE scripts_HellfireCitadel_HellfireRamparts Outland/HellfireCitadel/HellfireRamparts/*.cpp Outland/HellfireCitadel/HellfireRamparts/*.h)
file(GLOB_RECURSE scripts_HellfireCitadel_MagtheridonsLair Outland/HellfireCitadel/MagtheridonsLair/*.cpp Outland/HellfireCitadel/MagtheridonsLair/*.h)
file(GLOB_RECURSE scripts_HellfireCitadel_ShatteredHalls Outland/HellfireCitadel/ShatteredHalls/*.cpp Outland/HellfireCitadel/ShatteredHalls/*.h)
file(GLOB_RECURSE scripts_TempestKeep_arcatraz Outland/TempestKeep/arcatraz/*.cpp Outland/TempestKeep/arcatraz/*.h)
file(GLOB_RECURSE scripts_TempestKeep_botanica Outland/TempestKeep/botanica/*.cpp Outland/TempestKeep/botanica/*.h)
file(GLOB_RECURSE scripts_TempestKeep_Eye Outland/TempestKeep/Eye/*.cpp Outland/TempestKeep/Eye/*.h)
file(GLOB_RECURSE scripts_TempestKeep_Mechanar Outland/TempestKeep/Mechanar/*.cpp Outland/TempestKeep/Mechanar/*.h)

source_group(Outland\\Auchindoun\\AuchenaiCrypts FILES ${scripts_Auchindoun_AuchenaiCrypts})
source_group(Outland\\Auchindoun\\ManaTombs FILES ${scripts_Auchindoun_ManaTombs})
source_group(Outland\\Auchindoun\\SethekkHalls FILES ${scripts_Auchindoun_SethekkHalls})
source_group(Outland\\Auchindoun\\ShadowLabyrinth FILES ${scripts_Auchindoun_ShadowLabyrinth})
source_group(Outland\\BlackTemple FILES ${scripts_BlackTemple})
source_group(Outland\\CoilfangReservoir\\SerpentShrine FILES ${scripts_CoilfangReservoir_SerpentShrine})
source_group(Outland\\CoilfangReservoir\\SteamVault FILES ${scripts_CoilfangReservoir_SteamVault})
source_group(Outland\\CoilfangReservoir\\underbog FILES ${scripts_CoilfangReservoir_underbog})
source_group(Outland\\CoilfangReservoir\\TheSlavePens FILES ${scripts_CoilfangReservoir_TheSlavePens})
source_group(Outland\\GruulsLair FILES ${scripts_GruulsLair})
source_group(Outland\\HellfireCitadel\\HellfireRamparts FILES ${scripts_HellfireCitadel_HellfireRamparts})
source_group(Outland\\HellfireCitadel\\MagtheridonsLair FILES ${scripts_HellfireCitadel_MagtheridonsLair})
source_group(Outland\\HellfireCitadel\\ShatteredHalls FILES ${scripts_HellfireCitadel_ShatteredHalls})
source_group(Outland\\TempestKeep\\arcatraz FILES ${scripts_TempestKeep_arcatraz})
source_group(Outland\\TempestKeep\\botanica FILES ${scripts_TempestKeep_botanica})
source_group(Outland\\TempestKeep\\Eye FILES ${scripts_TempestKeep_Eye})
source_group(Outland\\TempestKeep\\Mechanar FILES ${scripts_TempestKeep_Mechanar})
source_group(Outland FILES ${scripts_Outland})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_Outland}
  ${scripts_Auchindoun_AuchenaiCrypts}
  ${scripts_Auchindoun_ManaTombs}
  ${scripts_Auchindoun_SethekkHalls}
  ${scripts_Auchindoun_ShadowLabyrinth}
  ${scripts_BlackTemple}
  ${scripts_CoilfangReservoir_SerpentShrine}
  ${scripts_CoilfangReservoir_SteamVault}
  ${scripts_CoilfangReservoir_underbog}
  ${scripts_CoilfangReservoir_TheSlavePens}
  ${scripts_GruulsLair}
  ${scripts_HellfireCitadel_HellfireRamparts}
  ${scripts_HellfireCitadel_MagtheridonsLair}
  ${scripts_HellfireCitadel_ShatteredHalls}
  ${scripts_TempestKeep_arcatraz}
  ${scripts_TempestKeep_botanica}
  ${scripts_TempestKeep_Eye}
  ${scripts_TempestKeep_Mechanar}
)

message("  -> Prepared: Outland")
