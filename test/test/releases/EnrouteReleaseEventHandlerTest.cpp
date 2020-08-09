#include "pch/pch.h"
#include "releases/EnrouteReleaseEventHandler.h"
#include "mock/MockApiInterface.h"
#include "websocket/WebsocketSubscription.h"
#include "releases/EnrouteReleaseType.h"
#include "releases//CompareEnrouteReleaseTypes.h"
#include "websocket/WebsocketMessage.h"

using ::testing::Test;
using ::testing::NiceMock;
using UKControllerPlugin::Releases::EnrouteReleaseEventHandler;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Releases::CompareEnrouteReleaseTypes;
using UKControllerPlugin::Releases::EnrouteReleaseType;
using UKControllerPlugin::Websocket::WebsocketMessage;

namespace UKControllerPluginTest {
    namespace Releases {

        class EnrouteReleaseEventHandlerTest : public Test
        {
            public:
                EnrouteReleaseEventHandlerTest()
                    : releases({ { 1, "test1", "testdesc1" }, { 2, "test2", "testdesc2" } }),
                    handler(mockApi, releases)
                {

                }

                std::set<EnrouteReleaseType, CompareEnrouteReleaseTypes> releases;
                NiceMock<MockApiInterface> mockApi;
                EnrouteReleaseEventHandler handler;
        };

        TEST_F(EnrouteReleaseEventHandlerTest, ItSubscribesToChannels)
        {
            std::set<WebsocketSubscription> expectedSubscriptions;
            expectedSubscriptions.insert(
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-enroute-releases"
                }
            );
            EXPECT_EQ(expectedSubscriptions, this->handler.GetSubscriptions());
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItChecksIfTheMessageIsValid)
        {
            nlohmann::json data{
                {"callsign", "BAW123"},
                {"type", 1},
                {"release_point", "foo"},
                {"initiating_controller", "LON_S_CTR"},
                {"target_controller", "LON_C_CTR"}
            };
            EXPECT_TRUE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItChecksIfTheMessageIsValidWhenReleasePointIsNull)
        {
            nlohmann::json data{
                {"callsign", "BAW123"},
                {"type", 1},
                {"release_point", nlohmann::json::value_t::null},
                {"initiating_controller", "LON_S_CTR"},
                {"target_controller", "LON_C_CTR"}
            };
            EXPECT_TRUE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, MessageIsInvalidIfMissingCallsign)
        {
            nlohmann::json data{
                {"type", 1},
                {"release_point", "foo"},
                {"initiating_controller", "LON_S_CTR"},
                {"target_controller", "LON_C_CTR"}
            };
            EXPECT_FALSE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, MessageIsInvalidIfCallsignInvalid)
        {
            nlohmann::json data{
                {"callsign", 1},
                {"type", 1},
                {"release_point", "foo"},
                {"initiating_controller", "LON_S_CTR"},
                {"target_controller", "LON_C_CTR"}
            };
            EXPECT_FALSE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, MessageIsInvalidIfMissingType)
        {
            nlohmann::json data{
                {"callsign", "BAW123"},
                {"release_point", "foo"},
                {"initiating_controller", "LON_S_CTR"},
                {"target_controller", "LON_C_CTR"}
            };
            EXPECT_FALSE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, MessageIsInvalidIfTypeInvalid)
        {
            nlohmann::json data{
                {"callsign", "BAW123"},
                {"type", "abc"},
                {"release_point", "foo"},
                {"initiating_controller", "LON_S_CTR"},
                {"target_controller", "LON_C_CTR"}
            };
            EXPECT_FALSE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, MessageIsInvalidTypeUnknown)
        {
            nlohmann::json data{
                {"callsign", "BAW123"},
                {"type", 3},
                {"release_point", "foo"},
                {"initiating_controller", "LON_S_CTR"},
                {"target_controller", "LON_C_CTR"}
            };
            EXPECT_FALSE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, MessageIsInvalidIfMissingReleasePoint)
        {
            nlohmann::json data{
                {"callsign", "BAW123"},
                {"type", 1},
                {"initiating_controller", "LON_S_CTR"},
                {"target_controller", "LON_C_CTR"}
            };
            EXPECT_FALSE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, MessageIsInvalidIfReleasePointInvalid)
        {
            nlohmann::json data{
                {"callsign", "BAW123"},
                {"type", 1},
                {"release_point", 1},
                {"initiating_controller", "LON_S_CTR"},
                {"target_controller", "LON_C_CTR"}
            };
            EXPECT_FALSE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, MessageIsInvalidIfMissingInitiatingController)
        {
            nlohmann::json data{
                {"callsign", "BAW123"},
                {"type", 1},
                {"release_point", "foo"},
                {"target_controller", "LON_C_CTR"}
            };
            EXPECT_FALSE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, MessageIsInvalidIfInitiatingControllerInvalid)
        {
            nlohmann::json data{
                {"callsign", "BAW123"},
                {"type", 1},
                {"release_point", "foo"},
                {"initiating_controller", 1},
                {"target_controller", "LON_C_CTR"}
            };
            EXPECT_FALSE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, MessageIsInvalidIfMissingTargetController)
        {
            nlohmann::json data{
                {"callsign", "BAW123"},
                {"type", 1},
                {"release_point", "foo"},
                {"initiating_controller", "LON_S_CTR"},
            };
            EXPECT_FALSE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, MessageIsInvalidIfTargetControllerInvalid)
        {
            nlohmann::json data{
                {"callsign", "BAW123"},
                {"type", 1},
                {"release_point", "foo"},
                {"initiating_controller", "LON_S_CTR"},
                {"target_controller", 1}
            };
            EXPECT_FALSE(this->handler.ReleaseMessageValid(data));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItHasATagItemDescriptionForReleaseType)
        {
            EXPECT_EQ(
                "Enroute Release Type",
                this->handler.GetTagItemDescription(this->handler.enrouteReleaseTypeTagItemId)
            );
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItHasATagItemDescriptionForReleasePoint)
        {
            EXPECT_EQ(
                "Enroute Release Point",
                this->handler.GetTagItemDescription(this->handler.enrouteReleasePointTagItemId)
            );
        }
    }  // namespace Releases
}  // namespace UKControllerPluginTest
