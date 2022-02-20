#include <gtest/gtest.h>
#include <fabtex.hpp>
#include <fabtex/node.hpp>


using namespace fabtex;

TEST(TestNode, BasicFunctions)
{
    auto root = node("root");
    root.emplace("test");
    EXPECT_TRUE(root.contains("test"));
    EXPECT_TRUE(root.is_root());
    auto& test = root["test"];
    EXPECT_FALSE(test.is_root());
    EXPECT_TRUE(test.parent().is_root());
    EXPECT_EQ(&test, &test.parent()["test"]);

    auto test_copy = test;
    EXPECT_TRUE(test_copy.is_root());
    test_copy = test;
    EXPECT_TRUE(test_copy.is_root());
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
