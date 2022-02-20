add_executable(core_test core_test.cpp)
target_link_libraries(core_test PRIVATE fabtex)
target_link_libraries(core_test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

add_test(
    NAME "TestCore"
    COMMAND core_test
)
