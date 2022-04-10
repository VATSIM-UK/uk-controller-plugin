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
        EXPECT_CALL(routeMock, GetPointsNumber()).Times(1).WillRepeatedly(Return(2));
        EXPECT_CALL(routeMock, GetPointName(0)).Times(3).WillRepeatedly(Return("LELNA"));
        EXPECT_CALL(routeMock, GetPointName(1)).Times(4).WillRepeatedly(Return("LFMN"));
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H8"));
    }

    TEST_F(SectorExitPointLelnaTest, GetIntentionCodeReturnsCorrectSecondaryCodeArree)
    {
        EXPECT_CALL(routeMock, GetPointsNumber()).Times(1).WillRepeatedly(Return(3));
        EXPECT_CALL(routeMock, GetPointName(0)).Times(3).WillRepeatedly(Return("LELNA"));
        EXPECT_CALL(routeMock, GetPointName(1)).Times(3).WillRepeatedly(Return("ARE"));
        EXPECT_CALL(routeMock, GetPointName(2)).Times(1).WillRepeatedly(Return("LEMD"));
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H3"));
    }

    TEST_F(SectorExitPointLelnaTest, GetIntentionCodeReturnsCorrectSecondaryCodeDomok)
    {
        EXPECT_CALL(routeMock, GetPointsNumber()).Times(1).WillRepeatedly(Return(3));
        EXPECT_CALL(routeMock, GetPointName(0)).Times(3).WillRepeatedly(Return("LELNA"));
        EXPECT_CALL(routeMock, GetPointName(1)).Times(2).WillRepeatedly(Return("DOMOK"));
        EXPECT_CALL(routeMock, GetPointName(2)).Times(1).WillRepeatedly(Return("LEMD"));
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H3"));
    }

    TEST_F(SectorExitPointLelnaTest, GetIntentionCodeReturnsCorrectIntentionCodeForBrestFirArrivals)
    {
        EXPECT_CALL(routeMock, GetPointsNumber()).Times(1).WillRepeatedly(Return(3));
        EXPECT_CALL(routeMock, GetPointName(2)).Times(1).WillRepeatedly(Return("LFRO"));
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H5"));
    }

    TEST_F(SectorExitPointLelnaTest, GetIntentionCodeReturnsCorrectIntentionCodeForCherbourgArrivals)
    {
        EXPECT_CALL(routeMock, GetPointsNumber()).Times(1).WillRepeatedly(Return(3));
        EXPECT_CALL(routeMock, GetPointName(2)).Times(1).WillRepeatedly(Return("LFRC"));
        EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 25000).compare("JC"));
    }
} // namespace UKControllerPluginTest::IntentionCode
