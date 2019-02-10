#include "pch/pch.h"
#include "hold/HoldManager.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "hold/ManagedHold.h"
#include "hold/HoldingData.h"

using UKControllerPlugin::Hold::HoldManager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Hold::ManagedHold;
using UKControllerPlugin::Hold::HoldingData;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldManagerTest : public Test
        {
            public:
                HoldManagerTest(void)
                {
                    manager.AddHold(hold1);

                    ON_CALL(mockFlightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                }

                const UKControllerPlugin::Hold::HoldingData hold1 = { 1, "WILLO", "WILLO", 8000, 15000, 209, 0 };
                NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
                NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                HoldManager manager;
        };


        TEST_F(HoldManagerTest, ItStartsEmpty)
        {
            EXPECT_EQ(1, this->manager.CountHolds());
        }

        TEST_F(HoldManagerTest, ItAddsAHold)
        {
            HoldingData hold2 = { 2, "TIMBA", "TIMBA", 8000, 15000, 209, 0 };
            this->manager.AddHold(hold2);
            EXPECT_EQ(2, this->manager.CountHolds());
        }

        TEST_F(HoldManagerTest, ItHandlesDuplicateHolds)
        {
            this->manager.AddHold(hold1);
            EXPECT_EQ(1, this->manager.CountHolds());
        }

        TEST_F(HoldManagerTest, ItAddsAircraftToHold)
        {
            this->manager.AddAircraftToHold(mockFlightplan, mockRadarTarget, 1);
            EXPECT_TRUE(this->manager.GetManagedHold(1)->HasAircraft("BAW123"));
            EXPECT_TRUE(1, this->manager.GetManagedHold(1)->CountHoldingAircraft());
        }

        TEST_F(HoldManagerTest, AddingAircraftToOneHoldRemovesFromAnother)
        {
            HoldingData hold2 = { 2, "TIMBA", "TIMBA", 8000, 15000, 209, 0 };
            this->manager.AddHold(hold2);
            this->manager.AddAircraftToHold(mockFlightplan, mockRadarTarget, 1);
            EXPECT_TRUE(this->manager.GetManagedHold(1)->HasAircraft("BAW123"));
            this->manager.AddAircraftToHold(mockFlightplan, mockRadarTarget, 2);
            EXPECT_FALSE(this->manager.GetManagedHold(1)->HasAircraft("BAW123"));
            EXPECT_TRUE(this->manager.GetManagedHold(2)->HasAircraft("BAW123"));
        }

        TEST_F(HoldManagerTest, ItHandlesAddingToNonExistantHold)
        {
            EXPECT_NO_THROW(this->manager.AddAircraftToHold(mockFlightplan, mockRadarTarget, 555));
        }

        TEST_F(HoldManagerTest, GetHoldReturnsNullPointerIfDoesntExist)
        {
            EXPECT_EQ(nullptr, this->manager.GetManagedHold(555));
        }

        TEST_F(HoldManagerTest, ItRemovesAnAircraftFromAnyHold)
        {
            HoldingData hold2 = { 2, "TIMBA", "TIMBA", 8000, 15000, 209, 0 };
            this->manager.AddHold(hold2);
            manager.AddAircraftToHold(mockFlightplan, mockRadarTarget, 1);
            EXPECT_TRUE(this->manager.GetManagedHold(1)->HasAircraft("BAW123"));
            manager.RemoveAircraftFromAnyHold("BAW123");
            EXPECT_FALSE(this->manager.GetManagedHold(1)->HasAircraft("BAW123"));
        }

        TEST_F(HoldManagerTest, ItHandlesRemovingNonExistantAircraft)
        {
            EXPECT_NO_THROW(manager.RemoveAircraftFromAnyHold("BAW123"));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
