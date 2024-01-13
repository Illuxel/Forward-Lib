enable_testing()

add_executable(TestWeb 
    test_query.cpp
    test_urlparser.cpp
    test_url.cpp
    test_wfs.cpp
    test_responder.cpp
    test_reqsponse.cpp
)

target_link_libraries(TestWeb
    GTest::gtest_main
    Forward::Web)

include(GoogleTest)
gtest_discover_tests(TestWeb)