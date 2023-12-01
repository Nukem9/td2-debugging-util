cmake_minimum_required(VERSION 3.26)

project(
    eac_wrapper
    VERSION 1.0
    LANGUAGES CXX)

set(PROJECT_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/source")

#
# Set up the library outputs
#
if (VCPKG_TARGET_TRIPLET MATCHES "x64")
	add_subdirectory("${PROJECT_SOURCE_PATH}/uplay_shim")
else()
	message(WARNING "Skipping Uplay shim for non-x64 builds")
endif()

add_subdirectory("${PROJECT_SOURCE_PATH}/eac_shim")
