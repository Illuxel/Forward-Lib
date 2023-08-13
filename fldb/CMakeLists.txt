cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

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

#--------------< Vars >--------------#

set(SQLITE3_PRIVATE_INCLUDE

)
set(SQLITE3_PRIVATE_SOURCE
    ${SQLITE3_PRIVATE_INCLUDE}

)

set(MYSQL_PRIVATE_INCLUDE
    fl/db/private/mysql/DriverLock.hpp
)
set(MYSQL_PRIVATE_SOURCE
    ${MYSQL_PRIVATE_INCLUDE}

    fl/db/private/mysql/DriverLock.cpp
)

set(FLDB_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/db)
set(FLDB_INCLUDE 

    fl/db/Core.hpp

    fl/db/Query.hpp
    fl/db/PreparedQuery.hpp
    fl/db/Result.hpp

    fl/db/DBConnection.hpp
    fl/db/Database.hpp
)
set(FLDB_SOURCE
    ${FLDB_INCLUDE}

    fl/db/Query.cpp
    fl/db/PreparedQuery.cpp
    fl/db/Result.cpp

    fl/db/DBConnection.cpp
    fl/db/Database.cpp
)

add_library(db STATIC 
    ${FLDB_SOURCE} 
    ${MYSQL_PRIVATE_SOURCE}
    ${SQLITE3_PRIVATE_SOURCE}
)

target_include_directories(db
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

    ${MYSQL_CPPCONN_INCLUDE}
PRIVATE
    ${MYSQL_PRIVATE_INCLUDE}
    ${SQLITE3_PRIVATE_INCLUDE}
)
target_link_libraries(db
PRIVATE
    OpenSSL::Crypto
    OpenSSL::SSL    

    ${MYSQL_LIB}
    ${MYSQL_CPPCONN_LIB}
PUBLIC
    SQLite::SQLite
    Fl::utils
)

install(FILES ${FLDB_INCLUDE} DESTINATION ${FLDB_INSTALL_DIR})
install(DIRECTORY ${MYSQL_CPPCONN_INCLUDE} DESTINATION .)

install(TARGETS db SQLite EXPORT fl-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)