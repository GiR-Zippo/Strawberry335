
file(GLOB sources_Entities Entities/*.cpp Entities/*.h)
file(GLOB_RECURSE sources_Entities_Corpse Entities/Corpse/*.cpp Entities/Corpse/*.h)
file(GLOB_RECURSE sources_Entities_Creature Entities/Creature/*.cpp Entities/Creature/*.h)
file(GLOB_RECURSE sources_Entities_DynamicObject Entities/DynamicObject/*.cpp Entities/DynamicObject/*.h)
file(GLOB_RECURSE sources_Entities_GameObject Entities/GameObject/*.cpp Entities/GameObject/*.h)
file(GLOB_RECURSE sources_Entities_Item Entities/Item/*.cpp Entities/Item/*.h)
file(GLOB_RECURSE sources_Entities_Object Entities/Object/*.cpp Entities/Object/*.h)
file(GLOB_RECURSE sources_Entities_Pet Entities/Pet/*.cpp Entities/Pet/*.h)
file(GLOB_RECURSE sources_Entities_Player Entities/Player/*.cpp Entities/Player/*.h)
file(GLOB_RECURSE sources_Entities_Totem Entities/Totem/*.cpp Entities/Totem/*.h)
file(GLOB_RECURSE sources_Entities_Transport Entities/Transport/*.cpp Entities/Transport/*.h)
file(GLOB_RECURSE sources_Entities_Unit Entities/Unit/*.cpp Entities/Unit/*.h)
file(GLOB_RECURSE sources_Entities_Vehicle Entities/Vehicle/*.cpp Entities/Vehicle/*.h)

source_group(Entities FILES ${sources_Entities})
source_group(Entities\\Corpse FILES ${sources_Entities_Corpse})
source_group(Entities\\Creature FILES ${sources_Entities_Creature})
source_group(Entities\\DynamicObject FILES ${sources_Entities_DynamicObject})
source_group(Entities\\GameObject FILES ${sources_Entities_GameObject})
source_group(Entities\\Item FILES ${sources_Entities_Item})
source_group(Entities\\Object FILES ${sources_Entities_Object})
source_group(Entities\\Pet FILES ${sources_Entities_Pet})
source_group(Entities\\Player FILES ${sources_Entities_Player})
source_group(Entities\\Totem FILES ${sources_Entities_Totem})
source_group(Entities\\Transport FILES ${sources_Entities_Transport})
source_group(Entities\\Unit FILES ${sources_Entities_Unit})
source_group(Entities\\Vehicle FILES ${sources_Entities_Vehicle})

set(game_STAT_SRCS
  ${game_STAT_SRCS}
  ${sources_Entities}
  ${sources_Entities_Corpse}
  ${sources_Entities_Creature}
  ${sources_Entities_DynamicObject}
  ${sources_Entities_GameObject}
  ${sources_Entities_Item}
  ${sources_Entities_Object}
  ${sources_Entities_Pet}
  ${sources_Entities_Player}
  ${sources_Entities_Totem}
  ${sources_Entities_Transport}
  ${sources_Entities_Unit}
  ${sources_Entities_Vehicle}
)
 