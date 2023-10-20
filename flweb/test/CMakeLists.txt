enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(TestWeb 
    test_query.cpp
    test_url.cpp
    test_wfs.cpp
    test_responder.cpp
    test_reqsponse.cpp
)

target_link_libraries(TestWeb
    Forward::Web
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TestWeb)