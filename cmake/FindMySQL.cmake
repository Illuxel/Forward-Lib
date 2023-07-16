
#--------------< MySQL Sufixes >--------------#

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

    if (WIN32)
        set(MYSQL_PATH_SUFFIXES
            "lib/debug"
            "lib/vs14/debug"
            "lib64/debug"
            "lib64/vs14/debug"
        )
    else()
        set(MYSQL_PATH_SUFFIXES
            "lib/debug"
            "lib64/debug"
        )
    endif()

elseif(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES MinSizeRel)

    if (WIN32)
        set(MYSQL_PATH_SUFFIXES
            "lib"
            "lib/vs14"
            "lib64"
            "lib64/vs14"
        )
    else()
        set(MYSQL_PATH_SUFFIXES
            "lib"
            "lib64"
        )
    endif()

else()
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

#--------------< MySQL C API Search >--------------#

set(MYSQL_INCLUDE)
set(MYSQL_LIB)

set(MYSQL_ENV
    MYSQL_DIR
    MYSQL_ROOT
)

set(MYSQL_POSSIBLE_PATH
    "C:/Program Files/MySQL/Connector C"
    "/usr/lib64"
    "/usr/local/lib64"
)

message(STATUS "Finding MySQL C API")

find_library(MYSQL_LIB
    NAME libmysql.lib
    PATHS 
        ${MYSQL_POSSIBLE_PATH}
    ENV
        ${MYSQL_ENV}
    PATH_SUFFIXES 
        ${MYSQL_PATH_SUFFIXES}
    NO_DEFAULT_PATH
    NO_CACHE
)

if (NOT MYSQL_LIB)
    message(FATAL_ERROR "Could NOT find MySQL C API ${CMAKE_BUILD_TYPE}")
endif()

message(STATUS "Found MySQL C API: ${MYSQL_LIB}")

#--------------< MySQL Connector C++ Search >--------------#

set(MYSQL_CPPCONN_LIB_NAME mysqlcppconn)  
set(MYSQL_CPPCONN_LIB_TYPE)
set(MYSQL_CPPCONN_LIB_EXT)

set(MYSQL_CPPCONN_POSSIBLE_PATH
    "C:/Program Files/MySQL/Connector C++"
    "C:/Program Files/MySQL/Connector C++ 8.0"
    "/usr/lib64"
    "/usr/local/lib64"
)

set(MYSQL_CPPCONN_ENV
    MYSQL_CPPCONN_DIR
    MYSQL_CPPCONN_ROOT
    MYSQL_CONNECTOR_DIR
    MYSQL_CONNECTOR_ROOT
    MYSQL_CONNECTOR_CPP_DIR
    MYSQL_CONNECTOR_CPP_ROOT
)

if (NOT BUILD_SHARED_LIBS)
    # To statically build MySQL Connector C++
    add_compile_definitions(
        CPPCONN_PUBLIC_FUNC=
    ) 
endif()

if (WIN32)

    if (BUILD_SHARED_LIBS)
        set(MYSQL_CPPCONN_LIB_EXT ".dll")
    else()
        set(MYSQL_CPPCONN_LIB_TYPE "-static")
        set(MYSQL_CPPCONN_LIB_EXT ".lib")
    endif()

elseif(UNIX)

    if (BUILD_SHARED_LIBS)
        set(MYSQL_CPPCONN_LIB_EXT ".so")
    else()
        set(MYSQL_CPPCONN_LIB_EXT ".a")
    endif()

endif()

string(APPEND MYSQL_CPPCONN_LIB_NAME ${MYSQL_CPPCONN_LIB_TYPE})
string(APPEND MYSQL_CPPCONN_LIB_NAME ${MYSQL_CPPCONN_LIB_EXT})

message(STATUS "Finding MySQL Connector C++: ${MYSQL_CPPCONN_LIB_NAME}")

find_path(MYSQL_CPPCONN_PATH
    NAME include
    PATHS
        ${MYSQL_POSSIBLE_PATH}
    ENV 
        ${MYSQL_CPPCONN_ENV}
    NO_DEFAULT_PATH
    NO_CACHE
)   

if (NOT MYSQL_CPPCONN_PATH)
    message(FATAL_ERROR "Could NOT find MySQL Connector C++ path")
endif()

set(MYSQL_CPPCONN_INCLUDE ${MYSQL_CPPCONN_PATH}/include)

find_library(MYSQL_CPPCONN_LIB
    NAME ${MYSQL_CPPCONN_LIB_NAME}
    PATHS 
        ${MYSQL_CPPCONN_PATH}
    ENV
        ${MYSQL_CPPCONN_ENV}
    PATH_SUFFIXES 
        ${MYSQL_PATH_SUFFIXES}
    NO_DEFAULT_PATH
    NO_CACHE
)

if (NOT MYSQL_CPPCONN_LIB)
    message(FATAL_ERROR "Could NOT find MySQL Connector C++ ${CMAKE_BUILD_TYPE}")
endif()

message(STATUS "Found MySQL Connector C++: ${MYSQL_CPPCONN_LIB}")