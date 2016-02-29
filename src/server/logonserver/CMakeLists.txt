# Copyright (C) 2008-2010 Trinity <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB_RECURSE sources_CommandLine CommandLine/*.cpp CommandLine/*.h)
file(GLOB_RECURSE sources_RemoteAccess RemoteAccess/*.cpp RemoteAccess/*.h)
file(GLOB_RECURSE sources_TCSoap TCSoap/*.cpp TCSoap/*.h)
file(GLOB_RECURSE sources_LogonThread LogonThread/*.cpp LogonThread/*.h)
file(GLOB sources_localdir *.cpp *.h)

if( USE_COREPCH )
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

if ( USE_COREPCH AND MSVC )
  set(logonserver_SRCS
    PrecompiledHeaders/logonPCH.cpp
    PrecompiledHeaders/logonPCH.h
  )
endif()

set(logonserver_SRCS
  ${logonserver_SRCS}
  ${sources_CommandLine}
##  ${sources_RemoteAccess}
##  ${sources_TCSoap}
  ${sources_LogonThread}
  ${sources_localdir}
)

if( WIN32 )
  set(logonserver_SRCS
    ${logonserver_SRCS}
    ${sources_Debugging}
    logonserver.rc
  )
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/gsoap
  ${CMAKE_SOURCE_DIR}/dep/SFMT
  ${CMAKE_SOURCE_DIR}/dep/mersennetwister
  ${CMAKE_SOURCE_DIR}/src/server/collision
  ${CMAKE_SOURCE_DIR}/src/server/collision/Management
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography/Authentication
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/shared/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic/CountedReference
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic/LinkedReference
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Networking
  ${CMAKE_SOURCE_DIR}/src/server/shared/Networking/Headers
  ${CMAKE_SOURCE_DIR}/src/server/shared/Networking/Sockets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Networking/Strategies
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/PlayerHolder
  ${CMAKE_SOURCE_DIR}/src/server/shared/PlayerHolder/PlayerHolderDB
  ${CMAKE_SOURCE_DIR}/src/server/shared/PlayerHolder/PlayerHolderPacket
  ${CMAKE_SOURCE_DIR}/src/server/shared/Policies
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_SOURCE_DIR}/src/server/logon
  ${CMAKE_SOURCE_DIR}/src/server/logon/Accounts
  ${CMAKE_SOURCE_DIR}/src/server/logon/Achievements
  ${CMAKE_SOURCE_DIR}/src/server/logon/Addons
  ${CMAKE_SOURCE_DIR}/src/server/logon/Battleground
  ${CMAKE_SOURCE_DIR}/src/server/logon/Cache
  ${CMAKE_SOURCE_DIR}/src/server/logon/Character
  ${CMAKE_SOURCE_DIR}/src/server/logon/Chat
  ${CMAKE_SOURCE_DIR}/src/server/logon/Chat/Commands
  ${CMAKE_SOURCE_DIR}/src/server/logon/Database
  ${CMAKE_SOURCE_DIR}/src/server/logon/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/logon/Groups
  ${CMAKE_SOURCE_DIR}/src/server/logon/Handler
  ${CMAKE_SOURCE_DIR}/src/server/logon/Logon
  ${CMAKE_SOURCE_DIR}/src/server/logon/Maps
  ${CMAKE_SOURCE_DIR}/src/server/logon/Misc
  ${CMAKE_SOURCE_DIR}/src/server/logon/Objects
  ${CMAKE_SOURCE_DIR}/src/server/logon/Server
  ${CMAKE_SOURCE_DIR}/src/server/logon/Server/Client
  ${CMAKE_SOURCE_DIR}/src/server/logon/Server/Node
  ${CMAKE_SOURCE_DIR}/src/server/logon/Server/Control
  ${CMAKE_SOURCE_DIR}/src/server/logon/Server/Generics
  ${CMAKE_SOURCE_DIR}/src/server/logon/Warden
  ${CMAKE_SOURCE_DIR}/src/server/authserver/Server
  ${CMAKE_SOURCE_DIR}/src/server/authserver/Realms
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/CommandLine
##  ${CMAKE_CURRENT_SOURCE_DIR}/RemoteAccess
##  ${CMAKE_CURRENT_SOURCE_DIR}/TCSoap
  ${CMAKE_CURRENT_SOURCE_DIR}/LogonThread
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

set(logonserver_LINK_FLAGS "")

add_executable(logonserver ${logonserver_SRCS} $<TARGET_OBJECTS:shared> $<TARGET_OBJECTS:logon>)

if( NOT WIN32 )
  add_definitions(-D_TRINITY_CORE_CONFIG='"${CONF_DIR}/logonserver.conf"')
endif()

add_dependencies(logonserver revision.h)

if( UNIX )
  if ( USE_GOLD )
    set(logonserver_LINK_FLAGS "-B'/usr/local/bin' -pthread -lncurses -ldl ${logonserver_LINK_FLAGS}")
  else()
    set(logonserver_LINK_FLAGS "-pthread -lncurses ${logonserver_LINK_FLAGS}")
  endif()
endif()

set_target_properties(logonserver PROPERTIES LINK_FLAGS "${logonserver_LINK_FLAGS}")

target_link_libraries(logonserver
  g3dlib
  gsoap
  ${JEMALLOC_LIBRARY}
  ${READLINE_LIBRARY}
  ${TERMCAP_LIBRARY}
  ${ACE_LIBRARY}
  ${MYSQL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${OPENSSL_EXTRA_LIBRARIES}
  ${ZLIB_LIBRARIES}
  ${OSX_LIBS}
)

if( WIN32 )
  add_custom_command(TARGET logonserver
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/logonserver.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
  )
endif()

if( UNIX )
  install(TARGETS logonserver DESTINATION bin)
  install(FILES logonserver.conf.dist DESTINATION etc)
elseif( WIN32 )
  install(TARGETS logonserver DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES logonserver.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

# Generate precompiled header
if( USE_COREPCH )
  if(CMAKE_COMPILER_IS_GNUCXX)
    add_precompiled_header(logonserver ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders/logonPCH.h)
  elseif(MSVC)
    add_native_precompiled_header(logonserver ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders/logonPCH)
  endif()
endif()

