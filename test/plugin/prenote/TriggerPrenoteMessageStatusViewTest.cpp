#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "prenote/PrenoteMessageStatusView.h"
#include "prenote/TriggerPrenoteMessageStatusView.h"
#include "time/SystemClock.h"

using testing::NiceMock;
using UKControllerPlugin::Prenote::PrenoteMessageStatusView;
using UKControllerPlugin::Prenote::TriggerPrenoteMessageStatusView;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;

namespace UKControllerPluginTest::Prenote {
    class TriggerPrenoteMessageStatusViewTest : public ::testing::Test
    {
        public:
        TriggerPrenoteMessageStatusViewTest()
        {
            SetTestNow(std::chrono::system_clock::now());
            ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));
        }
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        TriggerPrenoteMessageStatusView trigger;
    };

    TEST_F(TriggerPrenoteMessageStatusViewTest, ItTriggersTheView)
    {
        TriggerPrenoteMessageStatusView::Trigger(mockFlightplan, POINT{150, 200});
        EXPECT_EQ(150, PrenoteMessageStatusView::GetDisplayPosition().x);
        EXPECT_EQ(200, PrenoteMessageStatusView::GetDisplayPosition().y);
        EXPECT_EQ("BAW123", PrenoteMessageStatusView::GetDisplayingFor());
        EXPECT_EQ(TimeNow() + std::chrono::seconds(3), PrenoteMessageStatusView::GetDisplayUntil());
        EXPECT_TRUE(PrenoteMessageStatusView::ShouldDisplay());
    }
} // namespace UKControllerPluginTest::Prenote
