cmake_minimum_required(VERSION 3.24)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (FL_ENABLE_TESING)
    add_subdirectory(test)
endif()

if(NOT TARGET OPENSSL)
    find_package(OpenSSL 1.1.1 REQUIRED)
endif()

if(NOT TARGET Boost)

    if (NOT BUILD_SHARED_LIBS)
        set(Boost_USE_STATIC_RUNTIME ON)
    endif()

    list(APPEND CMAKE_PREFIX_PATH $ENV{BOOST})
    find_package(Boost REQUIRED system COMPONENTS)

endif()

if (WIN32)

    set(MYSQL_CONNECTOR_LIB_NAME)
    set(MYSQL_CONNECTOR_LIB_NAME_ONLY mysqlcppconn)
    set(MYSQL_CONNECTOR_LIB_TYPE)
    set(MYSQL_CONNECTOR_LIB_EXT)

    set(MYSQL_CONNECTOR_POSSIBLE_PATH
        "C:\\Program Files\\MySQL"
        "C:\\Program Files\\MySQL\\Connector C++"
    )

    if (BUILD_SHARED_LIBS)
        set(MYSQL_CONNECTOR_LIB_TYPE "")
        set(MYSQL_CONNECTOR_LIB_EXT ".dll")
    else()
        set(MYSQL_CONNECTOR_LIB_TYPE "-static")
        set(MYSQL_CONNECTOR_LIB_EXT ".lib")

        # To statically build MySQL Connector C++
        add_compile_definitions(CPPCONN_LIB_BUILD) 
    endif()

    string(APPEND MYSQL_CONNECTOR_LIB_NAME ${MYSQL_CONNECTOR_LIB_NAME_ONLY})
    string(APPEND MYSQL_CONNECTOR_LIB_NAME ${MYSQL_CONNECTOR_LIB_TYPE})
    string(APPEND MYSQL_CONNECTOR_LIB_NAME ${MYSQL_CONNECTOR_LIB_EXT})

    message(STATUS "Finding MySQL Connecotr lib: ${MYSQL_CONNECTOR_LIB_NAME}")

    find_path(MYSQL_CONNECTOR_PATH
    NAME LICENSE
    PATHS
        ${MYSQL_CONNECTOR_POSSIBLE_PATH}
    ENV MYSQL_CONNECTOR
    PATH_SUFFIXES
        include
        lib
        lib64
    NO_DEFAULT_PATH
    )   

    if (NOT MYSQL_CONNECTOR_PATH)
        message(FATAL_ERROR "Can not find MySQL Path")
    endif()

    set(MYSQL_CONNECTOR_INCLUDE ${MYSQL_CONNECTOR_PATH}/include)

    find_library(MYSQL_CONNECTOR_LIB
    NAME 
        ${MYSQL_CONNECTOR_LIB_NAME}
    PATHS
        ${MYSQL_CONNECTOR_PATH}
        ${MYSQL_CONNECTOR_POSSIBLE_PATH}
    ENV MYSQL_CONNECTOR
    PATH_SUFFIXES
        lib
        lib64
    NO_DEFAULT_PATH
    )

    if (NOT MYSQL_CONNECTOR_LIB)
        message(FATAL_ERROR "Can not find MySQL Connector library")
    endif()
    
    message(STATUS "Found MySQL Connector: ${MYSQL_CONNECTOR_LIB}")

else()
endif()

set(FLDB_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/db)
set(FLDB_INCLUDE 
    fl/db/Database.hpp
)
set(FLDB_SOURCE
    ${FLDB_INCLUDE}
    fl/db/Database.cpp
)

add_library(db STATIC ${FLDB_SOURCE})

set_target_properties(db PROPERTIES LINK_FLAGS "/ignore:4099")

target_include_directories(db
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

    $<BUILD_INTERFACE:${MYSQL_CONNECTOR_INCLUDE}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(db
INTERFACE
    OpenSSL::SSL    
PUBLIC
    Boost::system
    ${MYSQL_CONNECTOR_LIB}
    Fl::utils
)

install(TARGETS db EXPORT fl-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${FLDB_INCLUDE} DESTINATION ${FLDB_INSTALL_DIR})
install(DIRECTORY ${MYSQL_CONNECTOR_INCLUDE} DESTINATION .)