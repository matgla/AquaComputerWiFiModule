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
    const u8 expectedSerializedBuffer[] = {0xeb, 0xef, 0xef, 0xbe};

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

struct StructForTest
{
    int a;
    char b;
    std::string str;
};

TEST(SerializerShould, SerializeStruct)
{
    // StructForTest dataToSerialize;
    // dataToSerialize.a = 0x00000015;
    // dataToSerialize.b = 'a';
    // dataToSerialize.str = "I am working?";

    // const u8 expectedSerializedBuffer[] = {0x00, 0x00, 0x00, 0x15, 0x61, 0x49, 0x20, 0x61, 0x6d,
    //                                        0x20, 0x77, 0x6f, 0x72, 0x6b, 0x69, 0x6e, 0x67, 0x3f};

    // u8 buffer[1];
    // serialize(buffer, dataToSerialize);
    // EXPECT_THAT(buffer, ArrayCompare(expectedSerializedBuffer,
    // sizeof(expectedSerializedBuffer)));
}


} // namespace serializer
