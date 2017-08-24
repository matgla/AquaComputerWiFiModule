#include "serializer/serializer.hpp"

#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "matcher/arrayCompare.hpp"

using namespace testing;

namespace serializer
{

TEST(SerializerShould, SerializeU64)
{
    const u64 dataToSerialize = 0xabcd12345678beef;
    const u8 expectedSerializedBuffer[] = {0xef, 0xbe, 0x78, 0x56, 0x34, 0x12, 0xcd, 0xab};

    u8 buffer[8];
    serialize(buffer, dataToSerialize);
    EXPECT_THAT(buffer, ArrayCompare(expectedSerializedBuffer, sizeof(expectedSerializedBuffer)));
}

TEST(SerializerShould, SerializeU8)
{
    const u8 dataToSerialize = 0xbe;
    const u8 expectedSerializedBuffer[] = {0xbe};

    u8 buffer[1];
    serialize(buffer, dataToSerialize);
    EXPECT_THAT(buffer, ArrayCompare(expectedSerializedBuffer, sizeof(expectedSerializedBuffer)));
}

TEST(SerializerShould, SerializeU16)
{
    const u16 dataToSerialize = 0xbeef;
    const u8 expectedSerializedBuffer[] = {0xef, 0xbe};

    u8 buffer[2];
    serialize(buffer, dataToSerialize);
    EXPECT_THAT(buffer, ArrayCompare(expectedSerializedBuffer, sizeof(expectedSerializedBuffer)));
}

TEST(SerializerShould, SerializeU32)
{
    const u32 dataToSerialize = 0xbeeffeeb;
    const u8 expectedSerializedBuffer[] = {0xeb, 0xfe, 0xef, 0xbe};

    u8 buffer[4];
    serialize(buffer, dataToSerialize);
    EXPECT_THAT(buffer, ArrayCompare(expectedSerializedBuffer, sizeof(expectedSerializedBuffer)));
}

TEST(SerializerShould, SerializeInt)
{
    const int dataToSerialize = 0xfadebade;
    const u8 expectedSerializedBuffer[] = {0xde, 0xba, 0xde, 0xfa};

    u8 buffer[4];
    serialize(buffer, dataToSerialize);
    EXPECT_THAT(buffer, ArrayCompare(expectedSerializedBuffer, sizeof(expectedSerializedBuffer)));
}

TEST(SerializerShould, SerializeChar)
{
    const char dataToSerialize = 0x56;
    const u8 expectedSerializedBuffer[] = {0x56};

    u8 buffer[1];
    serialize(buffer, dataToSerialize);
    EXPECT_THAT(buffer, ArrayCompare(expectedSerializedBuffer, sizeof(expectedSerializedBuffer)));
}

} // namespace serializer
