cmake_minimum_required(VERSION 3.24)

set(TEST_SOURCE
    test_query.cpp
    test_url.cpp
    test_reqsponse.cpp
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_executable(TestNet ${TEST_SOURCE})

target_link_libraries(TestNet
PUBLIC    
    GTest::gtest_main
    Fl::net
)

include(GoogleTest)
gtest_discover_tests(TestNet)