add_executable(vector_test vector_test.cpp)
target_link_libraries(vector_test PRIVATE fabtex)
target_link_libraries(vector_test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

add_test(
    NAME "TestVector"
    COMMAND vector_test
)
