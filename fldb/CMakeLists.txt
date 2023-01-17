cmake_minimum_required(VERSION 3.24)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (BUILD_TESTING)
endif()

set(FLDB_INCLUDE 
    fl/db/Database.hpp
)
set(FLDB_SOURCE
    ${FLDB_INCLUDE}
    fl/db/Database.cpp
)

add_library(db STATIC ${FLDB_SOURCE})
target_include_directories(db
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(db
PUBLIC
    Qt6::Sql
    Fl::utils
)

install(FILES ${FLDB_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/db)
install(TARGETS db EXPORT fl-targets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)