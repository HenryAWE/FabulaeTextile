#include <gtest/gtest.h>
#include <iostream>
#include <fabtex/path.hpp>


namespace fabtex
{
    template <char Separator>
    std::ostream& operator<<(std::ostream& os, const basic_path<char, Separator>& p)
    {
        os << to_string(p);
        return os;
    }
}

using namespace fabtex;

TEST(TestPath, BasicFunctions)
{
    path p("/1/2/3/");
    EXPECT_EQ(p.string(), "1/2/3");
    EXPECT_EQ(p.string(), to_string(p));
    p.concat("3/4");
    EXPECT_EQ(p.string(), "1/2/33/4");
    EXPECT_EQ(p.front(), "1");
    EXPECT_EQ(p.back(), "4");
    p.pop_back();
    EXPECT_EQ(p.string(), "1/2/33");
    p.pop_front();
    EXPECT_EQ(p.string(), "2/33");
    p.assign("///");
    EXPECT_TRUE(p.empty());
    p.concat("1/2");
    EXPECT_EQ(p.string(), "1/2");
    EXPECT_EQ(p, path("/1//2"));
    p.clear();
    EXPECT_TRUE(p.empty());
}

TEST(TestPath, Operators)
{
    path p("1");
    p = p / ("2" + p);
    EXPECT_EQ(p, "1/21");
    p = p / ("2" + p);
    EXPECT_EQ(p, "1/21/21/21");
    p = "";
    EXPECT_NE(p, "1/21/21/21");
    EXPECT_TRUE(p.empty());

    EXPECT_LE(p.compare("1"), 0);
    p = "2";
    EXPECT_GT(p.compare("1"), 0);
    p /= "3";
    EXPECT_EQ(p, "2/3");
    EXPECT_GT(p.compare("2/1"), 0);
    EXPECT_GT(p, "2/1");
    EXPECT_LE(p.compare("2/4"), 0);
    EXPECT_LE(p, "2/4");
    EXPECT_LE(p.compare("2/3/4"), 0);
    EXPECT_LE(p, "2/3/4");
    p += "4";
    EXPECT_EQ(p, "2/34");
}

TEST(TestPath, Iterator)
{
    path p("1/1/1/1/1");

    std::size_t count = 0;
    for(auto i : p)
    {
        EXPECT_EQ(i, "1");
        ++count;
    }
    EXPECT_EQ(count, 5);

    p = "1/2/3/4/5";
    int val = 1;
    for(auto i : p)
    {
        EXPECT_EQ(i, std::to_string(val));
        ++val;
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
