#include "pch/pch.h"
#include "releases/EnrouteReleaseEventHandler.h"
#include "mock/MockApiInterface.h"
#include "websocket/WebsocketSubscription.h"
#include "releases/EnrouteReleaseType.h"
#include "releases/EnrouteRelease.h"
#include "releases/CompareEnrouteReleaseTypes.h"
#include "websocket/WebsocketMessage.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "tag/TagData.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "plugin/PopupMenuItem.h"
#include "mock/MockTaskRunnerInterface.h"
#include "mock/MockEuroScopeCControllerInterface.h"
#include "api/ApiException.h"

using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;
using ::testing::_;
using UKControllerPlugin::Releases::EnrouteReleaseEventHandler;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPlugin::Releases::CompareEnrouteReleaseTypes;
using UKControllerPlugin::Releases::EnrouteReleaseType;
using UKControllerPlugin::Releases::EnrouteRelease;
using UKControllerPlugin::Websocket::WebsocketMessage;
using UKControllerPlugin::Tag::TagData;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Api::ApiException;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest {
    namespace Releases {

        class EnrouteReleaseEventHandlerTest : public Test
        {
            public:
                EnrouteReleaseEventHandlerTest()
                    : releases({ { 1, "test1", "testdesc1" }, { 2, "test2", "testdesc2" } }),
                    handler(mockApi, plugin, taskRunner, releases, 101, 102)
                {

                }

                double fontSize = 24.1;
                COLORREF tagColour = RGB(255, 255, 255);
                int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
                char itemString[16] = "Foooooo";
                MockTaskRunnerInterface taskRunner;
                NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
                NiceMock<MockEuroScopeCControllerInterface> transferringController;
                NiceMock<MockEuroScopeCControllerInterface> targetController;
                NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
                NiceMock< MockEuroScopeCRadarTargetInterface> radarTarget;
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

        TEST_F(EnrouteReleaseEventHandlerTest, ItReceivesIncomingReleases)
        {
            std::shared_ptr<MockEuroScopeCControllerInterface> myController =
                std::make_shared<MockEuroScopeCControllerInterface>();

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(myController));

            ON_CALL(*myController, GetCallsign())
                .WillByDefault(Return("LON_C_CTR"));

            WebsocketMessage message{
                "App\\Events\\EnrouteReleaseEvent",
                "private-enroute-releases",
                nlohmann::json {
                    {"callsign", "BAW123"},
                    {"type", 1},
                    {"initiating_controller", "LON_S_CTR"},
                    {"target_controller", "LON_C_CTR"},
                    {"release_point", nlohmann::json::value_t::null}
                }
            };

            this->handler.ProcessWebsocketMessage(message);

            const EnrouteRelease& incomingRelease = this->handler.GetIncomingRelease("BAW123");
            EXPECT_EQ(1, incomingRelease.releaseType);
            EXPECT_EQ("", incomingRelease.releasePoint);
            std::chrono::minutes minutesFromNow = std::chrono::duration_cast<std::chrono::minutes>(
                incomingRelease.clearTime - std::chrono::system_clock::now() + std::chrono::seconds(10)
            );
            EXPECT_EQ(std::chrono::minutes(3), minutesFromNow);
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItReceivesIncomingReleasesWithReleasePoints)
        {
            std::shared_ptr<MockEuroScopeCControllerInterface> myController =
                std::make_shared<MockEuroScopeCControllerInterface>();

            ON_CALL(this->plugin, GetUserControllerObject)
                .WillByDefault(Return(myController));

            ON_CALL(*myController, GetCallsign())
                .WillByDefault(Return("LON_C_CTR"));

            WebsocketMessage message{
                "App\\Events\\EnrouteReleaseEvent",
                "private-enroute-releases",
                nlohmann::json {
                    {"callsign", "BAW123"},
                    {"type", 1},
                    {"initiating_controller", "LON_S_CTR"},
                    {"target_controller", "LON_C_CTR"},
                    {"release_point", "ARNUN"}
                }
            };

            this->handler.ProcessWebsocketMessage(message);

            const EnrouteRelease& incomingRelease = this->handler.GetIncomingRelease("BAW123");
            EXPECT_EQ(1, incomingRelease.releaseType);
            EXPECT_EQ("ARNUN", incomingRelease.releasePoint);
            std::chrono::minutes minutesFromNow = std::chrono::duration_cast<std::chrono::minutes>(
                incomingRelease.clearTime - std::chrono::system_clock::now() + std::chrono::seconds(10)
            );
            EXPECT_EQ(std::chrono::minutes(3), minutesFromNow);
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDoesntAddIncomingReleasesIfNotTargetingUser)
        {
            std::shared_ptr<MockEuroScopeCControllerInterface> myController =
                std::make_shared<MockEuroScopeCControllerInterface>();

            ON_CALL(this->plugin, GetUserControllerObject)
                .WillByDefault(Return(myController));

            ON_CALL(*myController, GetCallsign())
                .WillByDefault(Return("LON_D_CTR"));

            WebsocketMessage message{
                "App\\Events\\EnrouteReleaseEvent",
                "private-enroute-releases",
                nlohmann::json {
                    {"callsign", "BAW123"},
                    {"type", 1},
                    {"initiating_controller", "LON_S_CTR"},
                    {"target_controller", "LON_C_CTR"},
                    {"release_point", "ARNUN"}
                }
            };

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(this->handler.invalidRelease, this->handler.GetIncomingRelease("BAW123"));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDoesntAddIncomingReleasesIfNoUserController)
        {
            ON_CALL(this->plugin, GetUserControllerObject)
                .WillByDefault(Return(nullptr));

            WebsocketMessage message{
                "App\\Events\\EnrouteReleaseEvent",
                "private-enroute-releases",
                nlohmann::json {
                    {"callsign", "BAW123"},
                    {"type", 1},
                    {"initiating_controller", "LON_S_CTR"},
                    {"target_controller", "LON_C_CTR"},
                    {"release_point", "ARNUN"}
                }
            };

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(this->handler.invalidRelease, this->handler.GetIncomingRelease("BAW123"));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItRejectsInvalidReleaseMessages)
        {
            WebsocketMessage message{
                "App\\Events\\EnrouteReleaseEvent",
                "private-enroute-releases",
                nlohmann::json {
                    {"callsign", "BAW123"},
                    {"type", 1},
                    {"initiating_controller", "LON_S_CTR"},
                    {"target_controller", "LON_C_CTR"},
                }
            };

            this->handler.ProcessWebsocketMessage(message);

            EXPECT_EQ(this->handler.invalidRelease, this->handler.GetIncomingRelease("BAW123"));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItIgnoresIncorrectEvents)
        {
            WebsocketMessage message{
                "App\\Events\\NotEnrouteReleaseEvent",
                "private-enroute-releases",
                nlohmann::json {
                    {"callsign", "BAW123"},
                    {"type", 1},
                    {"initiating_controller", "LON_S_CTR"},
                    {"target_controller", "LON_C_CTR"},
                    {"release_point", nlohmann::json::value_t::null}
                }
            };

            this->handler.ProcessWebsocketMessage(message);

            EXPECT_EQ(this->handler.invalidRelease, this->handler.GetIncomingRelease("BAW123"));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDisplaysOutgoingReleaseTypeTagItem)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            TagData tagData(
                this->flightplan,
                this->radarTarget,
                108,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize
            );

            this->handler.AddOutgoingRelease("BAW123", {1, ""});
            this->handler.SetTagItemData(tagData);

            EXPECT_EQ("test1", tagData.GetItemString());
            EXPECT_EQ(EuroScopePlugIn::TAG_COLOR_ONGOING_REQUEST_FROM_ME, tagData.GetEuroscopeColourCode());
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDisplaysIncomingReleaseTypeTagItem)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            TagData tagData(
                this->flightplan,
                this->radarTarget,
                108,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize
            );

            this->handler.AddIncomingRelease("BAW123", {1, ""});
            this->handler.SetTagItemData(tagData);

            EXPECT_EQ("test1", tagData.GetItemString());
            EXPECT_EQ(EuroScopePlugIn::TAG_COLOR_TRANSFER_TO_ME_INITIATED, tagData.GetEuroscopeColourCode());
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItPrefersOutgoingReleaseTypeTagItem)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            TagData tagData(
                this->flightplan,
                this->radarTarget,
                108,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize
            );

            this->handler.AddIncomingRelease("BAW123", { 1, "" });
            this->handler.AddOutgoingRelease("BAW123", { 2, "" });
            this->handler.SetTagItemData(tagData);

            EXPECT_EQ("test2", tagData.GetItemString());
            EXPECT_EQ(EuroScopePlugIn::TAG_COLOR_ONGOING_REQUEST_FROM_ME, tagData.GetEuroscopeColourCode());
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItHandlesNoReleaseForCallsign)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW345"));

            TagData tagData(
                this->flightplan,
                this->radarTarget,
                108,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize
            );

            this->handler.AddOutgoingRelease("BAW123", {1, ""});
            this->handler.SetTagItemData(tagData);

            EXPECT_EQ("", tagData.GetItemString());
            EXPECT_EQ(EuroScopePlugIn::TAG_COLOR_ASSUMED, tagData.GetEuroscopeColourCode());
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDisplaysOutgoingReleasePointTagItem)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            TagData tagData(
                this->flightplan,
                this->radarTarget,
                109,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize
            );

            this->handler.AddOutgoingRelease("BAW123", { 1, "ARNUN" });
            this->handler.SetTagItemData(tagData);

            EXPECT_EQ("ARNUN", tagData.GetItemString());
            EXPECT_EQ(EuroScopePlugIn::TAG_COLOR_ONGOING_REQUEST_FROM_ME, tagData.GetEuroscopeColourCode());
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDisplaysNoOutgoingReleasePointTagItem)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            TagData tagData(
                this->flightplan,
                this->radarTarget,
                109,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize
            );

            this->handler.AddOutgoingRelease("BAW123", { 1, "" });
            this->handler.SetTagItemData(tagData);

            EXPECT_EQ("", tagData.GetItemString());
            EXPECT_EQ(EuroScopePlugIn::TAG_COLOR_ASSUMED, tagData.GetEuroscopeColourCode());
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDisplaysIncomingReleasePointTagItem)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            TagData tagData(
                this->flightplan,
                this->radarTarget,
                109,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize
            );

            this->handler.AddIncomingRelease("BAW123", { 1, "ABTUM" });
            this->handler.SetTagItemData(tagData);

            EXPECT_EQ("ABTUM", tagData.GetItemString());
            EXPECT_EQ(EuroScopePlugIn::TAG_COLOR_TRANSFER_TO_ME_INITIATED, tagData.GetEuroscopeColourCode());
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDisplaysNoIncomingReleasePointTagItem)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            TagData tagData(
                this->flightplan,
                this->radarTarget,
                109,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize
            );

            this->handler.AddIncomingRelease("BAW123", { 1, "" });
            this->handler.SetTagItemData(tagData);

            EXPECT_EQ("", tagData.GetItemString());
            EXPECT_EQ(EuroScopePlugIn::TAG_COLOR_ASSUMED, tagData.GetEuroscopeColourCode());
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItPrefersOutgoingReleasePointTagItem)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            TagData tagData(
                this->flightplan,
                this->radarTarget,
                109,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize
            );

            this->handler.AddIncomingRelease("BAW123", { 1, "ARNUN" });
            this->handler.AddOutgoingRelease("BAW123", { 2, "ABTUM" });
            this->handler.SetTagItemData(tagData);

            EXPECT_EQ("ABTUM", tagData.GetItemString());
            EXPECT_EQ(EuroScopePlugIn::TAG_COLOR_ONGOING_REQUEST_FROM_ME, tagData.GetEuroscopeColourCode());
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItClearsExpiredReleases)
        {
            TagData tagData(
                this->flightplan,
                this->radarTarget,
                109,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize
            );

            this->handler.AddIncomingRelease(
                "BAW123",
                { 1, "ARNUN", std::chrono::system_clock::now() + std::chrono::seconds(3) }
            );
            this->handler.AddOutgoingRelease(
                "BAW123",
                { 2, "ABTUM", std::chrono::system_clock::now() + std::chrono::seconds(3) }
            );

            this->handler.AddIncomingRelease(
                "BAW456",
                { 1, "ARNUN", std::chrono::system_clock::now() - std::chrono::seconds(1) }
            );
            this->handler.AddOutgoingRelease(
                "BAW456",
                { 2, "ABTUM", std::chrono::system_clock::now() - std::chrono::seconds(1) }
            );
            this->handler.TimedEventTrigger();

            EXPECT_EQ("ARNUN", this->handler.GetIncomingRelease("BAW123").releasePoint);
            EXPECT_EQ("ABTUM", this->handler.GetOutgoingRelease("BAW123").releasePoint);
            EXPECT_EQ(this->handler.invalidRelease, this->handler.GetIncomingRelease("BAW456"));
            EXPECT_EQ(this->handler.invalidRelease, this->handler.GetOutgoingRelease("BAW456"));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItTriggersTheReleaseTypeMenu)
        {
            PopupMenuItem menuItemRelease1;
            menuItemRelease1.firstValue = "test1";
            menuItemRelease1.secondValue = "testdesc1";
            menuItemRelease1.callbackFunctionId = 101;
            menuItemRelease1.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemRelease1.disabled = false;
            menuItemRelease1.fixedPosition = false;

            PopupMenuItem menuItemRelease2;
            menuItemRelease2.firstValue = "test2";
            menuItemRelease2.secondValue = "testdesc2";
            menuItemRelease2.callbackFunctionId = 101;
            menuItemRelease2.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemRelease2.disabled = false;
            menuItemRelease2.fixedPosition = false;

            PopupMenuItem menuItemReleaseNone;
            menuItemReleaseNone.firstValue = "NONE";
            menuItemReleaseNone.secondValue = "No Release";
            menuItemReleaseNone.callbackFunctionId = 101;
            menuItemReleaseNone.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemReleaseNone.disabled = false;
            menuItemReleaseNone.fixedPosition = true;

            RECT expectedArea = { 0, 0, 400, 600 };
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "Release Type", 2))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemRelease1))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemRelease2))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemReleaseNone))
                .Times(1);

            this->handler.DisplayReleaseTypeMenu(this->flightplan, this->radarTarget, "", { 0, 0 });
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDoesNotAddOutgoingReleaseIfAircraftNotOnline)
        {
            EXPECT_CALL(this->plugin, GetSelectedFlightplan())
                .Times(1)
                .WillOnce(Return(nullptr));

            this->handler.AddOutgoingRelease("BAW123", { 2, "ABTUM" });
            this->handler.ReleaseTypeSelected(1, "test1", {});
            EXPECT_EQ(2, this->handler.GetOutgoingRelease("BAW123").releaseType);
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDoesNotAddOutgoingReleaseIfAircraftNotTracked)
        {
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginFlightplan =
                std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(*pluginFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginFlightplan));

            this->handler.AddOutgoingRelease("BAW123", { 2, "ABTUM" });
            this->handler.ReleaseTypeSelected(1, "test1", {});
            EXPECT_EQ(2, this->handler.GetOutgoingRelease("BAW123").releaseType);
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItErasesOutgoingReleaseIfNoneSelected)
        {
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginFlightplan =
                std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(*pluginFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginFlightplan));

            this->handler.AddOutgoingRelease("BAW123", { 2, "" });
            this->handler.ReleaseTypeSelected(1, "NONE", {});
            EXPECT_EQ(this->handler.invalidRelease, this->handler.GetOutgoingRelease("BAW123"));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItUpdatesOutgoingReleaseIfAlreadyExists)
        {
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginFlightplan =
                std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(*pluginFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginFlightplan));

            this->handler.AddOutgoingRelease("BAW123", { 2, "ARNUN" });
            this->handler.ReleaseTypeSelected(1, "test1", {});
            EXPECT_EQ(1, this->handler.GetOutgoingRelease("BAW123").releaseType);
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItCreatesOutgoingReleaseWhenReleaseTypeSelected)
        {
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginFlightplan =
                std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(*pluginFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginFlightplan));

            this->handler.ReleaseTypeSelected(1, "test1", {});
            EXPECT_EQ(1, this->handler.GetOutgoingRelease("BAW123").releaseType);
            EXPECT_EQ("", this->handler.GetOutgoingRelease("BAW123").releasePoint);
            EXPECT_EQ(
                (std::chrono::system_clock::time_point::max)(),
                this->handler.GetOutgoingRelease("BAW123").clearTime
            );
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItTriggersTheReleasePointEditWithCurrentValue)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            EXPECT_CALL(this->plugin, ShowTextEditPopup(RectEq(RECT{ 0, 0, 80, 25 }), 102, "ARNUN"))
                .Times(1);

            this->handler.AddOutgoingRelease("BAW123", { 2, "ARNUN" });

            this->handler.DisplayReleasePointEditBox(this->flightplan, this->radarTarget, "", { 0, 0 });
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDoesntTriggerTheReleasePointEditIfNoOutgoingRelease)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            EXPECT_CALL(this->plugin, ShowTextEditPopup(_, _, _))
                .Times(0);

            this->handler.DisplayReleasePointEditBox(this->flightplan, this->radarTarget, "", { 0, 0 });
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDoesNotAddReleasePointIfAircraftNotOnline)
        {
            EXPECT_CALL(this->plugin, GetSelectedFlightplan())
                .Times(1)
                .WillOnce(Return(nullptr));

            this->handler.AddOutgoingRelease("BAW123", { 2, "ABTUM" });
            this->handler.EditReleasePoint(1, "ARNUN", {});
            EXPECT_EQ("ABTUM", this->handler.GetOutgoingRelease("BAW123").releasePoint);
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDoesNotAddReleasePointIfAircraftNotTracked)
        {
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginFlightplan =
                std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(*pluginFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginFlightplan));

            this->handler.AddOutgoingRelease("BAW123", { 2, "ABTUM" });
            this->handler.EditReleasePoint(1, "ARNUN", {});
            EXPECT_EQ("ABTUM", this->handler.GetOutgoingRelease("BAW123").releasePoint);
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItSetsOutgoingReleasePoint)
        {
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginFlightplan =
                std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(*pluginFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginFlightplan));

            this->handler.AddOutgoingRelease("BAW123", { 2, "ABTUM" });
            this->handler.EditReleasePoint(1, "ARNUN", {});
            EXPECT_EQ("ARNUN", this->handler.GetOutgoingRelease("BAW123").releasePoint);
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDoesntCreateANewReleaseForReleasePointOnly)
        {
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginFlightplan =
                std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(*pluginFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginFlightplan));

            this->handler.EditReleasePoint(1, "ARNUN", {});
            EXPECT_EQ(this->handler.invalidRelease, this->handler.GetOutgoingRelease("BAW123"));
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItTruncatesOutgoingReleasePointIfTooLong)
        {
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginFlightplan =
                std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(*pluginFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginFlightplan));

            this->handler.AddOutgoingRelease("BAW123", { 2, "ABTUM" });
            this->handler.EditReleasePoint(1, "1234567890123456", {});
            EXPECT_EQ("123456789012345", this->handler.GetOutgoingRelease("BAW123").releasePoint);
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDoesntDoTheReleaseIfTheTransferringControllerIsNotUser)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->transferringController, IsCurrentUser())
                .WillByDefault(Return(false));

            EXPECT_CALL(this->mockApi, SendEnrouteReleaseWithReleasePoint(_, _, _, _, _))
                .Times(0);

            EXPECT_CALL(this->mockApi, SendEnrouteRelease(_, _, _, _))
                .Times(0);

            this->handler.AddOutgoingRelease("BAW123", { 2, "ABTUM" });
            this->handler.HandoffInitiated(this->flightplan, this->transferringController, this->targetController);
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItDoesntDoTheReleaseIfThereIsNoOutgoingRelease)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->transferringController, IsCurrentUser())
                .WillByDefault(Return(true));

            EXPECT_CALL(this->mockApi, SendEnrouteReleaseWithReleasePoint(_, _, _, _, _))
                .Times(0);

            EXPECT_CALL(this->mockApi, SendEnrouteRelease(_, _, _, _))
                .Times(0);

            this->handler.HandoffInitiated(this->flightplan, this->transferringController, this->targetController);
        }


        TEST_F(EnrouteReleaseEventHandlerTest, ItHandlesExceptionsWhenSendingReleases)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->transferringController, GetCallsign())
                .WillByDefault(Return("LON_S_CTR"));

            ON_CALL(this->transferringController, IsCurrentUser())
                .WillByDefault(Return(true));

            ON_CALL(this->targetController, GetCallsign())
                .WillByDefault(Return("LON_C_CTR"));

            EXPECT_CALL(this->mockApi, SendEnrouteReleaseWithReleasePoint(_, _, _, _, _))
                .Times(1)
                .WillOnce(Throw(ApiException("test")));

            EXPECT_CALL(this->mockApi, SendEnrouteRelease(_, _, _, _))
                .Times(0);

            this->handler.AddOutgoingRelease("BAW123", { 2, "ABTUM" });

            EXPECT_NO_THROW(
                this->handler.HandoffInitiated(this->flightplan, this->transferringController, this->targetController)
            );
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItSendsReleaseWithReleasePoint)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->transferringController, GetCallsign())
                .WillByDefault(Return("LON_S_CTR"));

            ON_CALL(this->transferringController, IsCurrentUser())
                .WillByDefault(Return(true));

            ON_CALL(this->targetController, GetCallsign())
                .WillByDefault(Return("LON_C_CTR"));

            EXPECT_CALL(
                this->mockApi,
                SendEnrouteReleaseWithReleasePoint("BAW123", "LON_S_CTR", "LON_C_CTR", 2, "ABTUM")
            )
                .Times(1);

            EXPECT_CALL(this->mockApi, SendEnrouteRelease(_, _, _, _))
                .Times(0);

            this->handler.AddOutgoingRelease("BAW123", { 2, "ABTUM" });

            this->handler.HandoffInitiated(this->flightplan, this->transferringController, this->targetController);

            const EnrouteRelease& release = this->handler.GetOutgoingRelease("BAW123");
            std::chrono::minutes expectedClearDelta = std::chrono::duration_cast<std::chrono::minutes>(
                release.clearTime - std::chrono::system_clock::now() + std::chrono::seconds(10)
            );
            EXPECT_EQ(std::chrono::minutes(3), expectedClearDelta);
        }

        TEST_F(EnrouteReleaseEventHandlerTest, ItSendsReleaseWithoutReleasePoint)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->transferringController, GetCallsign())
                .WillByDefault(Return("LON_S_CTR"));

            ON_CALL(this->transferringController, IsCurrentUser())
                .WillByDefault(Return(true));

            ON_CALL(this->targetController, GetCallsign())
                .WillByDefault(Return("LON_C_CTR"));

            EXPECT_CALL(this->mockApi, SendEnrouteReleaseWithReleasePoint(_, _, _, _, _))
                .Times(0);

            EXPECT_CALL(this->mockApi, SendEnrouteRelease("BAW123", "LON_S_CTR", "LON_C_CTR", 2))
                .Times(1);

            this->handler.AddOutgoingRelease("BAW123", { 2, "" });
            this->handler.HandoffInitiated(this->flightplan, this->transferringController, this->targetController);

            const EnrouteRelease& release = this->handler.GetOutgoingRelease("BAW123");
            std::chrono::minutes expectedClearDelta = std::chrono::duration_cast<std::chrono::minutes>(
                release.clearTime - std::chrono::system_clock::now() + std::chrono::seconds(10)
                );
            EXPECT_EQ(std::chrono::minutes(3), expectedClearDelta);
        }
    }  // namespace Releases
}  // namespace UKControllerPluginTest
