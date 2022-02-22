#include <gtest/gtest.h>
#include <fabtex.hpp>
#include <fabtex/node.hpp>
#include <fabtex/data.hpp>


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

/*
Radical: 十 (U+5341)
Character: ⿰十十
*/
constexpr const char example_xml[] = 
R"(
<fabtex>
    <radical name="U+5341">
        <base>
            <stroke type="h"></stroke>
            <stroke type="s">
                <anchor src="b1" dst="../0/b1"></anchor>
            </stroke>
        </base>
    </radical>
    <character name="12">
        <base>
            <structure type="left_to_right">
                <left>U+5341</left>
                <right>U+5341</right>
            </structure>
        </base>
    </character>
</fabtex>
)";

TEST(TestNode, Loading)
{
    xml_parser parser;
    parser.parse(example_xml);
    EXPECT_EQ(parser.radicals()[0].name, "U+5341");
    EXPECT_EQ(parser.radicals()[0].strokes[0].type(), stroke_type::heng);
    EXPECT_EQ(parser.radicals()[0].strokes[1].type(), stroke_type::shu);
    EXPECT_EQ(parser.radicals()[0].strokes[1].connections.at("b1"), "../0/b1");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
