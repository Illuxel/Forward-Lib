enable_testing()

add_executable(TestUtils     
    test_flags.cpp
    test_typedarg.cpp
    test_stringbuilder.cpp
    test_log.cpp
    test_datetime.cpp
    test_mimetype.cpp
)

target_link_libraries(TestUtils   
    GTest::gtest_main
    Forward::Core
)

include(GoogleTest)
gtest_discover_tests(TestUtils)