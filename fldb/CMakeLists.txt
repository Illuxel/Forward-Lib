find_package(SQLite3 REQUIRED)

#--------------< Building tests and examples >--------------#

if (FL_ENABLE_EXAMPLE)
    # add_subdirectory(example)
endif()

if (FL_ENABLE_BENCHMARK)
    # add_subdirectory(bench)
endif()

if (FL_ENABLE_TEST)
    # add_subdirectory(test)
endif()

#--------------< Variables >--------------#

set(FLDB_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/db)
set(FLDB_INCLUDE 

    fl/db/Core.hpp

    fl/db/DBConnection.hpp

    fl/db/Query.hpp
    fl/db/PreparedQuery.hpp
    fl/db/Result.hpp
    
    fl/db/Database.hpp
)
set(FLDB_SOURCE
    ${FLDB_INCLUDE}

    fl/db/DBConnection.cpp
    
    fl/db/Query.cpp
    fl/db/PreparedQuery.cpp
    fl/db/Result.cpp

    fl/db/Database.cpp
)

#--------------< SQLite >--------------#

set(SQLITE3_INCLUDE

    fl/db/sqlite/SQLiteConnector.hpp
)
set(SQLITE3_SOURCE
    ${SQLITE3_INCLUDE}

    fl/db/sqlite/SQLiteConnector.cpp
)

add_compile_definitions(SQLITE_THREADSAFE=2)

#--------------< MYSQL >--------------#

set(MYSQL_INCLUDE

    fl/db/mysql/MySQLConnector.hpp
)
set(MYSQL_SOURCE
    ${MYSQL_INCLUDE}

    fl/db/mysql/MySQLConnector.cpp
)

#--------------< Project >--------------#

add_library(Forward.Database ${FLDB_SOURCE})

target_include_directories(Forward.Database
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(Forward.Database
PUBLIC
    Forward::Core
    SQLite::SQLite3 
)

install(FILES ${FLDB_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/db)
install(
    TARGETS  Forward.Database 
    EXPORT   fl-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)