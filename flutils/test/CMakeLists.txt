enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(TestUtils     
    test_flags.cpp
    test_typedarg.cpp
    test_stringbuilder.cpp
    # test_log.cpp
    test_datetime.cpp
    test_mimetype.cpp
)

target_link_libraries(TestUtils   
    GTest::gtest_main
    Forward::Utils
)

include(GoogleTest)
gtest_discover_tests(TestUtils)