#include "pch/pch.h"
#include "hold/HoldManager.h"
#include "hold/HoldEventHandler.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "hold/HoldWindowManager.h"
#include "plugin/PopupMenuItem.h"
#include "hold/ManagedHold.h"
#include "hold/HoldingData.h"

using UKControllerPlugin::Hold::ManagedHold;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Hold::HoldEventHandler;
using UKControllerPlugin::Hold::HoldWindowManager;
using UKControllerPlugin::Plugin::PopupMenuItem;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldEventHandlerTest : public Test
        {
            public:
                HoldEventHandlerTest(void)
                    : handler(this->manager, this->mockPlugin, HoldWindowManager(NULL, NULL), 1)
                {
                    manager.AddHold(ManagedHold(holdData));

                    // Add a FP to the holds initially.
                    NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplanInitial;
                    NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTargetInitial;

                    ON_CALL(mockFlightplanInitial, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    ON_CALL(mockFlightplanInitial, GetClearedAltitude())
                        .WillByDefault(Return(8000));

                    ON_CALL(mockRadarTargetInitial, GetFlightLevel())
                        .WillByDefault(Return(9000));

                    manager.AddAircraftToHold(mockFlightplanInitial, mockRadarTargetInitial, 1);


                    this->mockFlightplan.reset(new NiceMock<MockEuroScopeCFlightPlanInterface>);
                    this->mockRadarTarget.reset(new NiceMock<MockEuroScopeCRadarTargetInterface>);

                    // On subsequent calls, we use this to update the fp
                    ON_CALL(*this->mockFlightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    ON_CALL(*this->mockFlightplan, GetClearedAltitude())
                        .WillByDefault(Return(7000));

                    ON_CALL(*this->mockRadarTarget, GetFlightLevel())
                        .WillByDefault(Return(8000));

                    ON_CALL(mockPlugin, GetFlightplanForCallsign("BAW123"))
                        .WillByDefault(Return(this->mockFlightplan));

                    ON_CALL(mockPlugin, GetRadarTargetForCallsign("BAW123"))
                        .WillByDefault(Return(this->mockRadarTarget));
                }

                const UKControllerPlugin::Hold::HoldingData holdData = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, 0 };
                std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplan;
                std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                HoldManager manager;
                HoldEventHandler handler;
        };

        TEST_F(HoldEventHandlerTest, ItRemovesAnAircraftFromTheHoldIfTheFlightplanDisconnects)
        {
            EXPECT_TRUE(manager.GetManagedHold(1)->HasAircraft("BAW123"));
            this->handler.FlightPlanDisconnectEvent(*this->mockFlightplan);
            EXPECT_FALSE(manager.GetManagedHold(1)->HasAircraft("BAW123"));
        }

        TEST_F(HoldEventHandlerTest, ItCanBeConfiguredFromTheMenu)
        {
            PopupMenuItem expected;
            expected.callbackFunctionId = this->handler.popupMenuItemId;
            expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            expected.disabled = false;
            expected.firstValue = this->handler.menuItemDescription;
            expected.secondValue = "";
            expected.fixedPosition = false;
            EXPECT_TRUE(expected == this->handler.GetConfigurationMenuItem());
        }

        TEST_F(HoldEventHandlerTest, ItRejectsUnknownCommands)
        {
            EXPECT_FALSE(this->handler.ProcessCommand("NOPE"));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
