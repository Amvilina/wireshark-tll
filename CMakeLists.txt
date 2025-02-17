cmake_minimum_required(VERSION 2.8)

project(wireshark-tll)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

SET(DESTDIR ${CMAKE_BINARY_DIR} CACHE STRING "Output path")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DESTDIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DESTDIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DESTDIR}/lib)

set(PLUGIN wireshark_tll_plugin)
set(PLUGIN_VERSION "0.0.0" CACHE STRING "Version of Dissector plugin")
set(PLUGIN_VERSION_FULL "0.0.0" CACHE STRING "Full Version of Dissector plugin")

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -fPIC -O2 -g -Wall")

enable_testing()

add_subdirectory(src)
