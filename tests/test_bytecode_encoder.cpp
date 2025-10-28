#include <gtest/gtest.h>
#include "bytecode_encoder.h"

TEST(BytecodeEncoderTest, PacksCorrectly) {
    be::BytecodeEncoder<32> encoder({5, 27});
    auto packed = encoder.pack({1, 35});
    uint64_t test = 134217763;
    ASSERT_EQ(packed, test);
}

TEST(BytecodeEncoderTest, UnpacksCorrectly) {
    be::BytecodeEncoder<32> encoder({5, 27});
    auto unpacked = encoder.unpack(134217763);
    ASSERT_EQ(unpacked[0], 1);
    ASSERT_EQ(unpacked[1], 35);
}

TEST(BytecodeEncoderTest, OneFieldIs64bit) {
    be::BytecodeEncoder<64> encoder({64});
    auto packed = encoder.pack({1});
    auto res = encoder.unpack(packed);
    uint64_t test = 1;
    ASSERT_EQ(res[0], test);
}

TEST(BytecodeEncoderTest, OneFieldIs32bit) {
    be::BytecodeEncoder<64> encoder({32});
    auto packed = encoder.pack({1});
    auto res = encoder.unpack(packed);
    uint64_t test = 1;
    ASSERT_EQ(res[0], test);
}

TEST(BytecodeEncoderTest, PacksMaximumValuesAllowedPerWidthOfSingleInfoPoint) {
    be::BytecodeEncoder<8> encoder({4, 4});
    auto packed = encoder.pack({15, 15});
    auto res = encoder.unpack(packed);

    ASSERT_EQ(res[0], 15);
    ASSERT_EQ(res[1], 15);
}

TEST(BytecodeEncoderTest, ValueExceedsItsBitWidthMaximum) {
    be::BytecodeEncoder<8> encoder({4, 4});
    ASSERT_THROW(encoder.pack({16, 1}), std::runtime_error);
}

TEST(BytecodeEncoderTest, UnusedBits) {
    be::BytecodeEncoder<8> encoder({3, 3});
    auto packed = encoder.pack({7, 7});
    auto res = encoder.unpack(packed);
    ASSERT_EQ(res[0], 7);
    ASSERT_EQ(res[1], 7);

    // Check last 2 bits of packed are unused so unchanged to 0
    ASSERT_EQ(packed, 252);
}


TEST(BytecodeEncoderTest, Non2MultipleWidth) {
    be::BytecodeEncoder<17> encoder({10, 7});
    auto packed = encoder.pack({7, 7});
    auto res = encoder.unpack(packed);
    ASSERT_EQ(res[0], 7);
    ASSERT_EQ(res[1], 7);
}


TEST(BytecodeEncoderTest, Non2MultipleWidthUnusedBits) {
    be::BytecodeEncoder<17> encoder({6, 6});
    auto packed = encoder.pack({63, 63});
    auto res = encoder.unpack(packed);
    ASSERT_EQ(res[0], 63);
    ASSERT_EQ(res[1], 63);

    // Check last 5 bits of packed are unused so unchanged to 0
    ASSERT_EQ(packed, 131040);
}

TEST(BytecodeEncoderTest, ExcededSizesInConstructor) {
    ASSERT_DEATH(be::BytecodeEncoder<32> encoder({5, 27, 1}),
                 "total_size <= width && \"Total size of fields exceeds the specified width\"");
}
