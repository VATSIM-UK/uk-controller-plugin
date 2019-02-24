#include "pch/pch.h"
#include "hold/HoldConfigurationDialogFactory.h"
#include "hold/HoldingData.h"
#include "hold/HoldConfigurationDialog.h"
#include "hold/HoldManager.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "hold/HoldWindowManager.h"

using UKControllerPlugin::Hold::CreateHoldConfigurationDialog;
using UKControllerPlugin::Hold::HoldConfigurationDialog;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldWindowManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldConfigurationDialogFactoryTest : public Test
        {
            public:
                HoldConfigurationDialogFactoryTest()
                    : windowManager(NULL, NULL, holdmanager, mockPlugin)
                {

                }

            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
            HoldManager holdmanager;
            HoldWindowManager windowManager;
        };

        TEST_F(HoldConfigurationDialogFactoryTest, ItReturnsEmptyIfDataNotObject)
        {
            nlohmann::json data = "[]"_json;
            EXPECT_EQ(0, CreateHoldConfigurationDialog(data, this->windowManager)->CountHolds());
        }

        TEST_F(HoldConfigurationDialogFactoryTest, ItAddsHoldsWithData)
        {
            nlohmann::json data;
            nlohmann::json hold1;
            hold1["id"] = 1;
            hold1["description"] = "TIMBA";
            hold1["fix"] = "TIMBA";
            hold1["minimum_altitude"] = 7000;
            hold1["maximum_altitude"] = 15000;
            hold1["inbound_heading"] = 309;
            hold1["turn_direction"] = "right";
            nlohmann::json hold2;
            hold2["id"] = 2;
            hold2["description"] = "WILLO";
            hold2["fix"] = "WILLO";
            hold2["minimum_altitude"] = 7000;
            hold2["maximum_altitude"] = 15000;
            hold2["inbound_heading"] = 285;
            hold2["turn_direction"] = "left";

            data = { hold1, hold2 };
            EXPECT_EQ(2, CreateHoldConfigurationDialog(data, this->windowManager)->CountHolds());
        }

        TEST_F(HoldConfigurationDialogFactoryTest, ItDoesntAddNonObjects)
        {
            nlohmann::json data;
            data = { "Test" };

            EXPECT_EQ(0, CreateHoldConfigurationDialog(data, this->windowManager)->CountHolds());
        }

        TEST_F(HoldConfigurationDialogFactoryTest, ItDoesntAddInvalidData)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["id"] = 1;
            hold["description"] = "TIMBA";
            hold["fix"] = "TIMBA Description";
            hold["maximum_altitude"] = 15000;
            hold["inbound_heading"] = 309;
            hold["turn_direction"] = "left";

            data = { hold };

            EXPECT_EQ(0, CreateHoldConfigurationDialog(data, this->windowManager)->CountHolds());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
