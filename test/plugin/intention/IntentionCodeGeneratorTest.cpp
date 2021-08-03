#include "pch/pch.h"
#include "intention/IntentionCodeGenerator.h"
#include "mock/MockAirfieldGroup.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"
#include "intention/IntentionCodeData.h"
#include "intention/SectorExitRepositoryFactory.h"

using UKControllerPlugin::IntentionCode::AirfieldGroup;
using UKControllerPlugin::IntentionCode::IntentionCodeData;
using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using UKControllerPlugin::IntentionCode::SectorExitRepositoryFactory;
using UKControllerPlugin::IntentionCode::SectorExitRepository;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::_;
using EuroScopePlugIn::CPosition;

namespace UKControllerPluginTest {
    namespace IntentionCode {

        class IntentionCodeGeneratorTest : public ::testing::Test
        {
        public:
            void SetUp(void) {

                // Moving North - MATIK
                exitPositionNorth.m_Latitude = 61.000000000000000;
                exitPositionNorth.m_Longitude = -8.0666670000000007;
                nextFixPositonNorth.m_Latitude = 62.106896999999996;
                nextFixPositonNorth.m_Longitude = -7.5876749999999999;

                // Moving South - ORTAC
                exitPositionSouth.m_Latitude = 49.999167000000000;
                exitPositionSouth.m_Longitude = -2.0049999999999999;
                nextFixPositonSouth.m_Latitude = 49.724443999999998;
                nextFixPositonSouth.m_Longitude = -2.1050000000000000;

                // Moving East - KOK
                exitPositionEast.m_Latitude = 51.094722000000004;
                exitPositionEast.m_Longitude = 2.6516669999999998;
                nextFixPositonEast.m_Latitude = 50.912638999999999;
                nextFixPositonEast.m_Longitude = 3.6369719999999996;

                // Moving West - BAGSO
                exitPositionWest.m_Latitude = 53.680000000000000;
                exitPositionWest.m_Longitude = -5.5000000000000000;
                nextFixPositonWest.m_Latitude = 55.000000000000000;
                nextFixPositonWest.m_Longitude = -15.000000000000000;
            }

        protected:

            // Position data for tests
            CPosition exitPositionNorth;
            CPosition nextFixPositonNorth;
            CPosition exitPositionSouth;
            CPosition nextFixPositonSouth;
            CPosition exitPositionEast;
            CPosition nextFixPositonEast;
            CPosition exitPositionWest;
            CPosition nextFixPositonWest;
        };

        TEST_F(IntentionCodeGeneratorTest, StartsWithNoUserControllerPosition)
        {
            std::vector<std::unique_ptr<AirfieldGroup>> groups;
            IntentionCodeGenerator intention(std::move(groups), *SectorExitRepositoryFactory::Create());
            EXPECT_EQ("", intention.GetUserControllerPosition());
        }

        TEST_F(IntentionCodeGeneratorTest, ItSetsUserControllerPosition)
        {
            std::vector<std::unique_ptr<AirfieldGroup>> groups;
            IntentionCodeGenerator intention(std::move(groups), *SectorExitRepositoryFactory::Create());
            intention.SetUserControllerPosition("BLA");
            EXPECT_EQ("BLA", intention.GetUserControllerPosition());
        }

        TEST_F(IntentionCodeGeneratorTest, ReturnsCorrectCodeNoFlightplan)
        {
            std::vector<std::unique_ptr<AirfieldGroup>> groups;
            IntentionCodeGenerator intention(std::move(groups), *SectorExitRepositoryFactory::Create());
            NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
            IntentionCodeData data = intention.GetIntentionCodeForFlightplan("BAW123", "", "", mockFlightPlan, 0);
            EXPECT_TRUE(data.intentionCode == "--");
            EXPECT_FALSE(data.exitPointValid);
            EXPECT_EQ("", data.exitPoint);
            EXPECT_EQ(IntentionCodeGenerator::invalidExitPointIndex, data.exitPointIndex);
        }

        TEST_F(IntentionCodeGeneratorTest, ReturnsCorrectCodeFromAirfieldGroup)
        {
            NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
            std::vector<std::unique_ptr<AirfieldGroup>> groups;
            std::unique_ptr<StrictMock<MockAirfieldGroup>> mockAirfieldGroup(new StrictMock<MockAirfieldGroup>);

            MockAirfieldGroup * mockGroupRaw = mockAirfieldGroup.get();

            groups.push_back(std::move(mockAirfieldGroup));
            IntentionCodeGenerator intention(std::move(groups), *SectorExitRepositoryFactory::Create());
            IntentionCodeData data = intention.GetIntentionCodeForFlightplan(
                "BAW123",
                "OMDB",
                "EGLL",
                mockFlightPlan,
                27000
            );
            EXPECT_TRUE(data.intentionCode == "LL");
            EXPECT_FALSE(data.exitPointValid);
            EXPECT_EQ("", data.exitPoint);
            EXPECT_EQ(IntentionCodeGenerator::invalidExitPointIndex, data.exitPointIndex);
        }

        TEST_F(IntentionCodeGeneratorTest, IgnoresAirfieldGroupsThatArentApplicable)
        {
            NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
            std::vector<std::unique_ptr<AirfieldGroup>> groups;
            std::unique_ptr<MockAirfieldGroup> mockAirfieldGroup(new MockAirfieldGroup(false));

            MockAirfieldGroup * mockGroupRaw = mockAirfieldGroup.get();

            groups.push_back(std::move(mockAirfieldGroup));
            IntentionCodeGenerator intention(std::move(groups), *SectorExitRepositoryFactory::Create());
            IntentionCodeData data = intention.GetIntentionCodeForFlightplan(
                "BAW123",
                "OMDB",
                "EGNX",
                mockFlightPlan,
                27000
            );
            EXPECT_TRUE(data.intentionCode == "EGNX");
            EXPECT_FALSE(data.exitPointValid);
            EXPECT_EQ("", data.exitPoint);
            EXPECT_EQ(IntentionCodeGenerator::invalidExitPointIndex, data.exitPointIndex);
        }

        TEST_F(IntentionCodeGeneratorTest, ReturnsCorrectCodeNormalCase)
        {
            NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
            std::vector<std::unique_ptr<AirfieldGroup>> groups;

            ON_CALL(mockFlightPlan, GetPointsNumber())
                .WillByDefault(Return(3));

            ON_CALL(mockFlightPlan, GetPointName(0))
                .WillByDefault(Return("EGKK"));

            ON_CALL(mockFlightPlan, GetPointName(1))
                .WillByDefault(Return("BAKUR"));

            ON_CALL(mockFlightPlan, GetPointName(2))
                .WillByDefault(Return("KFJK"));

            ON_CALL(mockFlightPlan, GetPointPosition(1))
                .WillByDefault(Return(exitPositionWest));

            ON_CALL(mockFlightPlan, GetPointPosition(2))
                .WillByDefault(Return(nextFixPositonWest));

            ON_CALL(mockFlightPlan, GetPointDistanceInMinutes(_))
                .WillByDefault(Return(999));

            std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
            IntentionCodeGenerator intention(std::move(groups), std::move(*exitPoints));
            IntentionCodeData data = intention.GetIntentionCodeForFlightplan(
                "BAW123",
                "EGKK",
                "KFJK",
                mockFlightPlan,
                24000
            );
            EXPECT_TRUE(data.intentionCode == "S3");
            EXPECT_TRUE(data.exitPointValid);
            EXPECT_EQ("BAKUR", data.exitPoint);
            EXPECT_EQ(1, data.exitPointIndex);
        }

        TEST_F(IntentionCodeGeneratorTest, ItSkipsNonApplicableCodes)
        {
            NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
            std::vector<std::unique_ptr<AirfieldGroup>> groups;

            ON_CALL(mockFlightPlan, GetPointsNumber())
                .WillByDefault(Return(3));

            ON_CALL(mockFlightPlan, GetPointName(0))
                .WillByDefault(Return("EGKK"));

            ON_CALL(mockFlightPlan, GetPointName(1))
                .WillByDefault(Return("LAKEY"));

            ON_CALL(mockFlightPlan, GetPointName(2))
                .WillByDefault(Return("KFJK"));

            ON_CALL(mockFlightPlan, GetPointPosition(1))
                .WillByDefault(Return(exitPositionSouth));

            ON_CALL(mockFlightPlan, GetPointPosition(2))
                .WillByDefault(Return(nextFixPositonSouth));

            ON_CALL(mockFlightPlan, GetPointDistanceInMinutes(_))
                .WillByDefault(Return(999));

            std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
            IntentionCodeGenerator intention(std::move(groups), std::move(*exitPoints));
            IntentionCodeData data = intention.GetIntentionCodeForFlightplan(
                "BAW123",
                "EGKK",
                "KFJK",
                mockFlightPlan,
                24000
            );
            EXPECT_TRUE(data.intentionCode == "KFJK");
            EXPECT_FALSE(data.exitPointValid);
            EXPECT_EQ("", data.exitPoint);
            EXPECT_EQ(intention.invalidExitPointIndex, data.exitPointIndex);
        }

        TEST_F(IntentionCodeGeneratorTest, ReturnsDestinationIcaoNoMatch)
        {
            NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
            std::vector<std::unique_ptr<AirfieldGroup>> groups;

            ON_CALL(mockFlightPlan, GetPointsNumber())
                .WillByDefault(Return(4));

            ON_CALL(mockFlightPlan, GetPointName(0))
                .WillByDefault(Return("EGKK"));

            ON_CALL(mockFlightPlan, GetPointName(1))
                .WillByDefault(Return("BEL"));

            ON_CALL(mockFlightPlan, GetPointName(2))
                .WillByDefault(Return("NOTAFIX"));

            ON_CALL(mockFlightPlan, GetPointName(3))
                .WillByDefault(Return("KLAS"));

            std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
            IntentionCodeGenerator intention(std::move(groups), std::move(*exitPoints));
            IntentionCodeData data = intention.GetIntentionCodeForFlightplan(
                "BAW123",
                "EGKK",
                "KLAS",
                mockFlightPlan,
                24000
            );
            EXPECT_TRUE(data.intentionCode == "KLAS");
            EXPECT_FALSE(data.exitPointValid);
            EXPECT_EQ("", data.exitPoint);
            EXPECT_EQ(IntentionCodeGenerator::invalidExitPointIndex, data.exitPointIndex);
        }

        TEST_F(IntentionCodeGeneratorTest, GetIntentionCodeForFlightPlanIgnoresFixIfTravellingInWrongDirection)
        {
            NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
            std::vector<std::unique_ptr<AirfieldGroup>> groups;

            ON_CALL(mockFlightPlan, GetPointsNumber())
                .WillByDefault(Return(3));

            ON_CALL(mockFlightPlan, GetPointName(0))
                .WillByDefault(Return("KLAS"));

            ON_CALL(mockFlightPlan, GetPointName(1))
                .WillByDefault(Return("BAKUR"));

            ON_CALL(mockFlightPlan, GetPointName(2))
                .WillByDefault(Return("NOTAFIX"));

            ON_CALL(mockFlightPlan, GetPointName(3))
                .WillByDefault(Return("EDDM"));

            ON_CALL(mockFlightPlan, GetPointPosition(1))
                .WillByDefault(Return(exitPositionEast));

            ON_CALL(mockFlightPlan, GetPointPosition(2))
                .WillByDefault(Return(nextFixPositonEast));

            ON_CALL(mockFlightPlan, GetPointDistanceInMinutes(_))
                .WillByDefault(Return(999));

            std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
            IntentionCodeGenerator intention(std::move(groups), std::move(*exitPoints));
            IntentionCodeData data = intention.GetIntentionCodeForFlightplan(
                "BAW123",
                "KLAS",
                "EDDM",
                mockFlightPlan,
                24000
            );
            EXPECT_TRUE(data.intentionCode == "EDDM");
            EXPECT_FALSE(data.exitPointValid);
            EXPECT_EQ("", data.exitPoint);
            EXPECT_EQ(IntentionCodeGenerator::invalidExitPointIndex, data.exitPointIndex);
        }

        TEST_F(IntentionCodeGeneratorTest, GetIntentionCodeReturnsDifferentCodeIfFlightplanChanges)
        {
            NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
            std::vector<std::unique_ptr<AirfieldGroup>> groups;
            std::unique_ptr<MockAirfieldGroup> mockAirfieldGroup(new StrictMock<MockAirfieldGroup>);

            MockAirfieldGroup * groupRaw = mockAirfieldGroup.get();

            groups.push_back(std::move(mockAirfieldGroup));
            std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
            IntentionCodeGenerator intention(std::move(groups), std::move(*exitPoints));
            IntentionCodeData data = intention.GetIntentionCodeForFlightplan(
                "BAW123",
                "KLAS",
                "EGLL",
                mockFlightPlan,
                33000
            );
            EXPECT_TRUE(data.intentionCode == "LL");
            EXPECT_FALSE(data.exitPointValid);
            EXPECT_EQ(IntentionCodeGenerator::invalidExitPointIndex, data.exitPointIndex);
            data = intention.GetIntentionCodeForFlightplan("BAW123", "KLAS", "EGKK", mockFlightPlan, 35000);
            EXPECT_TRUE(data.intentionCode == "KK");
            EXPECT_FALSE(data.exitPointValid);
            EXPECT_EQ("", data.exitPoint);
            EXPECT_EQ(IntentionCodeGenerator::invalidExitPointIndex, data.exitPointIndex);
        }

        TEST_F(IntentionCodeGeneratorTest, GetIntentionCodeReturnsIcaoIfPointPassed)
        {
            NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
            std::vector<std::unique_ptr<AirfieldGroup>> groups;

            ON_CALL(mockFlightPlan, GetPointsNumber())
                .WillByDefault(Return(3));

            ON_CALL(mockFlightPlan, GetPointName(0))
                .WillByDefault(Return("EGLL"));

            ON_CALL(mockFlightPlan, GetPointName(1))
                .WillByDefault(Return("BAKUR"));

            ON_CALL(mockFlightPlan, GetPointName(2))
                .WillByDefault(Return("KLAS"));

            ON_CALL(mockFlightPlan, GetPointPosition(1))
                .WillByDefault(Return(exitPositionWest));

            ON_CALL(mockFlightPlan, GetPointPosition(2))
                .WillByDefault(Return(nextFixPositonWest));

            EXPECT_CALL(mockFlightPlan, GetPointDistanceInMinutes(1))
                .Times(1)
                .WillOnce(Return(-1));

            EXPECT_CALL(mockFlightPlan, GetPointsAssignedIndex())
                .Times(1)
                .WillOnce(Return(-1));

            std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
            IntentionCodeGenerator intention(std::move(groups), std::move(*exitPoints));
            IntentionCodeData data = intention.GetIntentionCodeForFlightplan(
                "BAW123",
                "EGLL",
                "KLAS",
                mockFlightPlan,
                33000
            );
            EXPECT_TRUE(data.intentionCode == "KLAS");
            EXPECT_FALSE(data.exitPointValid);
            EXPECT_EQ("", data.exitPoint);
            EXPECT_EQ(IntentionCodeGenerator::invalidExitPointIndex, data.exitPointIndex);
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
