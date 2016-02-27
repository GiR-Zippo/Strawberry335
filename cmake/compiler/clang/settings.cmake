add_definitions(-D_BUILD_DIRECTIVE='"${CMAKE_BUILD_TYPE}"')

add_definitions(-fno-delete-null-pointer-checks)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 --stdlib=libc++")

if(PLATFORM EQUAL 32)
  # Required on 32-bit systems to enable SSE2 (standard on x64)
  add_definitions(-msse2 -mfpmath=sse)
endif()
add_definitions(-DHAVE_SSE2 -D__SSE2__)
message(STATUS "GCC: SSE2 flags forced")

if( WITH_WARNINGS )
  add_definitions(-W -Wall -Wextra -Winit-self -Wfatal-errors -Woverloaded-virtual)
  message(STATUS "Clang: All warnings enabled")
endif()

if( WITH_COREDEBUG )
  add_definitions(-g3)
  message(STATUS "Clang: Debug-flags set (-g3)")
endif()

# -Wno-narrowing needed to suppress a warning in g3d
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")