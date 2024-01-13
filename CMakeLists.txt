cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

cmake_policy(SET CMP0135 NEW)

project(Forward.Lib 
    VERSION 0.5.0
    LANGUAGES CXX C
    DESCRIPTION "Set of tools for developing Forward Applications"
)

option(FL_ENABLE_TEST "Enabling testing, will compile google test projects" ON)
option(FL_ENABLE_BENCHMARK "Enabling benchmarking, will compile google benchmark projects" ON)
option(FL_ENABLE_EXAMPLE "Enabling examples, will compile all examples" ON)

#--------------< Projects defines >--------------#

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if (NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
    set(CMAKE_INSTALL_INCLUDEDIR "include")
endif()

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

#--------------< Packages >--------------#

if(NOT TARGET gtest)
    find_package(gtest CONFIG REQUIRED)
endif()

if(NOT TARGET benchmark)
    find_package(benchmark CONFIG REQUIRED)
endif()

if(NOT TARGET Boost)
    find_package(Boost REQUIRED system json COMPONENTS)
endif()

if(NOT TARGET OpenSSL)
    find_package(OpenSSL REQUIRED)
endif()

#--------------< Variables >--------------#

set(FL_CMAKE_LIBDIR ${CMAKE_INSTALL_LIBDIR}/cmake/fl)

#--------------< Including sublibs and API>--------------#

add_subdirectory(flcore)
add_subdirectory(fldb)
add_subdirectory(flnet)
add_subdirectory(flweb)

add_subdirectory(flapi)

#--------------< Aliases >--------------#

add_library(Forward::Core ALIAS Forward.Core)
add_library(Forward::Database ALIAS Forward.Database)
add_library(Forward::Net ALIAS Forward.Net)
add_library(Forward::Web ALIAS Forward.Web)

add_library(Forward::API ALIAS Forward.API)
add_library(Forward::Client ALIAS Forward.Client)

#--------------< Package configure >--------------#

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/fl-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/fl-config.cmake
    INSTALL_DESTINATION ${FL_CMAKE_LIBDIR}
    PATH_VARS
        CMAKE_INSTALL_LIBDIR
        CMAKE_INSTALL_INCLUDEDIR
)
write_basic_package_version_file(cmake/fl-config-version.cmake
    VERSION ${ForwardLib_VERSION}
    COMPATIBILITY ExactVersion
)

#--------------< Export targets >--------------#

install(FILES 
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/fl-config.cmake 
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/fl-config-version.cmake
    DESTINATION ${FL_CMAKE_LIBDIR}
)

install(EXPORT fl-targets
    FILE fl-targets.cmake
    NAMESPACE ${namespace}::
    DESTINATION ${FL_CMAKE_LIBDIR}
)