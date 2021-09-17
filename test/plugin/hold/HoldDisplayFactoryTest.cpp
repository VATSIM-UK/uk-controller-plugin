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

using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Hold::HoldDisplay;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::PublishedHoldCollection;
using UKControllerPlugin::Navaids::Navaid;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest::Hold {

    class HoldDisplayFactoryTest : public Test
    {
        public:
        HoldDisplayFactoryTest()
            : dialogManager(dialogProvider), navaid({1, "TIMBA", EuroScopePlugIn::CPosition()}),
              holdManager(mockApi, taskRunner), factory(mockPlugin, holdManager, navaids, holds, dialogManager)

        {
            this->navaids.AddNavaid(navaid);
            this->holds.Add(std::move(this->holdData));
        }

        NiceMock<MockTaskRunnerInterface> taskRunner;
        NiceMock<MockApiInterface> mockApi;
        NiceMock<MockDialogProvider> dialogProvider;
        DialogManager dialogManager;
        NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
        HoldingData holdData = {1, "TIMBA", "TIMBA TEST", 7000, 15000, 360, "left", {}};
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
} // namespace UKControllerPluginTest::Hold
