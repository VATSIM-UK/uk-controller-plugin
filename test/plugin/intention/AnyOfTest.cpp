#include "intention/AnyOf.h"

using UKControllerPlugin::IntentionCode::AnyOf;
using UKControllerPlugin::IntentionCode::Condition;

namespace UKControllerPluginTest::IntentionCode {
    class AnyOfTest : public testing::Test
    {
        public:
        AnyOfTest()
            : condition1(std::make_shared<testing::NiceMock<MockCondition>>()),
              condition2(std::make_shared<testing::NiceMock<MockCondition>>()),
              anyOf(std::list<std::shared_ptr<Condition>>{condition1, condition2})
        {
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        std::shared_ptr<testing::NiceMock<MockCondition>> condition1;
        std::shared_ptr<testing::NiceMock<MockCondition>> condition2;
        AnyOf anyOf;
    };

    TEST_F(AnyOfTest, ItReturnsFalseIfNoConditions)
    {
        EXPECT_FALSE((AnyOf({})).Passes(flightplan, radarTarget));
    }

    TEST_F(AnyOfTest, ItReturnsTrueIfASingleConditionPasses)
    {
        EXPECT_CALL(*condition1, Passes(testing::Ref(flightplan), testing::Ref(radarTarget)))
            .Times(1)
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*condition2, Passes(testing::Ref(flightplan), testing::Ref(radarTarget)))
            .Times(1)
            .WillOnce(testing::Return(true));

        EXPECT_TRUE(anyOf.Passes(flightplan, radarTarget));
    }

    TEST_F(AnyOfTest, ItReturnsFalseIfNoConditionsPass)
    {
        EXPECT_CALL(*condition1, Passes(testing::Ref(flightplan), testing::Ref(radarTarget)))
            .Times(1)
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*condition2, Passes(testing::Ref(flightplan), testing::Ref(radarTarget)))
            .Times(1)
            .WillOnce(testing::Return(false));

        EXPECT_FALSE(anyOf.Passes(flightplan, radarTarget));
    }
} // namespace UKControllerPluginTest::IntentionCode
