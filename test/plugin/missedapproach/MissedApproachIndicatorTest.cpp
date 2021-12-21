#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachCollection.h"
#include "missedapproach/MissedApproachIndicator.h"
#include "tag/TagData.h"
#include "time/SystemClock.h"

using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::MissedApproach::MissedApproachCollection;
using UKControllerPlugin::MissedApproach::MissedApproachIndicator;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachIndicatorTest : public testing::Test
    {
        public:
        MissedApproachIndicatorTest() : indicator(collection)
        {
        }

        UKControllerPlugin::Tag::TagData GetTagData()
        {
            return UKControllerPlugin::Tag::TagData(
                this->mockFlightplan,
                this->mockRadarTarget,
                130,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                this->itemString,
                &this->euroscopeColourCode,
                &this->tagColour,
                &this->fontSize);
        }

        double fontSize = 24.1;
        COLORREF tagColour = RGB(255, 255, 255);
        int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
        char itemString[16] = "";
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> mockRadarTarget;
        MissedApproachCollection collection;
        MissedApproachIndicator indicator;
    };

    TEST_F(MissedApproachIndicatorTest, ItHasATagItemDescription)
    {
        EXPECT_EQ("Missed Approach Indicator", indicator.GetTagItemDescription(130));
    }

    TEST_F(MissedApproachIndicatorTest, ItSetsTheTagItemData)
    {
        ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

        auto missed = std::make_shared<class MissedApproach>(1, "BAW123", TimeNow() + std::chrono::minutes(2), true);
        missed->Acknowledge("a", "b");
        collection.Add(missed);

        auto tagData = GetTagData();
        indicator.SetTagItemData(tagData);

        EXPECT_EQ("GO AROUND", tagData.GetItemString());
    }

    TEST_F(MissedApproachIndicatorTest, ItDoesntSetDataForExpiredApproaches)
    {
        ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

        auto missed = std::make_shared<class MissedApproach>(1, "BAW123", TimeNow() - std::chrono::minutes(2), true);
        missed->Acknowledge("a", "b");
        collection.Add(missed);

        auto tagData = GetTagData();
        indicator.SetTagItemData(tagData);

        EXPECT_TRUE(tagData.GetItemString().empty());
    }

    TEST_F(MissedApproachIndicatorTest, ItDoesntSetDataForNonAcknowledgedApproaches)
    {
        ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

        auto missed = std::make_shared<class MissedApproach>(1, "BAW123", TimeNow() + std::chrono::minutes(2), true);
        collection.Add(missed);

        auto tagData = GetTagData();
        indicator.SetTagItemData(tagData);

        EXPECT_TRUE(tagData.GetItemString().empty());
    }

    TEST_F(MissedApproachIndicatorTest, ItDoesntSetDataForWrongCallsign)
    {
        ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

        auto missed = std::make_shared<class MissedApproach>(1, "BAW456", TimeNow() + std::chrono::minutes(2), true);
        missed->Acknowledge("a", "b");
        collection.Add(missed);

        auto tagData = GetTagData();
        indicator.SetTagItemData(tagData);

        EXPECT_TRUE(tagData.GetItemString().empty());
    }
} // namespace UKControllerPluginTest::MissedApproach
