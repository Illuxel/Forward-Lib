cmake_minimum_required(VERSION 3.24)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (BUILD_TESTING)
endif()

set(FLTYPES_INCLUDE 
    fl/types/Account.hpp
    fl/types/Channel.hpp
    fl/types/Chat.hpp
    fl/types/ForwardMessage.hpp
    fl/types/Message.hpp
)
set(FLTYPES_SOURCE
    ${FLTYPES_INCLUDE}
    fl/types/Account.cpp
    fl/types/Channel.cpp
    fl/types/Chat.cpp
    fl/types/ForwardMessage.cpp
    fl/types/Message.cpp
)

add_library(types STATIC ${FLTYPES_SOURCE})
target_include_directories(types
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(types
PUBLIC
    Qt6::Core
)

install(FILES ${FLTYPES_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/types)

install(TARGETS types EXPORT fl-targets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)