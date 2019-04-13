#include "pch/pch.h"
#include "hold/HoldDisplayFactory.h"
#include "hold/HoldManager.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "hold/ManagedHold.h"
#include "hold/HoldingData.h"
#include "hold/HoldDisplay.h"

using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Hold::ManagedHold;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldDisplay;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldDisplayFactoryTest : public Test
        {
            public:

                HoldDisplayFactoryTest()
                    : factory(mockPlugin, holdManager), 
                    managedHold({ 1, "TIMBA", "TIMBA TEST", 7000, 15000, 360, "left", {} })
                {
                    this->holdManager.AddHold(std::move(managedHold));
                }

                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                HoldManager holdManager;
                HoldDisplayFactory factory;
                HoldingData holdData = {1, "TIMBA", "TIMBA TEST", 7000, 15000, 360, "left", {}};
                ManagedHold managedHold;
        };

        TEST_F(HoldDisplayFactoryTest, ItReturnsNullPointerIfHoldNotFound)
        {
            EXPECT_EQ(nullptr, this->factory.Create(55));
        }

        TEST_F(HoldDisplayFactoryTest, ItReturnsAHoldDisplayIfHoldFound)
        {
            std::unique_ptr<HoldDisplay> display = this->factory.Create(1);
            EXPECT_EQ(this->holdData, display->managedHold.GetHoldParameters());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
