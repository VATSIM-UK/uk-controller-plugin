#include "intention/AllOf.h"

using UKControllerPlugin::IntentionCode::AllOf;
using UKControllerPlugin::IntentionCode::Condition;

namespace UKControllerPluginTest::IntentionCode {
    class AllOfTest : public testing::Test
    {
        public:
        AllOfTest()
            : condition1(std::make_shared<testing::NiceMock<MockCondition>>()),
              condition2(std::make_shared<testing::NiceMock<MockCondition>>()),
              allOf(std::list<std::shared_ptr<Condition>>{condition1, condition2})
        {
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        std::shared_ptr<testing::NiceMock<MockCondition>> condition1;
        std::shared_ptr<testing::NiceMock<MockCondition>> condition2;
        AllOf allOf;
    };

    TEST_F(AllOfTest, ItReturnsTrueIfNoConditions)
    {
        EXPECT_TRUE((AllOf({})).Passes(flightplan, radarTarget));
    }

    TEST_F(AllOfTest, ItReturnsTrueIfAllConditionsPass)
    {
        EXPECT_CALL(*condition1, Passes(testing::Ref(flightplan), testing::Ref(radarTarget)))
            .Times(1)
            .WillOnce(testing::Return(true));

        EXPECT_CALL(*condition2, Passes(testing::Ref(flightplan), testing::Ref(radarTarget)))
            .Times(1)
            .WillOnce(testing::Return(true));

        EXPECT_TRUE(allOf.Passes(flightplan, radarTarget));
    }

    TEST_F(AllOfTest, ItReturnsFalseIfConditionFails)
    {
        EXPECT_CALL(*condition1, Passes(testing::Ref(flightplan), testing::Ref(radarTarget)))
            .Times(1)
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*condition2, Passes(testing::Ref(flightplan), testing::Ref(radarTarget))).Times(0);

        EXPECT_FALSE(allOf.Passes(flightplan, radarTarget));
    }
} // namespace UKControllerPluginTest::IntentionCode
