cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(ForwardLib 
    VERSION 0.5.0
    LANGUAGES CXX C
    DESCRIPTION "Set of tools to develope Forward Applications"
)

option(FL_ENABLE_TESTING "Enabling testing, will add google test feautures in C++" ON)
option(FL_ENABLE_EXAMPLES "Enabling benchmarking, will add google benchmark feautures in C++" ON)
option(FL_ENABLE_BENCHMARK "Enabling benchmarking, will add google benchmark feautures in C++" ON)

# option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

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

#--------------< Packages >--------------#

if(NOT TARGET OPENSSL)
    find_package(OpenSSL 1.1.1 REQUIRED)
endif()

if(NOT TARGET Boost)

    if (WIN32)
        list(APPEND CMAKE_PREFIX_PATH $ENV{BOOST_DIR})
    endif()

    if (NOT BUILD_SHARED_LIBS)
        set(Boost_USE_STATIC_LIBS ON)
    endif()

    find_package(Boost REQUIRED system json COMPONENTS)

endif()

include(cmake/cxx-common.cmake)
include(GNUInstallDirs)

set(namespace Fl)

set(FL_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/cmake/fl)
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
add_subdirectory(fldb)

add_library(${namespace}::utils ALIAS utils)
add_library(${namespace}::net ALIAS net)
add_library(${namespace}::db ALIAS db)

#--------------< API >--------------#

add_library(flapi SHARED ${FL_SOURCE})
add_library(${namespace}::flapi ALIAS flapi)

target_include_directories(flapi
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(flapi
PUBLIC
    ${namespace}::utils
    ${namespace}::net
    ${namespace}::db
)

add_library(client SHARED ${FLCLIENT_SOURCE})
add_library(${namespace}::client ALIAS client)

target_include_directories(client
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(client PUBLIC ${namespace}::flapi)


set_target_properties(flapi 
PROPERTIES
    VERSION ${ForwardLib_VERSION} 
    SOVERSION ${ForwardLib_VERSION}
)
set_target_properties(client 
PROPERTIES 
    VERSION ${ForwardLib_VERSION}
    SOVERSION ${ForwardLib_VERSION}
)

#--------------< Exporting targets >--------------#

install(FILES ${FL_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/forward)
install(FILES ${FLCLIENT_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/api)

install(TARGETS flapi client
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