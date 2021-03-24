### This file is an adaptation derived from
###   github.com/francofusco/template-cmake-project/blob/main/CMakeLists.txt
### Original Author: Franco FUSCO
### Source: github.com/francofusco/template-cmake-project
cmake_minimum_required(VERSION 3.19.4)
project(digital_filters VERSION 1.0.0)

# Allow using colors in messages. Use with parsimony ;)
include(cmake/colors.cmake)


#############
# LIBRARIES #
#############

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
  INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)


############
# BINARIES #
############

add_executable(showcase src/bin/showcase.cpp)
target_link_libraries(showcase ${PROJECT_NAME})

add_executable(filtfilt src/bin/filtfilt.cpp)
target_link_libraries(filtfilt ${PROJECT_NAME})


###########
# INSTALL #
###########

include(GNUInstallDirs)

# Install the library (actually, no artifact is copied)
install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib
  EXPORT ${PROJECT_NAME}Targets
)

# Copy all headers from the "include" folder
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}
  DESTINATION include
)

# Install the "Targets" file as well.
install(
  EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  FILE ${PROJECT_NAME}Targets.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
)

# Helpers to create the "package config" files.
include(CMakePackageConfigHelpers)

# Create the "Config.cmake" file (to allow "find_package"-ing this project).
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION "lib/cmake/example"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# Generate the version file associated to the "Config.cmake" file.
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# Install the configuration file.
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
)

# Make the project usable from the build directory as well.
export(
  EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
)


# add the possibility tu run 'make uninstall' to remove files added via 'make install'
# NOTE: this will remove files only, and not their parent directories.
# WARNING: this will work only if you do not play around the installed targets manually,
# and if you do not change the install destination.
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake
  IMMEDIATE @ONLY
)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake")


###########
# TESTING #
###########
find_package(GTest QUIET)
option(BUILD_TESTS "Enable building unit tests" ${GTEST_FOUND})

if(${BUILD_TESTS})
  if(${GTEST_FOUND})
    message(STATUS "Tests will be built using GTest.")
    enable_testing()
    add_subdirectory(test)
  else(${GTEST_FOUND})
    message(FATAL_ERROR "BUILD_TESTS is ON, but GTest could not be found. Tests will not be compiled!")
  endif(${GTEST_FOUND})
else(${BUILD_TESTS})
  message(STATUS "Tests will NOT be run. If you have GTest installed, you can enable them via -DBUILD_TESTS=ON.")
endif(${BUILD_TESTS})

#########################
# DOXYGEN DOCUMENTATION #
#########################

add_subdirectory(doc)
