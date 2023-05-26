# ---- Developer mode ----

# Developer mode enables targets and code paths in the CMake scripts that are
# only relevant for the developer(s) of vfs
# Targets necessary to build the project must be provided unconditionally, so
# consumers can trivially build and package the project
if(PROJECT_IS_TOP_LEVEL)
  option(VFS_DEVELOPER_MODE "Enable developer mode" OFF)
  option(BUILD_SHARED_LIBS "Build shared libs." OFF)
endif()

# ---- Suppress C4251 on Windows ----

# Please see include/vfs/vfs.hpp for more details
set(export_header_extra_content "
/* This needs to suppress only for MSVC */
#if defined(_MSC_VER) && !defined(__ICL)
#  define VFS_SUPPRESS_C4251 _Pragma(\"warning(suppress:4251)\")
#else
#  define VFS_SUPPRESS_C4251
#endif

/* This needs to suppress only for MSVC */
#if defined(_MSC_VER) && !defined(__ICL)
#  define VFS_REEXPORT VFS_EXPORT
#else
#  define VFS_REEXPORT
#endif
")

# ---- Warning guard ----

# target_include_directories with the SYSTEM modifier will request the compiler
# to omit warnings from the provided paths, if the compiler supports that
# This is to provide a user experience similar to find_package when
# add_subdirectory or FetchContent is used to consume this project
set(warning_guard "")
if(NOT PROJECT_IS_TOP_LEVEL)
  option(
      vfs_INCLUDES_WITH_SYSTEM
      "Use SYSTEM modifier for vfs's includes, disabling warnings"
      ON
  )
  mark_as_advanced(vfs_INCLUDES_WITH_SYSTEM)
  if(vfs_INCLUDES_WITH_SYSTEM)
    set(warning_guard SYSTEM)
  endif()
endif()
