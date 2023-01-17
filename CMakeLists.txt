cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(forwardLib VERSION 0.0.1
    LANGUAGES CXX
    DESCRIPTION ""
)

option(FL_INCLUDE_BENCHMARK_COMPILE "Enabling sources, will add google bench includes" ON) 
option(FL_ENABLE_TESING "Enabling testing, will add google test feautures in C++" ON)
option(FL_ENABLE_BENCHMARK "Enabling benchmarking, will add google benchmark feautures in C++" ON)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif()
if (NOT DEFINED CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR "bin")
endif()
if (NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
    set(CMAKE_INSTALL_INCLUDEDIR "include")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(cxx-common)
include(project-common)
include(qt-common)

include(GNUInstallDirs)

set(namespace Fl)

set(FLAPI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fl)

set(FLAPI_INCLUDE
    fl/flapi.hpp
)
set(FLAPI_SOURCE
    ${FLAPI_INCLUDE}
    fl/flapi.cpp
)

set(FLCLIENT_INCLUDE
    fl/forward/Client.hpp
)
set(FLCLIENT_SOURCE
    ${FLCLIENT_INCLUDE}
    fl/forward/Client.cpp
)

if (MSVC)
    source_group("cmake-common" 
    FILES 
        "cmake/cxx-common.cmake"
        "cmake/project-common.cmake"
        "cmake/fl-install.cmake"
        "cmake/fl-config.cmake"
    )
endif()

add_subdirectory(fltypes)
add_subdirectory(flutils)
add_subdirectory(flnet)
add_subdirectory(fldb)

add_library(flapi ${FLAPI_SOURCE})
add_library(${namespace}::flapi ALIAS flapi)
target_include_directories(flapi
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(flapi
PUBLIC
    ${namespace}::types
    ${namespace}::utils
    ${namespace}::net
    ${namespace}::db
)

add_library(client ${FLCLIENT_SOURCE})
add_library(${namespace}::client ALIAS client)
target_include_directories(client
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(client
PUBLIC
    ${namespace}::flapi
)

add_library(${namespace}::types ALIAS types)
add_library(${namespace}::utils ALIAS utils)
add_library(${namespace}::net ALIAS net)
add_library(${namespace}::db ALIAS db)

add_subdirectory(vendor)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
elseif(APPLE)
elseif(UNIX)
endif()

#--------------< Exporting targets >--------------#

install(TARGETS flapi client
    EXPORT fl-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${FLAPI_INCLUDE} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/fl")
install(FILES ${FLCLIENT_INCLUDE} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/fl/forward")

install(EXPORT fl-targets
    FILE fl-targets.cmake
    NAMESPACE ${namespace}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fl
)

#--------------< Configuration File >--------------#

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/fl-config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/fl-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fl
)

#--------------< Package version >--------------#

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/fl-config-version.cmake"
    VERSION ${forwardLib_VERSION}
    COMPATIBILITY ExactVersion
)

install(
FILES
    "${CMAKE_CURRENT_BINARY_DIR}/fl-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/fl-config-version.cmake"
DESTINATION 
    "${CMAKE_INSTALL_LIBDIR}/cmake/fl"
)

export(EXPORT fl-targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/fl-targets.cmake"
    NAMESPACE ${namespace}::
)

set_target_properties(flapi PROPERTIES
    VERSION ${forwardLib_VERSION} 
    SOVERSION ${forwardLib_VERSION}
)
set_target_properties(client 
PROPERTIES 
    VERSION ${forwardLib_VERSION}
    SOVERSION ${forwardLib_VERSION}
)