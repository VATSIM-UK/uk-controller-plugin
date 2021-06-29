#include "pch/pch.h"
#include "integration/MessageType.h"

using testing::Test;
using UKControllerPlugin::Integration::MessageType;

namespace UKControllerPluginTest::Integration {

    class MessageTypeTargetTest : public Test
    { };

    TEST_F(MessageTypeTargetTest, IsEqualIfSame)
    {
        MessageType message1 = {"test1", 1};
        MessageType message2 = {"test1", 1};
        EXPECT_EQ(message1, message2);
    }

    TEST_F(MessageTypeTargetTest, IsNotEqualIfDifferentType)
    {
        MessageType message1 = {"test1", 1};
        MessageType message2 = {"test2", 1};
        EXPECT_NE(message1, message2);
    }

    TEST_F(MessageTypeTargetTest, IsNotEqualIfDifferentVersion)
    {
        MessageType message1 = {"test1", 1};
        MessageType message2 = {"test1", 2};
        EXPECT_NE(message1, message2);
    }
} // namespace UKControllerPluginTest::Integration
