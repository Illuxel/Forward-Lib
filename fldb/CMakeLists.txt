cmake_minimum_required(VERSION 3.24)

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
    fl/db/DBConnection.hpp
    fl/db/Database.hpp
)
set(FLDB_SOURCE
    ${FLDB_INCLUDE}
    fl/db/DBConnection.cpp
    fl/db/Database.cpp
)

add_library(db STATIC ${FLDB_SOURCE})

target_include_directories(db
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

    $<BUILD_INTERFACE:${MYSQL_CPPCONN_INCLUDE}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(db
PRIVATE
    OpenSSL::Crypto
    OpenSSL::SSL    
    Boost::system
    ${MYSQL_LIB}
PUBLIC
    ${MYSQL_CPPCONN_LIB}
    Fl::utils
)

install(TARGETS db 
    EXPORT fl-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${FLDB_INCLUDE} DESTINATION ${FLDB_INSTALL_DIR})
install(DIRECTORY ${MYSQL_CPPCONN_INCLUDE} DESTINATION .)