cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

set(TEST_SOURCE
    test_main.cpp
    test_flags.cpp
    test_typedarg.cpp
    test_stringbuilder.cpp
    # test_log.cpp
    test_datetime.cpp
    test_mimetype.cpp
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_executable(TestUtils ${TEST_SOURCE})

target_link_libraries(TestUtils   
    GTest::gtest_main
    Fl::utils
)

include(GoogleTest)
gtest_discover_tests(TestUtils)