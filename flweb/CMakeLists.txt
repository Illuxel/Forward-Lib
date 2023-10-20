if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (FL_ENABLE_BENCHMARK)
    add_subdirectory(bench)
endif()

if (FL_ENABLE_TESTING)
    add_subdirectory(test)
endif()

set(WEB_INCLUDE
)
set(WEB_SOURCE
    ${WEB_INCLUDE}   
)

add_library(Forward.Web STATIC ${WEB_SOURCE})

target_include_directories(Forward.Web 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(Forward.Web
PUBLIC    
    Forward::Net
)

install(FILES ${WEB_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/web)

install(TARGETS Forward.Web jwt-cpp
    EXPORT fl-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
