# Package overloads - Linux
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  if (WITH_TCMALLOC)
    set(JEMALLOC_LIBRARY "tcmalloc_minimal")
  elseif (NOT WITH_TCMALLOC)
    set(JEMALLOC_LIBRARY "jemalloc")
  endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  if(EXISTS "/usr/bin/ld.gold")
    set( USE_GOLD true )
    message("!!!!!! Found 'GOLD LINKER' and using it !!!!!!")
    message("!!!!!!   Please create a symbolic link  !!!!!!")
    message("!!!!!!      from /usr/local/bin/ld      !!!!!!")
    message("!!!!!!      to   /usr/bin/ld.gold       !!!!!!")
  endif()
endif()

# Package overloads - Linux
#(CMAKE_SYSTEM_NAME MATCHES "Linux")
#  if (NOT NOJEM)
#    set(JEMALLOC_LIBRARY "jemalloc")
#  endif()
#endif()

# set default configuration directory
if( NOT CONF_DIR )
  set(CONF_DIR ${CMAKE_INSTALL_PREFIX}/etc)
  message(STATUS "UNIX: Using default configuration directory")
endif()

# set default library directory
if( NOT LIBSDIR )
  set(LIBSDIR ${CMAKE_INSTALL_PREFIX}/lib)
  message(STATUS "UNIX: Using default library directory")
endif()

# configure uninstaller
configure_file(
  "${CMAKE_SOURCE_DIR}/cmake/platform/cmake_uninstall.in.cmake"
  "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
  @ONLY
)
message(STATUS "UNIX: Configuring uninstall target")

# create uninstaller target (allows for using "make uninstall")
add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
)
message(STATUS "UNIX: Created uninstall target")

if(CMAKE_C_COMPILER MATCHES "gcc")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/gcc/settings.cmake)
elseif(CMAKE_C_COMPILER MATCHES "icc")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/icc/settings.cmake)
elseif(CMAKE_C_COMPILER MATCHES "clang")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/clang/settings.cmake)
elseif(CMAKE_C_COMPILER MATCHES "cc")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/gcc/settings.cmake)
endif()
