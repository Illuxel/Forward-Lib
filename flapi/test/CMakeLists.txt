enable_testing()

add_executable(TestAPI test_client.cpp)

target_link_libraries(TestAPI
    GTest::gtest_main
    Forward::API
)

add_executable(TestClient test_client.cpp)

target_link_libraries(TestClient
    GTest::gtest_main
    Forward::Client
)

include(GoogleTest)
gtest_discover_tests(TestClient)