cmake_minimum_required(VERSION 2.8.2)
project(REMOTE_WIRING)

# Prerequisites
find_package(Git REQUIRED)

# Set the output folders
set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Werror")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

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

include_directories(${serial_wiring_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/firmata)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(samples)
