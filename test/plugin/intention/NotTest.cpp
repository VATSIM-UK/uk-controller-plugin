//
// Created by Andy on 30/12/2022.
//
#include "intention/Not.h"

using UKControllerPlugin::IntentionCode::Condition;
using UKControllerPlugin::IntentionCode::Not;

namespace UKControllerPluginTest::IntentionCode {
    class NotTest : public testing::Test
    {
        public:
        NotTest() : condition(std::make_shared<testing::NiceMock<MockCondition>>()), notCondition(condition)
        {
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        std::shared_ptr<testing::NiceMock<MockCondition>> condition;
        Not notCondition;
    };

    TEST_F(NotTest, ItFailsIfRulePasses)
    {
        EXPECT_CALL(*condition, Passes(testing::Ref(flightplan), testing::Ref(radarTarget)))
            .Times(1)
            .WillOnce(testing::Return(true));

        EXPECT_FALSE(notCondition.Passes(flightplan, radarTarget));
    }

    TEST_F(NotTest, ItPassesIfRuleFails)
    {
        EXPECT_CALL(*condition, Passes(testing::Ref(flightplan), testing::Ref(radarTarget)))
            .Times(1)
            .WillOnce(testing::Return(false));

        EXPECT_TRUE(notCondition.Passes(flightplan, radarTarget));
    }
} // namespace UKControllerPluginTest::IntentionCode
