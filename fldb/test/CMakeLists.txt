cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

set(TEST_SOURCE 
    test_main.cpp
    test_mysql.cpp
    test_sqlite.cpp
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_executable(TestDb ${TEST_SOURCE})

target_link_libraries(TestDb
    GTest::gtest_main
    Fl::db
)

include(GoogleTest)
gtest_discover_tests(TestDb)