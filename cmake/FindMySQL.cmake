<<<<<<< Updated upstream

#--------------< MySQL Sufixes >--------------#

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    if (WIN32)
        set(MYSQL_LIB_PATH_SUFFIXES
            "lib/debug"
            "lib/vs14/debug"
            "lib64/debug"
            "lib64/vs14/debug"
        )
    else()
        set(MYSQL_LIB_PATH_SUFFIXES
            "lib"
            "lib64"
            "lib/x86_64-linux-gnu" # for debian
        )
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    if (WIN32)
        set(MYSQL_LIB_PATH_SUFFIXES
            "lib"
            "lib/vs14"
            "lib64"
            "lib64/vs14"
        )
    else()
        set(MYSQL_LIB_PATH_SUFFIXES
            "lib"
            "lib64"
            "lib/x86_64-linux-gnu" # for debian
        )
    endif()
else()
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

#--------------< MySQL C API Search >--------------#

set(MYSQL_LIB_NAME mysql)
set(MYSQL_ENV MYSQL_DIR MYSQL_ROOT)
set(MYSQL_POSSIBLE_PATH
    "C:/Program Files/MySQL/Connector C"
    "/usr"
)

if (WIN32)
    if (NOT FL_BUILD_STATIC)
        string(PREPEND MYSQL_LIB_NAME "lib")
        string(APPEND MYSQL_LIB_NAME ".dll")
    else()
        string(APPEND MYSQL_LIB_NAME "client")
        string(APPEND MYSQL_LIB_NAME ".lib")
    endif()
elseif(UNIX)
    string(PREPEND MYSQL_LIB_NAME "lib")

    if (NOT FL_BUILD_STATIC)
        string(APPEND MYSQL_LIB_NAME ".so")
    else()
        string(APPEND MYSQL_LIB_NAME "client")
        string(APPEND MYSQL_LIB_NAME ".a")
    endif()
endif()

message(STATUS "Finding MySQL C API: ${MYSQL_LIB_NAME}")

find_library(MYSQL_LIB
    NAME ${MYSQL_LIB_NAME}
    PATHS 
        ${MYSQL_POSSIBLE_PATH}
    ENV
        ${MYSQL_ENV}
    PATH_SUFFIXES 
        ${MYSQL_LIB_PATH_SUFFIXES}
    NO_DEFAULT_PATH
    NO_CACHE
)

if (NOT MYSQL_LIB)
    set(MYSQL_LIB "")
    message("Could NOT find MySQL C API file: ${MYSQL_LIB_NAME} build type: ${CMAKE_BUILD_TYPE}")
else()
    message(STATUS "Found MySQL C API: ${MYSQL_LIB}")
endif()

#--------------< MySQL Connector C++ Search >--------------#

set(MYSQL_CPPCONN_LIB_NAME mysqlcppconn)  

set(MYSQL_CPPCONN_POSSIBLE_PATH
    "C:/Program Files/MySQL/Connector C++"
    "C:/Program Files/MySQL/Connector C++ 8.0"
    "/usr"
)

set(MYSQL_CPPCONN_ENV
    MYSQL_CPPCONN_DIR
    MYSQL_CPPCONN_ROOT
    MYSQL_CONNECTOR_DIR
    MYSQL_CONNECTOR_ROOT
    MYSQL_CONNECTOR_CPP_DIR
    MYSQL_CONNECTOR_CPP_ROOT
)

if (FL_BUILD_STATIC)
    # To statically build MySQL Connector C++
    add_compile_definitions(
        CPPCONN_PUBLIC_FUNC=
    ) 
endif()

if (WIN32)

    if (NOT FL_BUILD_STATIC)
        string(APPEND MYSQL_CPPCONN_LIB_NAME ".dll")
    else()
        string(APPEND MYSQL_CPPCONN_LIB_NAME "-static")
        string(APPEND MYSQL_CPPCONN_LIB_NAME ".lib")
    endif()

elseif(UNIX)

    string(PREPEND MYSQL_CPPCONN_LIB_NAME "lib")

    if (NOT FL_BUILD_STATIC)
        string(APPEND MYSQL_CPPCONN_LIB_NAME ".so")
    else()
        string(APPEND MYSQL_CPPCONN_LIB_NAME "-static")
        string(APPEND MYSQL_CPPCONN_LIB_NAME ".a")
    endif()

endif()

message(STATUS "Finding MySQL Connector C++: ${MYSQL_CPPCONN_LIB_NAME}")

find_path(MYSQL_CPPCONN_INCLUDE
    NAMES cppconn
    PATHS
        ${MYSQL_CPPCONN_POSSIBLE_PATH}
    ENV 
        ${MYSQL_CPPCONN_ENV}
    PATH_SUFFIXES
        include
    NO_DEFAULT_PATH
    NO_CACHE
)   

if (NOT MYSQL_CPPCONN_INCLUDE)
    message(FATAL_ERROR "Could NOT find MySQL Connector C++ path")
endif()

find_library(MYSQL_CPPCONN_LIB
    NAME ${MYSQL_CPPCONN_LIB_NAME}
    PATHS 
        ${MYSQL_CPPCONN_POSSIBLE_PATH}
    ENV
        ${MYSQL_CPPCONN_ENV}
    PATH_SUFFIXES 
        ${MYSQL_LIB_PATH_SUFFIXES}
    NO_DEFAULT_PATH
    NO_CACHE
)

if (NOT MYSQL_CPPCONN_LIB)
    message(FATAL_ERROR "Could NOT find MySQL Connector C++ ${CMAKE_BUILD_TYPE}")
else()
    message(STATUS "Found MySQL Connector C++: ${MYSQL_CPPCONN_LIB}")
=======

#--------------< MySQL Sufixes >--------------#

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    if (WIN32)
        set(MYSQL_LIB_PATH_SUFFIXES
            "lib/debug"
            "lib/vs14/debug"
            "lib64/debug"
            "lib64/vs14/debug"
        )
    else()
        set(MYSQL_LIB_PATH_SUFFIXES
            "lib"
            "lib64"
            "lib/x86_64-linux-gnu" # for debian
        )
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    if (WIN32)
        set(MYSQL_LIB_PATH_SUFFIXES
            "lib"
            "lib/vs14"
            "lib64"
            "lib64/vs14"
        )
    else()
        set(MYSQL_LIB_PATH_SUFFIXES
            "lib"
            "lib64"
            "lib/x86_64-linux-gnu" # for debian
        )
    endif()
else()
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

#--------------< MySQL C API Search >--------------#

set(MYSQL_LIB_NAME mysql)
set(MYSQL_ENV MYSQL_DIR MYSQL_ROOT)
set(MYSQL_POSSIBLE_PATH
    "C:/Program Files/MySQL/Connector C"
    "/usr"
)

if (WIN32)
    if (NOT FL_BUILD_STATIC)
        string(PREPEND MYSQL_LIB_NAME "lib")
        string(APPEND MYSQL_LIB_NAME ".dll")
    else()
        string(APPEND MYSQL_LIB_NAME "client")
        string(APPEND MYSQL_LIB_NAME ".lib")
    endif()
elseif(UNIX)
    string(PREPEND MYSQL_LIB_NAME "lib")

    if (NOT FL_BUILD_STATIC)
        string(APPEND MYSQL_LIB_NAME ".so")
    else()
        string(APPEND MYSQL_LIB_NAME "client")
        string(APPEND MYSQL_LIB_NAME ".a")
    endif()
endif()

message(STATUS "Finding MySQL C API: ${MYSQL_LIB_NAME}")

find_library(MYSQL_LIB
    NAME ${MYSQL_LIB_NAME}
    PATHS 
        ${MYSQL_POSSIBLE_PATH}
    ENV
        ${MYSQL_ENV}
    PATH_SUFFIXES 
        ${MYSQL_LIB_PATH_SUFFIXES}
    NO_DEFAULT_PATH
    NO_CACHE
)

if (NOT MYSQL_LIB)
    set(MYSQL_LIB "")
    message("Could NOT find MySQL C API file: ${MYSQL_LIB_NAME} build type: ${CMAKE_BUILD_TYPE}")
else()
    message(STATUS "Found MySQL C API: ${MYSQL_LIB}")
endif()

#--------------< MySQL Connector C++ Search >--------------#

set(MYSQL_CPPCONN_LIB_NAME mysqlcppconn)  

set(MYSQL_CPPCONN_POSSIBLE_PATH
    "C:/Program Files/MySQL/Connector C++"
    "C:/Program Files/MySQL/Connector C++ 8.0"
    "/usr"
)

set(MYSQL_CPPCONN_ENV
    MYSQL_CPPCONN_DIR
    MYSQL_CPPCONN_ROOT
    MYSQL_CONNECTOR_DIR
    MYSQL_CONNECTOR_ROOT
    MYSQL_CONNECTOR_CPP_DIR
    MYSQL_CONNECTOR_CPP_ROOT
)

if (FL_BUILD_STATIC)
    # To statically build MySQL Connector C++
    add_compile_definitions(
        CPPCONN_PUBLIC_FUNC=
    ) 
endif()

if (WIN32)

    if (NOT FL_BUILD_STATIC)
        string(APPEND MYSQL_CPPCONN_LIB_NAME ".dll")
    else()
        string(APPEND MYSQL_CPPCONN_LIB_NAME "-static")
        string(APPEND MYSQL_CPPCONN_LIB_NAME ".lib")
    endif()

elseif(UNIX)

    string(PREPEND MYSQL_CPPCONN_LIB_NAME "lib")

    if (NOT FL_BUILD_STATIC)
        string(APPEND MYSQL_CPPCONN_LIB_NAME ".so")
    else()
        string(APPEND MYSQL_CPPCONN_LIB_NAME "-static")
        string(APPEND MYSQL_CPPCONN_LIB_NAME ".a")
    endif()

endif()

message(STATUS "Finding MySQL Connector C++: ${MYSQL_CPPCONN_LIB_NAME}")

find_path(MYSQL_CPPCONN_INCLUDE
    NAMES cppconn
    PATHS
        ${MYSQL_CPPCONN_POSSIBLE_PATH}
    ENV 
        ${MYSQL_CPPCONN_ENV}
    PATH_SUFFIXES
        include
    NO_DEFAULT_PATH
    NO_CACHE
)   

if (NOT MYSQL_CPPCONN_INCLUDE)
    message(FATAL_ERROR "Could NOT find MySQL Connector C++ path")
endif()

find_library(MYSQL_CPPCONN_LIB
    NAME ${MYSQL_CPPCONN_LIB_NAME}
    PATHS 
        ${MYSQL_CPPCONN_POSSIBLE_PATH}
    ENV
        ${MYSQL_CPPCONN_ENV}
    PATH_SUFFIXES 
        ${MYSQL_LIB_PATH_SUFFIXES}
    NO_DEFAULT_PATH
    NO_CACHE
)

if (NOT MYSQL_CPPCONN_LIB)
    message(FATAL_ERROR "Could NOT find MySQL Connector C++ ${CMAKE_BUILD_TYPE}")
else()
    message(STATUS "Found MySQL Connector C++: ${MYSQL_CPPCONN_LIB}")
>>>>>>> Stashed changes
endif()