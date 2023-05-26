if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_INSTALL_INCLUDEDIR "./include/vfs" CACHE STRING "")
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package vfs)

install(
    DIRECTORY
    "${PROJECT_SOURCE_DIR}/include/"
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT vfs_Development
)

install(
    TARGETS vfs_vfs
    EXPORT vfsTargets
    RUNTIME #
    COMPONENT vfs_Runtime
    LIBRARY #
    COMPONENT vfs_Runtime
    NAMELINK_COMPONENT vfs_Development
    ARCHIVE #
    COMPONENT vfs_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    vfs_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(vfs_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${vfs_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT vfs_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${vfs_INSTALL_CMAKEDIR}"
    COMPONENT vfs_Development
)

install(
    EXPORT vfsTargets
    NAMESPACE vfs::
    DESTINATION "${vfs_INSTALL_CMAKEDIR}"
    COMPONENT vfs_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
