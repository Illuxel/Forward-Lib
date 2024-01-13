
enable_testing()

add_executable(TestDatabase     
    test_mysql.cpp
    test_sqlite.cpp)
    
target_link_libraries(TestDatabase
    GTest::gtest_main
    Forward::Database)

include(GoogleTest)
gtest_discover_tests(TestDatabase)