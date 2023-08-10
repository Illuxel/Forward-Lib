cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(TEST_SOURCE
    test_main.cpp
    test_client.cpp
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_executable(TestClient ${TEST_SOURCE})

target_link_libraries(TestClient   
    GTest::gtest_main
    Fl::client
)

include(GoogleTest)
gtest_discover_tests(TestClient)