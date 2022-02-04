add_executable(bitmap_test bitmap_test.cpp)
target_link_libraries(bitmap_test PRIVATE fabtex)
target_link_libraries(bitmap_test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

add_test(
    NAME "TestBitmap"
    COMMAND bitmap_test
)
