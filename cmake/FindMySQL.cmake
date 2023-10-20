#--------------< MySQL Path/Suffixes >--------------#

if (WIN32)

    set(MYSQL_CPPCONN_ENV
        MYSQL_CPPCONN_DIR
        MYSQL_CPPCONN_ROOT
        MYSQL_CONNECTOR_DIR
        MYSQL_CONNECTOR_ROOT
        MYSQL_CONNECTOR_CPP_DIR
        MYSQL_CONNECTOR_CPP_ROOT
    )
    set(MYSQL_CPPCONN_POSSIBLE_PATHS
        "C:/Program Files/MySQL/*/include"
        "C:/Program Files/MySQL/Connector C++ 8.0"
    )

    if (MSVC)
        set(MYSQL_LIB_PATH_SUFFIXES
            "lib/vs14"
            "lib64/vs14"
        )
    else()
        set(MYSQL_LIB_PATH_SUFFIXES
            "lib"
            "lib64"
        )
    endif()

    if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
        foreach(PREFIX ${MYSQL_LIB_PATH_SUFFIXES})
            list(APPEND TEMP_PREFIXES ${PREFIX}/debug)
        endforeach()
        set(MYSQL_LIB_PATH_SUFFIXES ${TEMP_PREFIXES})
    endif()

else()

    set(MYSQL_CPPCONN_POSSIBLE_PATHS
        "/usr"
    )

    set(MYSQL_LIB_PATH_SUFFIXES
        "lib"
        "lib64"
        "usr/lib"
        "usr/lib64"
        "lib/x86_64-linux-gnu" # for debian
        "lib64/x86_64-linux-gnu" # for debian
    )    
endif()

#--------------< MySQL Connector C++ Search >--------------#

message(STATUS "Finding MySQL Connector C++")

find_path(MYSQL_CPPCONN_INCLUDE
    NAMES jdbc cppconn
    PATHS
        ${MYSQL_CPPCONN_POSSIBLE_PATHS}
    ENV 
        ${MYSQL_CPPCONN_ENV}
    NO_DEFAULT_PATH
    NO_CACHE
)   

if (NOT MYSQL_CPPCONN_INCLUDE)
    message(FATAL_ERROR "Could NOT find MySQL Connector C++: Include Path")
endif()

if (NOT BUILD_STATIC OR NOT BUILD_SHARED_LIBS)

    #--------------< Try find dynamic lib >--------------#

    if (WIN32)
        set(MYSQL_CPPCONN_LIB_NAME "mysqlcppconn.lib") 
    elseif(UNIX)
        set(MYSQL_CPPCONN_LIB_NAME "libmysqlcppconn.so")
    endif()

    find_library(MYSQL_CPPCONN_LIB
        NAME ${MYSQL_CPPCONN_LIB_NAME}
        PATHS 
            ${MYSQL_CPPCONN_POSSIBLE_PATHS}
        ENV
            ${MYSQL_CPPCONN_ENV}
        PATH_SUFFIXES 
            ${MYSQL_LIB_PATH_SUFFIXES}
        NO_DEFAULT_PATH
        NO_CACHE
    )

    if (MYSQL_CPPCONN_LIB)
        message(STATUS "Found MySQL Connector C++: Dynamic Lib")
    else()

        message(STATUS "Could NOT find MySQL Connector C++: Dynamic Lib")

        if (WIN32)
            set(MYSQL_CPPCONN_LIB_NAME "mysqlcppconn-static.lib") 
        elseif(UNIX)
            set(MYSQL_CPPCONN_LIB_NAME "libmysqlcppconn-static.a")
        endif()

        #--------------< Try find static lib >--------------#

        find_library(MYSQL_CPPCONN_LIB
            NAME ${MYSQL_CPPCONN_LIB_NAME}
            PATHS 
                ${MYSQL_CPPCONN_POSSIBLE_PATHS}
            ENV
                ${MYSQL_CPPCONN_ENV}
            PATH_SUFFIXES 
                ${MYSQL_LIB_PATH_SUFFIXES}
            NO_DEFAULT_PATH
            NO_CACHE
        )

        if (NOT MYSQL_CPPCONN_LIB)
            message(FATAL_ERROR "Could NOT find MySQL Connector C++: Static Lib")
        endif()

        message(STATUS "Found MySQL Connector C++: Static Lib")

        # To statically build MySQL Connector C++
        add_compile_definitions(
            CPPCONN_PUBLIC_FUNC=
        )

    endif()
endif()