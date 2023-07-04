cmake_minimum_required(VERSION 3.24)
set(VFS_VERSION 0.0.0)

include(cmake/prelude.cmake)

project(
    vfs
    VERSION ${VFS_VERSION}
    DESCRIPTION "Virtual filesystem library"
    HOMEPAGE_URL "https://alff.tech"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)

# ---- build options ----
option(VFS_ENABLE_CLANG_TIDY "Enables clang-tidy" OFF)
option(VFS_ENABLE_CPPCHECK "Enables static analysis with cppcheck" OFF)
option(VFS_ENABLE_SANITIZERS "Enables runtime sanitizers" OFF)
option(VFS_ENABLE_TESTS "Enables unit tests" ON)

if (VFS_ENABLE_CLANG_TIDY)
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy;--header-filter=${PROJECT_SOURCE_DIR}/source,${PROJECT_SOURCE_DIR}/include")
endif()

if (VFS_ENABLE_CPPCHECK)
  set(CMAKE_CXX_CPPCHECK "cppcheck;--inline-suppr")
endif()

if (VFS_ENABLE_SANITIZERS)
  set(CMAKE_CXX_FLAGS_SANITIZE "-O2 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-common")
  set(CMAKE_MAP_IMPORTED_CONFIG_SANITIZE "Sanitize;RelWithDebInfo;Release;Debug;")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wshadow -Wformat=2 -Wundef")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wshadow -Wformat=2 -Wundef")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set (CMAKE_CXX_FLAGS "/utf-8 /WX /W4 /wd4251 /permissive- /volatile:iso /Zc:preprocessor /EHsc /Zc:__cplusplus /Zc:externConstexpr /Zc:throwingNew")
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Developer mode ----

if(NOT VFS_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of vfs"
  )
endif()

include(cmake/dev-mode.cmake)


# ---- VFS library ----
add_library(
    vfs_vfs
    include/vfs/file_operation_results.hpp
    include/vfs/virtual_file_system_fwd.hpp
    include/vfs/virtual_file_system.hpp
    source/vfs/virtual_file_system.cpp
    include/vfs/provider_fwd.hpp
    include/vfs/provider.hpp
    source/vfs/provider.cpp
    include/vfs/path_fwd.hpp
    include/vfs/path.hpp
    source/vfs/path.cpp
    include/vfs/providers/local_filesystem_provider.hpp
    source/vfs/providers/local_filesystem_provider.cpp
)
add_library(vfs::vfs ALIAS vfs_vfs)

include(GenerateExportHeader)
generate_export_header(
    vfs_vfs
    BASE_NAME vfs
    EXPORT_FILE_NAME export/vfs/vfs_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE export_header_extra_content
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(vfs_vfs PUBLIC VFS_STATIC_DEFINE)
endif()

set_target_properties(
    vfs_vfs PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME vfs
    OUTPUT_NAME vfs
)

target_include_directories(
    vfs_vfs ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    vfs_vfs SYSTEM
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(vfs_vfs PUBLIC cxx_std_20)

# ---- Tests ----

if(VFS_ENABLE_TESTS)
  include(CTest)
  add_subdirectory(test)
endif()

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()
