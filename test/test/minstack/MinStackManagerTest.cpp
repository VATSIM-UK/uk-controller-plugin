#include "pch/pch.h"
#include "minstack/MinStackManager.h"
#include "minstack/MinStackLevel.h"
#include "websocket/WebsocketSubscription.h"
#include "websocket/WebsocketMessage.h"

using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::MinStack::MinStackLevel;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPlugin::Websocket::WebsocketMessage;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace MinStack {

        class MinStackManagerTest : public Test
        {
            public:
                MinStackManager msl;
        };

        TEST_F(MinStackManagerTest, ItGeneratesTmaKeys)
        {
            EXPECT_EQ("tma.LTMA", this->msl.GetMslKeyTma("LTMA"));
        }

        TEST_F(MinStackManagerTest, ItGeneratesAirfieldKeys)
        {
            EXPECT_EQ("airfield.EGGD", this->msl.GetMslKeyAirfield("EGGD"));
        }

        TEST_F(MinStackManagerTest, ItCanStoreUserAcknowledgementsOfMinStackLevels)
        {
            this->msl.AddMsl(msl.GetMslKeyTma("LTMA"), "tma", "LTMA", 8000);
            EXPECT_FALSE(this->msl.GetMinStackLevel(msl.GetMslKeyTma("LTMA")).IsAcknowledged());
            this->msl.AcknowledgeMsl(msl.GetMslKeyTma("LTMA"));
            EXPECT_TRUE(this->msl.GetMinStackLevel(msl.GetMslKeyTma("LTMA")).IsAcknowledged());
        }

        TEST_F(MinStackManagerTest, ItReturnsAllThePossibleKeysForMinStacks)
        {
            this->msl.AddMsl(this->msl.GetMslKeyTma("LTMA"), "tma", "LTMA", 8000);
            this->msl.AddMsl(this->msl.GetMslKeyTma("STMA"), "tma", "STMA", 8000);


            std::set<std::string> expectedKeys = { this->msl.GetMslKeyTma("LTMA"), this->msl.GetMslKeyTma("STMA") };
            EXPECT_EQ(expectedKeys, this->msl.GetAllMslKeys());
        }

        TEST_F(MinStackManagerTest, ItReturnsInvalidMslIfKeyNotFound)
        {
            EXPECT_EQ(this->msl.invalidMsl, this->msl.GetMinStackLevel("nope"));
        }

        TEST_F(MinStackManagerTest, ItReturnsAMsl)
        {
            this->msl.AddMsl(this->msl.GetMslKeyTma("LTMA"), "tma", "LTMA", 8000);
            MinStackLevel minStack = this->msl.GetMinStackLevel(this->msl.GetMslKeyTma("LTMA"));

            EXPECT_EQ(8000, minStack.msl);
            EXPECT_EQ("tma", minStack.type);
            EXPECT_EQ("LTMA", minStack.name);
            EXPECT_FALSE(minStack.IsAcknowledged());
        }

        TEST_F(MinStackManagerTest, ItHasWebsocketSubscriptions)
        {
            std::set<WebsocketSubscription> expected = {
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-minstack-updates"
                }
            };
            EXPECT_EQ(expected, this->msl.GetSubscriptions());
        }

        TEST_F(MinStackManagerTest, ItAllowsManualMinStackUpdates)
        {
            this->msl.AddMsl(this->msl.GetMslKeyTma("LTMA"), "tma", "LTMA", 8000);
            this->msl.SetMinStackLevel(this->msl.GetMslKeyTma("LTMA"), 9000);

            EXPECT_EQ(9000, this->msl.GetMinStackLevel(this->msl.GetMslKeyTma("LTMA")).msl);
        }

        TEST_F(MinStackManagerTest, ItUpdatesMinStackLevelsFromJson)
        {
            nlohmann::json mslData;
            mslData["airfield"] = {
                {"EGBB", 7000}
            };
            mslData["tma"] = {
                {"MTMA", 6000}
            };

            WebsocketMessage message{
                "App\\Events\\MinStacksUpdatedEvent",
                "private-minstack-updates",
                mslData,
                false
            };

            this->msl.ProcessWebsocketMessage(message);
            EXPECT_EQ(7000, this->msl.GetMinStackLevel(this->msl.GetMslKeyAirfield("EGBB")).msl);
            EXPECT_EQ(6000, this->msl.GetMinStackLevel(this->msl.GetMslKeyTma("MTMA")).msl);
        }

        TEST_F(MinStackManagerTest, ItHandlesNullMslValues)
        {
            nlohmann::json mslData;
            mslData["airfield"] = {
                {"EGBB", nullptr}
            };
            mslData["tma"] = {
                {"MTMA", nullptr}
            };

            WebsocketMessage message{
                "App\\Events\\MinStacksUpdatedEvent",
                "private-minstack-updates",
                mslData,
                false
            };

            this->msl.ProcessWebsocketMessage(message);
            EXPECT_EQ(this->msl.invalidMsl, this->msl.GetMinStackLevel(this->msl.GetMslKeyAirfield("EGBB")).msl);
            EXPECT_EQ(this->msl.invalidMsl, this->msl.GetMinStackLevel(this->msl.GetMslKeyTma("MTMA")).msl);
        }

        TEST_F(MinStackManagerTest, ItDoesntDoManualMinStackUpdatesIfItDoesntExist)
        {
            EXPECT_NO_THROW(this->msl.SetMinStackLevel("nope", 8000));
        }
    }  // namespace MinStack
}  // namespace UKControllerPluginTest
