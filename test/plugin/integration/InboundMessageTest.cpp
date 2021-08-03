#include "pch/pch.h"
#include "integration/InboundMessage.h"
#include "integration/MessageType.h"

using testing::Test;
using UKControllerPlugin::Integration::InboundMessage;
using UKControllerPlugin::Integration::MessageType;

namespace UKControllerPluginTest::Integration {

    class InboundMessageTest : public Test
    { };

    TEST_F(InboundMessageTest, ItLoadsMessageTypeFromJson)
    {
        nlohmann::json messageData = {
            {"type", "test_message"},
            {"version", 2},
            {"data", {{"foo", "bar"}}}
        };
        auto parsedMessage = InboundMessage::FromJson(messageData);
        MessageType expectedMessageType{"test_message", 2};
        EXPECT_EQ(
            expectedMessageType,
            parsedMessage->GetMessageType()
        );
    }

    TEST_F(InboundMessageTest, ItLoadsMessageDataFromJson)
    {
        nlohmann::json messageData = {
            {"type", "test_message"},
            {"version", 2},
            {"data", {{"foo", "bar"}}}
        };
        auto parsedMessage = InboundMessage::FromJson(messageData);
        nlohmann::json expectedData{{"foo", "bar"}};
        EXPECT_EQ(
            expectedData,
            parsedMessage->GetMessageData()
        );
    }

    TEST_F(InboundMessageTest, FromJsonFailsToParseOnMissingType)
    {
        nlohmann::json messageData = {
            {"version", 2},
            {"data", {{"foo", "bar"}}}
        };

        EXPECT_EQ(nullptr, InboundMessage::FromJson(messageData));
    }

    TEST_F(InboundMessageTest, FromJsonFailsToParseOnTypeNotString)
    {
        nlohmann::json messageData = {
            {"type", 123},
            {"version", 2},
            {"data", {{"foo", "bar"}}}
        };

        EXPECT_EQ(nullptr, InboundMessage::FromJson(messageData));
    }

    TEST_F(InboundMessageTest, FromJsonFailsToParseOnMissingVersion)
    {
        nlohmann::json messageData = {
            {"type", "test_message"},
            {"data", {{"foo", "bar"}}}
        };

        EXPECT_EQ(nullptr, InboundMessage::FromJson(messageData));
    }

    TEST_F(InboundMessageTest, FromJsonFailsToParseOnVersionNotInteger)
    {
        nlohmann::json messageData = {
            {"type", "test_message"},
            {"version", "abc"},
            {"data", {{"foo", "bar"}}}
        };

        EXPECT_EQ(nullptr, InboundMessage::FromJson(messageData));
    }

    TEST_F(InboundMessageTest, FromJsonFailsToParseOnMissingData)
    {
        nlohmann::json messageData = {
            {"type", "test_message"},
            {"version", 2},
        };

        EXPECT_EQ(nullptr, InboundMessage::FromJson(messageData));
    }

    TEST_F(InboundMessageTest, FromJsonFailsToParseOnDataNotObject)
    {
        nlohmann::json messageData = {
            {"type", "test_message"},
            {"version", 2},
            {"data", nlohmann::json::array()}
        };

        EXPECT_EQ(nullptr, InboundMessage::FromJson(messageData));
    }

    TEST_F(InboundMessageTest, FromJsonFailsToParseOnMessageNotObject)
    {
        nlohmann::json messageData = nlohmann::json::array();
        EXPECT_EQ(nullptr, InboundMessage::FromJson(messageData));
    }
} // namespace UKControllerPluginTest::Integration
