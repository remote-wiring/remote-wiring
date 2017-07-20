cmake_minimum_required(VERSION 2.8.2)
cmake_policy(SET CMP0048 NEW)
project(REMOTE_WIRING VERSION 0.1.3 LANGUAGES CXX)

# Compile settings
option(LOG "Enable debug log" OFF)
if (LOG)
  message(STATUS "Debug logging enabled...")
  add_definitions(-DLOG)
endif()

# Prerequisites
find_package(Git REQUIRED)

# Enable Crascit's "DownloadProject" CMake Module (MIT)
# https://github.com/crascit/downloadproject.git
include(DownloadProject.cmake)

# Download Serial Wiring as source
download_project(
  PROJ serial_wiring
  PREFIX serial_wiring
  GIT_REPOSITORY https://github.com/remote-wiring/serial-wiring.git
  GIT_TAG master
  TIMEOUT 10
)
add_subdirectory(${serial_wiring_SOURCE_DIR} ${serial_wiring_BINARY_DIR})

# firmata
add_library(firmata ${CMAKE_CURRENT_SOURCE_DIR}/firmata/FirmataMarshaller.cpp ${CMAKE_CURRENT_SOURCE_DIR}/firmata/FirmataParser.cpp)
add_dependencies(firmata serial_wiring)

target_include_directories(firmata
  PRIVATE $<BUILD_INTERFACE:${serial_wiring_SOURCE_DIR}/include>
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/firmata>
)

set_target_properties(firmata
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY lib
    LIBRARY_OUTPUT_DIRECTORY lib
)

# To view the list of supported compiler features, see
# (https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html)
target_compile_features(firmata
  PUBLIC
    cxx_nullptr
    cxx_override
    cxx_generalized_initializers
)

target_compile_options(firmata
  PUBLIC -Wall -Wextra -Werror
)

# remote_wiring
add_library(remote_wiring ${CMAKE_CURRENT_SOURCE_DIR}/src/FirmataDevice.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/FirmataI2c.cpp)
add_dependencies(remote_wiring firmata serial_wiring)

target_include_directories(remote_wiring
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/firmata>
  PRIVATE $<BUILD_INTERFACE:${serial_wiring_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/remote-wiring>
)

set_target_properties(remote_wiring
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY lib
    LIBRARY_OUTPUT_DIRECTORY lib
)

# To view the list of supported compiler features, see
# (https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html)
target_compile_features(remote_wiring
  PUBLIC
    cxx_nullptr
    cxx_override
    cxx_generalized_initializers
)

target_compile_options(remote_wiring
  PUBLIC -Wall -Wextra -Werror
)

# Build sample applications
add_subdirectory(samples)
