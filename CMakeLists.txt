cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

cmake_policy(SET CMP0135 NEW)

project(ForwardLib 
    VERSION 0.5.0
    LANGUAGES CXX C
    DESCRIPTION "Set of tools for developing Forward Applications"
)

# option(BUILD_STATIC "Build library statically" OFF)

option(FL_ENABLE_TESTING "Enabling testing, will add google test feautures in C++" OFF)
option(FL_ENABLE_EXAMPLES "Enabling benchmarking, will add google benchmark feautures in C++" OFF)
option(FL_ENABLE_BENCHMARK "Enabling benchmarking, will add google benchmark feautures in C++" OFF)

if (NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
    set(CMAKE_INSTALL_INCLUDEDIR "include")
endif()

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
elseif(APPLE)
elseif(UNIX)
endif()

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

#--------------< Packages >--------------#

if (NOT TARGET benchmark)
    find_package(benchmark REQUIRED)
endif()

if(NOT TARGET OPENSSL)
    find_package(OpenSSL REQUIRED)
endif()

if(NOT TARGET Boost)
    find_package(Boost 1.81 REQUIRED system json COMPONENTS)
endif()


#--------------< Includes >--------------#

include(cmake/cxx-common.cmake)
include(GNUInstallDirs)

#--------------< Variables >--------------#

set(FL_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR}/cmake/fl)

set(FL_INCLUDE
    fl/forward/Account.hpp
    fl/forward/Channel.hpp
    fl/forward/Chat.hpp
    fl/forward/ForwardMessage.hpp
    fl/forward/Message.hpp
)
set(FL_SOURCE
    ${FL_INCLUDE}

    fl/forward/Account.cpp
    fl/forward/Channel.cpp
    fl/forward/Chat.cpp
    fl/forward/ForwardMessage.cpp
    fl/forward/Message.cpp
)

set(FLCLIENT_INCLUDE
    fl/forward/Client.hpp
)
set(FLCLIENT_SOURCE
    ${FLCLIENT_INCLUDE}

    fl/forward/Client.cpp
)

#--------------< External dependencies >--------------#

add_subdirectory(vendor)

#--------------< Building tests and examples >--------------#

if (FL_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

if (FL_ENABLE_BENCHMARK)
    add_subdirectory(bench)
endif()

if (FL_ENABLE_TESTING)
    add_subdirectory(test)
endif()

#--------------< Including sublibs >--------------#

add_subdirectory(flutils)
add_subdirectory(flnet)
add_subdirectory(flweb)
add_subdirectory(fldb)

add_library(Forward::Utils ALIAS Forward.Utils)
add_library(Forward::Net ALIAS Forward.Net)
add_library(Forward::Web ALIAS Forward.Web)
add_library(Forward::Database ALIAS Forward.Database)

#--------------< API >--------------#

add_library(Forward.API SHARED ${FL_SOURCE})
add_library(Forward::API ALIAS Forward.API)

target_include_directories(Forward.API
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(Forward.API
PUBLIC
    Forward::Utils
    Forward::Net
    Forward::Web
    Forward::Database
)

add_library(Forward.Client SHARED ${FLCLIENT_SOURCE})
add_library(Forward::Client ALIAS Forward.Client)

target_include_directories(Forward.Client
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(Forward.Client PUBLIC Forward::API)

#--------------< API versions >--------------#

set_target_properties(Forward.API 
PROPERTIES
    VERSION ${ForwardLib_VERSION} 
    SOVERSION ${ForwardLib_VERSION}
)
set_target_properties(Forward.Client 
PROPERTIES 
    VERSION ${ForwardLib_VERSION}
    SOVERSION ${ForwardLib_VERSION}
)

#--------------< Exporting targets >--------------#

install(FILES ${FL_INCLUDE} ${FLCLIENT_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/forward)

install(TARGETS Forward.API Forward.Client
    EXPORT fl-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
 
#--------------< Package configure >--------------#

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/fl-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/fl-config.cmake
    INSTALL_DESTINATION ${FL_INSTALL_LIBDIR}
    PATH_VARS
        CMAKE_INSTALL_LIBDIR
        CMAKE_INSTALL_INCLUDEDIR
)
write_basic_package_version_file(cmake/fl-config-version.cmake
    VERSION ${ForwardLib_VERSION}
    COMPATIBILITY ExactVersion
)

install(FILES 
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/fl-config.cmake 
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/fl-config-version.cmake
    DESTINATION ${FL_INSTALL_LIBDIR}
)

install(EXPORT fl-targets
    FILE fl-targets.cmake
    NAMESPACE ${namespace}::
    DESTINATION ${FL_INSTALL_LIBDIR}
)