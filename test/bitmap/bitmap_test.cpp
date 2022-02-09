#include <gtest/gtest.h>
#include <fabtex/bitmap.hpp>


using namespace fabtex;

TEST(TestBitmap, BasicFunctions)
{
    bitmap_rgba8888 img;
    EXPECT_TRUE(img.empty());
    img.resize({ 4, 4 });
    EXPECT_EQ(img.size(), (vec<2, std::size_t>(4)));
    EXPECT_FALSE(img.valid({ 4, 4 }));
    EXPECT_FALSE(img.valid({ 0, 4 }));
    EXPECT_FALSE(img.valid({ 4, 0 }));
    for(std::size_t i = 0; i < 4; ++i)
    {
        for(std::size_t j = 0; j < 4; ++j)
        {
            EXPECT_EQ((img.index({ i, j })), (img[{ i, j}]));
            for(auto& k : img.index({ i, j }))
            {
                EXPECT_TRUE(img.valid({ i, j }));
                EXPECT_EQ(k, 0);
            }
        }
    }
    img.resize({ 8, 8 }, { 1, 1, 1, 1 });
    EXPECT_EQ(img.size(), (vec<2, std::size_t>(8)));
    EXPECT_FALSE(img.valid({ 8, 8 }));
    EXPECT_FALSE(img.valid({ 0, 8 }));
    EXPECT_FALSE(img.valid({ 8, 0 }));
    for (std::size_t i = 0; i < 8; ++i)
    {
        for (std::size_t j = 0; j < 8; ++j)
        {
            EXPECT_EQ((img.index({ i, j })), (img[{ i, j}]));
            for (auto& k : img.index({ i, j }))
            {
                EXPECT_TRUE(img.valid({ i, j }));
                EXPECT_EQ(k, 1);
            }
        }
    }
    img.index({ 2, 2 }) = { 3, 3, 3, 3 };
    EXPECT_EQ(img.index({2, 2}), (std::array<std::uint8_t, 4>{ 3, 3, 3, 3 }));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
