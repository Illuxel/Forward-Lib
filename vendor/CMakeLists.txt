add_subdirectory(jwt-cpp)

if (FL_ENABLE_TESING)
    add_subdirectory(googletest)
endif()

if (FL_ENABLE_BENCHMARK)
    add_subdirectory(benchmark)
endif()
