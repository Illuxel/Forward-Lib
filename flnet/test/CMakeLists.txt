enable_testing()

set(TEST_SOURCE test_main.cpp)

add_executable(TestNet ${TEST_SOURCE})

target_link_libraries(TestNet
    GTest::gtest_main
    Forward::Net
)

include(GoogleTest)
gtest_discover_tests(TestNet)