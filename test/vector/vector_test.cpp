#include <gtest/gtest.h>
#include <iostream>
#include <fabtex/vector.hpp>


namespace fabtex
{
    template <std::size_t N, typename T>
    std::ostream& operator<<(std::ostream& os, const vec<N, T>& v)
    {
        os << '(';
        for(std::size_t i = 0; i < N; ++i)
        {
            if(i != 0)
                os << ',';
            os << v[i];
        }
        os << ')';
        return os;
    }
}

using namespace fabtex;

TEST(TestVector, BasicFunctions)
{
    vec<2, int> default_ctor_v2;
    EXPECT_EQ(default_ctor_v2[0], 0);
    EXPECT_EQ(default_ctor_v2[1], 0);

    const vec<2, int> iv2_0(-1);
    static_assert(iv2_0.size() == 2, "iv2_0.size() == 2 failed");
    EXPECT_EQ(iv2_0[0], -1);
    EXPECT_EQ(iv2_0[1], -1);
    for(auto& i : iv2_0)
        EXPECT_EQ(i, -1);

    vec<2, int> iv2_1(1, 2);
    EXPECT_EQ(iv2_1[0], 1);
    EXPECT_EQ(iv2_1[1], 2);

    auto iv2_2 = iv2_0;
    EXPECT_EQ(iv2_2[0], -1);
    EXPECT_EQ(iv2_2[1], -1);
    iv2_2 = iv2_1;
    EXPECT_EQ(iv2_2[0], 1);
    EXPECT_EQ(iv2_2[1], 2);
    EXPECT_FALSE(iv2_2 == iv2_0);
    EXPECT_TRUE(iv2_2 != iv2_0);
    EXPECT_TRUE(iv2_2 == iv2_1);}

TEST(TestVector, MathOperationsVec2)
{
    vec<2, int> v0(1, 2);
    vec<2, int> v1(3, 4);

    EXPECT_EQ((v0 + vec<2, int>(2, 2)), v1);
    EXPECT_EQ((v0 - vec<2, int>(2, 2)), (vec<2, int>(-1, 0)));
    EXPECT_EQ(v0 + 2, v1);
    EXPECT_EQ(2 + v0, v1);

    EXPECT_EQ((vec<2, int>(6, 4) * 2), (vec<2, int>(12, 8)));
    EXPECT_EQ((2 * vec<2, int>(6, 4)), (vec<2, int>(12, 8)));

    EXPECT_EQ((vec<2, int>(6, 4) / 2), (vec<2, int>(3, 2)));

    EXPECT_EQ((-vec<2, int>(6, 4)), (vec<2, int>(-6, -4)));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
