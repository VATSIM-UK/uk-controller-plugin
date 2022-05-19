#include "intention/SectorExitPointEtrat.h"

using UKControllerPlugin::IntentionCode::SectorExitPointEtrat;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;

namespace UKControllerPluginTest::IntentionCode {

    class SectorExitPointEtratTest : public testing::Test
    {
        public:
        SectorExitPointEtratTest() : exitPoint("ETRAT", "E", SectorExitPointEtrat::outSouth)
        {
            ON_CALL(routeMock, GetPointsNumber()).WillByDefault(testing::Return(1));
            ON_CALL(routeMock, GetPointName(0)).WillByDefault(testing::Return("EDDF"));
        }

        testing::NiceMock<MockEuroscopeExtractedRouteInterface> routeMock;
        SectorExitPointEtrat exitPoint;
    };

    TEST_F(SectorExitPointEtratTest, ItAppliesToAllControllers)
    {
        EXPECT_TRUE(exitPoint.AppliesToController("BLA"));
    }

    TEST_F(SectorExitPointEtratTest, GetIntentionCodeReturnsCorrectSecondaryCode)
    {
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 25000).compare("E2"));
    }

    TEST_F(SectorExitPointEtratTest, GetIntentionCodeReturnsCorrectSecondaryBoundary)
    {
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 29000).compare("E2"));
    }

    TEST_F(SectorExitPointEtratTest, GetIntentionCodeReturnsCorrectPrimaryCodeBoundary)
    {
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 29001).compare("E"));
    }

    TEST_F(SectorExitPointEtratTest, GetIntentionCodeReturnsCorrectPrimaryCode)
    {
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("E"));
    }

    TEST_F(SectorExitPointEtratTest, GetIntentionCodeReturnsCorrectPrimaryCodeNoPointsInRoute)
    {
        ON_CALL(routeMock, GetPointsNumber()).WillByDefault(testing::Return(0));
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("E"));
    }

    TEST_F(SectorExitPointEtratTest, GetIntentionCodeReturnsCorrectCodeForDeauville)
    {
        ON_CALL(routeMock, GetPointName(0)).WillByDefault(testing::Return("LFRG"));
        EXPECT_EQ("E3", exitPoint.GetIntentionCode(routeMock, 0, 25000));
    }

    TEST_F(SectorExitPointEtratTest, GetIntentionCodeReturnsCorrectCodeForCaen)
    {
        ON_CALL(routeMock, GetPointName(0)).WillByDefault(testing::Return("LFRK"));
        EXPECT_EQ("E3", exitPoint.GetIntentionCode(routeMock, 0, 25000));
    }

    TEST_F(SectorExitPointEtratTest, GetIntentionCodeReturnsCorrectCodeForLeHavre)
    {
        ON_CALL(routeMock, GetPointName(0)).WillByDefault(testing::Return("LFOH"));
        EXPECT_EQ("E3", exitPoint.GetIntentionCode(routeMock, 0, 25000));
    }

    TEST_F(SectorExitPointEtratTest, GetIntentionCodeReturnsCorrectCodeForEvreux)
    {
        ON_CALL(routeMock, GetPointName(0)).WillByDefault(testing::Return("LFOE"));
        EXPECT_EQ("E3", exitPoint.GetIntentionCode(routeMock, 0, 25000));
    }
} // namespace UKControllerPluginTest::IntentionCode
