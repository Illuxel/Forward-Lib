enable_testing()

add_executable(TestDatabase
    test_mysql.cpp
    test_sqlite.cpp
)
target_link_libraries(TestDatabase
    Forward::Database
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TestDatabase)