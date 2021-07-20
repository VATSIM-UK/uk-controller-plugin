#include "pch/pch.h"
#include "integration/MessageType.h"

using testing::Test;
using UKControllerPlugin::Integration::MessageType;

namespace UKControllerPluginTest::Integration {

    class MessageTypeTest : public Test
    { };

    TEST_F(MessageTypeTest, IsEqualIfSame)
    {
        MessageType message1 = {"test1", 1};
        MessageType message2 = {"test1", 1};
        EXPECT_EQ(message1, message2);
    }

    TEST_F(MessageTypeTest, IsNotEqualIfDifferentType)
    {
        MessageType message1 = {"test1", 1};
        MessageType message2 = {"test2", 1};
        EXPECT_NE(message1, message2);
    }

    TEST_F(MessageTypeTest, IsNotEqualIfDifferentVersion)
    {
        MessageType message1 = {"test1", 1};
        MessageType message2 = {"test1", 2};
        EXPECT_NE(message1, message2);
    }

    TEST_F(MessageTypeTest, ItConvertsToJson)
    {
        nlohmann::json expected{
            {"type", "test1"},
            {"version", 2}
        };

        MessageType message = {"test1", 2};
        EXPECT_EQ(expected, message.ToJson());
    }
} // namespace UKControllerPluginTest::Integration
