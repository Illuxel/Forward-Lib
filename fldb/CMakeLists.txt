cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif()

include(../cmake/FindMySQL.cmake)

#--------------< Tests >--------------#

if (FL_ENABLE_TESTING)
    add_subdirectory(test)
endif()

if (FL_ENABLE_BENCHMARKS)
    add_subdirectory(bench)
endif()

#--------------< Vars >--------------#

set(FLDB_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/db)
set(FLDB_INCLUDE 

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

set(FLDB_PRIVATE_INCLUDE
    fl/db/private/mysql/DriverLock.hpp
)
set(FLDB_PRIVATE_SOURCE
    ${FLDB_PRIVATE_INCLUDE}

    fl/db/private/mysql/DriverLock.cpp
)

add_library(db STATIC ${FLDB_SOURCE} ${FLDB_PRIVATE_SOURCE})

target_include_directories(db
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${MYSQL_CPPCONN_INCLUDE}>

    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
PRIVATE
    ${FLDB_PRIVATE_SOURCE}
)
target_link_libraries(db
PRIVATE
    OpenSSL::Crypto
    OpenSSL::SSL    
    Boost::system
    ${MYSQL_LIB}
    ${MYSQL_CPPCONN_LIB}
PUBLIC
    Fl::utils
)

install(TARGETS db 
    EXPORT fl-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${FLDB_INCLUDE} DESTINATION ${FLDB_INSTALL_DIR})
install(DIRECTORY ${MYSQL_CPPCONN_INCLUDE} DESTINATION .)