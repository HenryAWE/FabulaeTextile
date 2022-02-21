#include <gtest/gtest.h>
#include <fabtex.hpp>
#include <fabtex/node.hpp>


using namespace fabtex;

TEST(TestNode, BasicFunctions)
{
    auto root = node("root");
    EXPECT_EQ(root.name(), "root");
    root.emplace("test");
    EXPECT_TRUE(root.contains("test"));
    EXPECT_TRUE(root.is_root());
    auto& test = root["test"];
    EXPECT_FALSE(test.is_root());
    EXPECT_EQ(test.name(), "test");
    EXPECT_TRUE(test.parent().is_root());
    EXPECT_EQ(&test, &test.parent()["test"]);

    auto test_copy = test;
    EXPECT_TRUE(test_copy.is_root());

    EXPECT_EQ(root.find(""), &root);
    EXPECT_EQ(root.find("test"), &test);
    root["test"].emplace("child1").first.emplace("child2");
    EXPECT_TRUE(root.exists("test/child1"));
    EXPECT_TRUE(root.exists("test/child1/child2"));
    EXPECT_FALSE(root.exists("test/not_exists"));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
