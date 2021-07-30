#include "pch/pch.h"
#include "push/InterpretPushEvent.h"
#include "push/PushEvent.h"

using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::invalidMessage;
using UKControllerPlugin::Push::InterpretPushedEvent;

namespace UKControllerPluginTest::Push {
    TEST(InterpretPushEventTest, ItReturnsInvalidIfJsonInvalid)
    {
        EXPECT_EQ(invalidMessage, InterpretPushedEvent("{]"));
    }

    TEST(InterpretPushEventTest, ItReturnsInvalidIfNoEvent)
    {
        nlohmann::json message;
        message["channel"] = "test-channel";
        message["data"] = {{"test", "lol"}};

        EXPECT_EQ(invalidMessage, InterpretPushedEvent(message.dump()));
    }

    TEST(InterpretPushEventTest, ItReturnsInvalidIfEventNotAString)
    {
        nlohmann::json message;
        message["event"] = 1;
        message["channel"] = "test-channel";
        message["data"] = {{"test", "lol"}};

        EXPECT_EQ(invalidMessage, InterpretPushedEvent(message.dump()));
    }

    TEST(InterpretPushEventTest, ItHandlesMissingChannel)
    {
        nlohmann::json message;
        message["event"] = "test-event";
        message["data"] = {{"test", "lol"}};

        PushEvent expectedMessage = {
            "test-event",
            "none",
            {{"test", "lol"}},
            message.dump()
        };

        EXPECT_EQ(expectedMessage, InterpretPushedEvent(message.dump()));
    }

    TEST(InterpretPushEventTest, ItHandlesNonStringChannel)
    {
        nlohmann::json message;
        message["event"] = "test-event";
        message["channel"] = 1;
        message["data"] = {{"test", "lol"}};

        PushEvent expectedMessage = {
            "test-event",
            "none",
            {{"test", "lol"}},
            message.dump()
        };

        EXPECT_EQ(expectedMessage, InterpretPushedEvent(message.dump()));
    }

    TEST(InterpretPushEventTest, ItHandlesMissingData)
    {
        nlohmann::json message;
        message["event"] = "test-event";
        message["channel"] = "test-channel";

        EXPECT_EQ(invalidMessage, InterpretPushedEvent(message.dump()));
    }

    TEST(InterpretPushEventTest, ItHandlesNonObjectData)
    {
        nlohmann::json message;
        message["event"] = "test-event";
        message["channel"] = "test-channel";
        message["data"] = nlohmann::json::array({1, 2, 3});

        EXPECT_EQ(invalidMessage, InterpretPushedEvent(message.dump()));
    }
} // namespace UKControllerPluginTest::Push
