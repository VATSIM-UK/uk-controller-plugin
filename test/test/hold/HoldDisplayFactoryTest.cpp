#include "pch/pch.h"
#include "hold/HoldDisplayFactory.h"
#include "hold/HoldManager.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "hold/HoldingData.h"
#include "hold/HoldDisplay.h"
#include "navaids/NavaidCollection.h"
#include "navaids/Navaid.h"
#include "hold/PublishedHoldCollection.h"
#include "mock/MockDialogProvider.h"
#include "dialog/DialogManager.h"
#include "mock/MockApiInterface.h"
#include "mock/MockTaskRunnerInterface.h"

using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldDisplay;
using UKControllerPlugin::Hold::PublishedHoldCollection;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Navaids::Navaid;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldDisplayFactoryTest : public Test
        {
            public:

                HoldDisplayFactoryTest()
                    : factory(mockPlugin, holdManager, navaids, holds, dialogManager),
                    navaid({ 1, "TIMBA", EuroScopePlugIn::CPosition()}),
                    dialogManager(dialogProvider), holdManager(mockApi, taskRunner)
                {
                    this->navaids.AddNavaid(navaid);
                    this->holds.Add(std::move(this->holdData));
                }

                NiceMock<MockTaskRunnerInterface> taskRunner;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockDialogProvider> dialogProvider;
                DialogManager dialogManager;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                HoldingData holdData = { 1, "TIMBA", "TIMBA TEST", 7000, 15000, 360, "left", {} };
                PublishedHoldCollection holds;
                Navaid navaid;
                NavaidCollection navaids;
                HoldManager holdManager;
                HoldDisplayFactory factory;
        };

        TEST_F(HoldDisplayFactoryTest, ItReturnsNullPointerIfNavaidNotFound)
        {
            EXPECT_EQ(nullptr, this->factory.Create("WILLO"));
        }

        TEST_F(HoldDisplayFactoryTest, ItReturnsAHoldDisplayIfNavaidFound)
        {
            std::unique_ptr<HoldDisplay> display = this->factory.Create("TIMBA");
            EXPECT_EQ(this->navaid, display->navaid);
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
