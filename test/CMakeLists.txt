cmake_minimum_required(VERSION 3.24)

project(vfsTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(vfs REQUIRED)
  enable_testing()
endif()

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(vfs_test
  source/vfs_test.cpp
  source/path_test.cpp
)

target_link_libraries(
    vfs_test PRIVATE
    vfs::vfs
    Catch2::Catch2WithMain
)
target_compile_features(vfs_test PRIVATE cxx_std_17)

catch_discover_tests(vfs_test)

# ---- End-of-file commands ----

add_folders(Test)
