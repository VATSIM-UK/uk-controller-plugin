#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachCollection.h"
#include "missedapproach/TriggerMissedApproach.h"
#include "mock/MockApiInterface.h"
#include "mock/MockWinApi.h"
#include "time/ParseTimeStrings.h"
#include "time/SystemClock.h"

using ::testing::NiceMock;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::MissedApproach::MissedApproachCollection;
using UKControllerPlugin::MissedApproach::TriggerMissedApproach;
using UKControllerPlugin::Time::ParseTimeString;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest::MissedApproach {
    class TriggerMissedApproachTest : public testing::Test
    {
        public:
        TriggerMissedApproachTest()
            : missed1(std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now())),
              missed2(std::make_shared<class MissedApproach>(2, "BAW456", std::chrono::system_clock::now())),
              collection(std::make_shared<MissedApproachCollection>()), trigger(collection, windows, api, activeCallsigns)
        {
            SetTestNow(ParseTimeString("2021-08-23 13:55:00"));
        }
        
        ActiveCallsignCollection activeCallsigns;
        NiceMock<MockWinApi> windows;
        NiceMock<MockApiInterface> api;
        std::shared_ptr<class MissedApproach> missed1;
        std::shared_ptr<class MissedApproach> missed2;
        std::shared_ptr<MissedApproachCollection> collection;
        TriggerMissedApproach trigger;
    };
    
    TEST_F(TriggerMissedApproachTest, ItTriggersAMissedApproach)
    {
    
    }
} // namespace UKControllerPluginTest::MissedApproach
