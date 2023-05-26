include(cmake/folders.cmake)

option(VFS_BUILD_MCSS_DOCS "Build documentation using Doxygen and m.css" OFF)
if(VFS_BUILD_MCSS_DOCS)
  include(cmake/docs.cmake)
endif()

option(VFS_ENABLE_COVERAGE "Enable coverage support separate from CTest's" OFF)
if(VFS_ENABLE_COVERAGE)
  include(cmake/coverage.cmake)
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  include(cmake/open-cpp-coverage.cmake OPTIONAL)
endif()

include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

add_folders(Project)
