#include "pch/pch.h"
#include "websocket/InterpretPusherWebsocketMessage.h"
#include "websocket/InboundWebsocketMessage.h"

using UKControllerPlugin::Websocket::InboundWebsocketMessage;

TEST(InterpretPusherMessageTest, ItReturnsInvalidIfJsonInvalid)
{
    EXPECT_EQ(invalidMessage, InterpretPusherMessage("{]"));
}

TEST(InterpretPusherMessageTest, ItReturnsInvalidIfNoEvent)
{
    nlohmann::json message;
    message["channel"] = "test-channel";
    message["data"] = { {"test", "lol"} };

    EXPECT_EQ(invalidMessage, InterpretPusherMessage(message.dump()));
}

TEST(InterpretPusherMessageTest, ItReturnsInvalidIfEventNotAString)
{
    nlohmann::json message;
    message["event"] = 1;
    message["channel"] = "test-channel";
    message["data"] = { {"test", "lol"} };

    EXPECT_EQ(invalidMessage, InterpretPusherMessage(message.dump()));
}

TEST(InterpretPusherMessageTest, ItHandlesMissingChannel)
{
    nlohmann::json message;
    message["event"] = "test-event";
    message["data"] = { {"test", "lol"} };

    InboundWebsocketMessage expectedMessage = {
        "test-event",
        "none",
        { {"test", "lol"} },
        false
    };

    EXPECT_EQ(expectedMessage, InterpretPusherMessage(message.dump()));
}

TEST(InterpretPusherMessageTest, ItHandlesNonStringChannel)
{
    nlohmann::json message;
    message["event"] = "test-event";
    message["channel"] = 1;
    message["data"] = { {"test", "lol"} };

    InboundWebsocketMessage expectedMessage = {
        "test-event",
        "none",
        { {"test", "lol"} },
        false
    };

    EXPECT_EQ(expectedMessage, InterpretPusherMessage(message.dump()));
}

TEST(InterpretPusherMessageTest, ItHandlesMissingData)
{
    nlohmann::json message;
    message["event"] = "test-event";
    message["channel"] = "test-channel";

    InboundWebsocketMessage expectedMessage = {
        "test-event",
        "test-channel",
        {},
        false
    };

    EXPECT_EQ(expectedMessage, InterpretPusherMessage(message.dump()));
}

TEST(InterpretPusherMessageTest, ItHandlesNonObjectData)
{
    nlohmann::json message;
    message["event"] = "test-event";
    message["channel"] = "test-channel";
    message["data"] = { 1, 2, 3 };

    InboundWebsocketMessage expectedMessage = {
        "test-event",
        "test-channel",
        {},
        false
    };

    EXPECT_EQ(expectedMessage, InterpretPusherMessage(message.dump()));
}

TEST(InterpretPusherMessageTest, ItDetectsProtocolMessages)
{
    nlohmann::json message;
    message["event"] = "pusher:test";
    message["channel"] = "test-channel";
    message["data"] = { {"test", "lol"} };

    InboundWebsocketMessage expectedMessage = {
        "pusher:test",
        "test-channel",
        { {"test", "lol"} },
        true
    };

    EXPECT_EQ(expectedMessage, InterpretPusherMessage(message.dump()));
}