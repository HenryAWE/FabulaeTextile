add_executable(path_test path_test.cpp)
target_link_libraries(path_test PRIVATE fabtex)
target_link_libraries(path_test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

add_test(
    NAME "TestPath"
    COMMAND path_test
)
