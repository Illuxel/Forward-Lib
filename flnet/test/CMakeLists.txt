cmake_minimum_required(VERSION 3.24)

set(TEST_SOURCE
    test_url.cpp
    test_reqsponse.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(TestNet ${TEST_SOURCE})

target_link_libraries(TestNet
PUBLIC    
    GTest::gtest_main
    Fl::net
)

include(GoogleTest)
gtest_discover_tests(TestNet)