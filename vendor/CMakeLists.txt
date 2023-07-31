add_subdirectory(sqlite3-cmake)
add_subdirectory(jwt-cpp)

if (FL_ENABLE_TESTING)
    add_subdirectory(googletest)
endif()

if (FL_ENABLE_BENCHMARK)
    add_subdirectory(benchmark)
endif()
