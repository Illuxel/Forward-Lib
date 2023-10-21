if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif()

include(../cmake/FindMySQL.cmake)

#--------------< Tests >--------------#

if (FL_ENABLE_BENCHMARKS)
    add_subdirectory(bench)
endif()

if (FL_ENABLE_TESTING)
    add_subdirectory(test)
endif()

#--------------< Core Vars >--------------#

set(FLDB_INCLUDE 

    fl/db/Core.hpp

    fl/db/Query.hpp
    fl/db/PreparedQuery.hpp
    fl/db/Result.hpp
    
    fl/db/Configuration.hpp
    fl/db/Connection.hpp
    fl/db/Database.hpp
)
set(FLDB_SOURCE
    ${FLDB_INCLUDE}

    fl/db/Query.cpp
    fl/db/PreparedQuery.cpp
    fl/db/Result.cpp

    fl/db/Configuration.cpp
    fl/db/Connection.cpp
    fl/db/Database.cpp
)

#--------------< SQLite >--------------#

set(SQLITE3_INCLUDE
    sqlite3/sqlite3.h
    sqlite3/sqlite3ext.h
)
set(SQLITE3_SOURCE
    ${SQLITE3_INCLUDE}

    sqlite3/shell.c
    sqlite3/sqlite3.c
)

add_compile_definitions(SQLITE_THREADSAFE=2)

#--------------< MYSQL >--------------#

set(MYSQL_SOURCE

    fl/db/mysql/MySQLConnection.hpp
)

if (MYSQL_CPPCONN_STATIC)
    add_compile_definitions(CPPCONN_PUBLIC_FUNC=)
endif()

#--------------< Project >--------------#

add_library(Forward.Database 
    ${FLDB_SOURCE} 
    ${SQLITE3_SOURCE}
    ${MYSQL_SOURCE}
)

target_include_directories(Forward.Database
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    ${MYSQL_CPPCONN_INCLUDE}
)
target_link_libraries(Forward.Database
PRIVATE
    OpenSSL::Crypto
    OpenSSL::SSL
    ${MYSQL_CPPCONN_LIB}
PUBLIC
    Forward::Core
)

install(FILES ${FLDB_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/db)
install(FILES ${SQLITE3_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${MYSQL_CPPCONN_INCLUDE} DESTINATION .)

install(
    TARGETS  Forward.Database 
    EXPORT   fl-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)