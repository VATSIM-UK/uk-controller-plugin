#include "intention/SectorExitPointLelna.h"

using ::testing::NiceMock;
using ::testing::Return;
using UKControllerPlugin::IntentionCode::SectorExitPointLelna;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;

namespace UKControllerPluginTest::IntentionCode {

    class SectorExitPointLelnaTest : public testing::Test
    {
        public:
        SectorExitPointLelnaTest() : exitPoint("LELNA", "H8", SectorExitPointLelna::outSouth)
        {
        }

        NiceMock<MockEuroscopeExtractedRouteInterface> routeMock;
        SectorExitPointLelna exitPoint;
    };

    TEST_F(SectorExitPointLelnaTest, ItAppliesToAllControllers)
    {
        EXPECT_TRUE(exitPoint.AppliesToController("BLA"));
    }

    TEST_F(SectorExitPointLelnaTest, GetIntentionCodeReturnsCorrectDefaultCodeLelna)
    {
        ON_CALL(routeMock, GetPointsNumber()).WillByDefault(Return(2));
        ON_CALL(routeMock, GetPointName(0)).WillByDefault(Return("LELNA"));
        ON_CALL(routeMock, GetPointName(1)).WillByDefault(Return("LFMN"));
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H8"));
    }

    TEST_F(SectorExitPointLelnaTest, GetIntentionCodeReturnsCorrectSecondaryCodeArree)
    {
        ON_CALL(routeMock, GetPointsNumber()).WillByDefault(Return(3));
        ON_CALL(routeMock, GetPointName(0)).WillByDefault(Return("LELNA"));
        ON_CALL(routeMock, GetPointName(1)).WillByDefault(Return("ARE"));
        ON_CALL(routeMock, GetPointName(2)).WillByDefault(Return("LEMD"));
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H3"));
    }

    TEST_F(SectorExitPointLelnaTest, GetIntentionCodeReturnsCorrectSecondaryCodeDomok)
    {
        ON_CALL(routeMock, GetPointsNumber()).WillByDefault(Return(3));
        ON_CALL(routeMock, GetPointName(0)).WillByDefault(Return("LELNA"));
        ON_CALL(routeMock, GetPointName(1)).WillByDefault(Return("DOMOK"));
        ON_CALL(routeMock, GetPointName(2)).WillByDefault(Return("LEMD"));
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H3"));
    }

    TEST_F(SectorExitPointLelnaTest, GetIntentionCodeReturnsCorrectIntentionCodeForBrestFirArrivals)
    {
        ON_CALL(routeMock, GetPointsNumber()).WillByDefault(Return(3));
        ON_CALL(routeMock, GetPointName(2)).WillByDefault(Return("LFRO"));
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H5"));
    }

    TEST_F(SectorExitPointLelnaTest, GetIntentionCodeReturnsCorrectIntentionCodeForCherbourgArrivals)
    {
        ON_CALL(routeMock, GetPointsNumber()).WillByDefault(Return(3));
        ON_CALL(routeMock, GetPointName(2)).WillByDefault(Return("LFRC"));
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 25000).compare("JC"));
    }
} // namespace UKControllerPluginTest::IntentionCode
