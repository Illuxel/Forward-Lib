cmake_minimum_required(VERSION 3.24)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

#--------------< MySQL Connector Search >--------------#

set(MYSQL_CONNECTOR_LIB_NAME mysqlcppconn)
set(MYSQL_CONNECTOR_LIB_TYPE)
set(MYSQL_CONNECTOR_LIB_EXT)

set(MYSQL_CONNECTOR_POSSIBLE_PATH)
set(MYSQL_CONNECTOR_PATH_SUFFIXES)

set(MYSQL_CONNECTOR_INCLUDE)

if (WIN32)

    if (BUILD_SHARED_LIBS)
        set(MYSQL_CONNECTOR_LIB_EXT ".dll")
    else()
        set(MYSQL_CONNECTOR_LIB_TYPE "-static")
        set(MYSQL_CONNECTOR_LIB_EXT ".lib")
        # To statically build MySQL Connector C++
        add_compile_definitions(
            CPPCONN_PUBLIC_FUNC=
        ) 
    endif()

    set(MYSQL_CONNECTOR_POSSIBLE_PATH
        "C:\\Program Files\\MySQL"
        "C:\\Program Files\\MySQL\\Connector C++ 8.0"
    )

elseif(UNIX)

    if (BUILD_SHARED_LIBS)
        set(MYSQL_CONNECTOR_LIB_EXT ".so")
    else()
        set(MYSQL_CONNECTOR_LIB_EXT ".a")
        # To statically build MySQL Connector C++
        # add_compile_definitions(
        #     CPPCONN_PUBLIC_FUNC=
        # ) 
    endif()

    set(MYSQL_CONNECTOR_POSSIBLE_PATH
        "/usr/lib"
        "/usr/lib64"
        "/usr/local/lib"
        "/usr/local/lib64"
    )

endif()

string(APPEND MYSQL_CONNECTOR_LIB_NAME ${MYSQL_CONNECTOR_LIB_TYPE})
string(APPEND MYSQL_CONNECTOR_LIB_NAME ${MYSQL_CONNECTOR_LIB_EXT})

message(STATUS "Finding MySQL Connecotr lib: ${MYSQL_CONNECTOR_LIB_NAME}")

find_path(MYSQL_CONNECTOR_PATH
    NAME include
    PATHS
        ${MYSQL_CONNECTOR_POSSIBLE_PATH}
    ENV 
        MYSQL_CONNECTOR_DIR
        MYSQL_CONNECTOR_ROOT
    NO_DEFAULT_PATH
    NO_CACHE
)   

if (NOT MYSQL_CONNECTOR_PATH)
    message(FATAL_ERROR "Can not find MySQL Path")
endif()

message(STATUS "In root directory: ${MYSQL_CONNECTOR_PATH}")

set(MYSQL_CONNECTOR_INCLUDE ${MYSQL_CONNECTOR_PATH}/include)

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

    if (WIN32)
        set(MYSQL_CONNECTOR_PATH_SUFFIXES
            "lib/vs14/debug"
            "lib64/vs14/debug"
        )
    else()
        set(MYSQL_CONNECTOR_PATH_SUFFIXES
            "lib/debug"
            "lib64/debug"
        )
    endif()

elseif(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES MinSizeRel)

    if (WIN32)
        set(MYSQL_CONNECTOR_PATH_SUFFIXES
            "lib"
            "lib/vs14"
            "lib64"
            "lib64/vs14"
        )
    else()
        set(MYSQL_CONNECTOR_PATH_SUFFIXES
            "lib"
            "lib64"
        )
    endif()

else()
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

find_library(MYSQL_CONNECTOR_LIB
    NAME ${MYSQL_CONNECTOR_LIB_NAME}
    PATHS 
        ${MYSQL_CONNECTOR_PATH}
    ENV 
        MYSQL_CONNECTOR_DIR
        MYSQL_CONNECTOR_ROOT
    PATH_SUFFIXES 
        ${MYSQL_CONNECTOR_PATH_SUFFIXES}
    NO_DEFAULT_PATH
    NO_CACHE
)

if (NOT MYSQL_CONNECTOR_LIB)
    message(FATAL_ERROR "Cannot find MySQL Connector C++ ${CMAKE_BUILD_TYPE}")
endif()

message(STATUS "Found MySQL Connector C++: ${MYSQL_CONNECTOR_LIB}")

#--------------< Tests >--------------#

if (FL_ENABLE_TESING)
    add_subdirectory(test)
endif()

#--------------< Vars >--------------#

set(FLDB_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/db)
set(FLDB_INCLUDE 
    fl/db/Database.hpp
)
set(FLDB_SOURCE
    ${FLDB_INCLUDE}
    fl/db/Database.cpp
)

add_library(db STATIC ${FLDB_SOURCE})

target_link_options(db PUBLIC "/ignore:4099")

target_include_directories(db
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

    $<BUILD_INTERFACE:${MYSQL_CONNECTOR_INCLUDE}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(db
PRIVATE
    OpenSSL::Crypto
    OpenSSL::SSL    
    Boost::system
PUBLIC
    ${MYSQL_CONNECTOR_LIB}
    Fl::utils
)

install(TARGETS db EXPORT fl-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${FLDB_INCLUDE} DESTINATION ${FLDB_INSTALL_DIR})
install(DIRECTORY ${MYSQL_CONNECTOR_INCLUDE} DESTINATION .)