include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Wireshark REQUIRED)
include_directories(${Wireshark_INCLUDE_DIR})

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})

set(INSTALL_DIRECTORY
	/usr/lib/x86_64-linux-gnu/wireshark/plugins/${Wireshark_VERSION_MAJOR}.${Wireshark_VERSION_MINOR}/epan
)

add_definitions(-DPLUGIN_VERSION="${PLUGIN_VERSION_FULL}")

set(SOURCES
	plugin_handler.cpp
	plugin.c
)

set_source_files_properties(
    ${SOURCES}
    PROPERTIES
    COMPILE_FLAGS "${WERROR_COMMON_FLAGS}"
)

add_library(${PLUGIN} SHARED ${SOURCES})
target_link_libraries(${PLUGIN} tll wireshark)

install(TARGETS ${PLUGIN} LIBRARY DESTINATION ${INSTALL_DIRECTORY})

