#include "intention/IntentionCodeData.h"
#include "intention/IntentionCodeGenerator.h"
#include "intention/SectorExitRepository.h"
#include "intention/SectorExitRepositoryFactory.h"

using EuroScopePlugIn::CPosition;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::Test;

using UKControllerPlugin::IntentionCode::AirfieldGroup;
using UKControllerPlugin::IntentionCode::IntentionCodeData;
using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPlugin::IntentionCode::SectorExitRepository;
using UKControllerPlugin::IntentionCode::SectorExitRepositoryFactory;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;

namespace UKControllerPluginTest::IntentionCode {

    class IntentionCodeGeneratorTest : public ::testing::Test
    {
        public:
        void SetUp(void)
        {

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
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.exitPointIndex);
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.ukExitPointIndex);
        EXPECT_EQ("", data.exitPoint);
        EXPECT_EQ("", data.ukExitPoint);
        EXPECT_EQ(IntentionCodeGenerator::GetInvalidExitPointIndex(), data.exitPointIndex);
    }

    TEST_F(IntentionCodeGeneratorTest, ReturnsCorrectCodeFromAirfieldGroup)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
        std::vector<std::unique_ptr<AirfieldGroup>> groups;
        std::unique_ptr<StrictMock<MockAirfieldGroup>> mockAirfieldGroup(new StrictMock<MockAirfieldGroup>);

        groups.push_back(std::move(mockAirfieldGroup));
        IntentionCodeGenerator intention(std::move(groups), *SectorExitRepositoryFactory::Create());
        IntentionCodeData data =
            intention.GetIntentionCodeForFlightplan("BAW123", "OMDB", "EGLL", mockFlightPlan, 27000);
        EXPECT_TRUE(data.intentionCode == "LL");
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.exitPointIndex);
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.ukExitPointIndex);
        EXPECT_EQ("", data.exitPoint);
        EXPECT_EQ("", data.ukExitPoint);
        EXPECT_EQ(IntentionCodeGenerator::GetInvalidExitPointIndex(), data.exitPointIndex);
    }

    TEST_F(IntentionCodeGeneratorTest, IgnoresAirfieldGroupsThatArentApplicable)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
        std::vector<std::unique_ptr<AirfieldGroup>> groups;
        std::unique_ptr<MockAirfieldGroup> mockAirfieldGroup(new MockAirfieldGroup(false));

        groups.push_back(std::move(mockAirfieldGroup));
        IntentionCodeGenerator intention(std::move(groups), *SectorExitRepositoryFactory::Create());
        IntentionCodeData data =
            intention.GetIntentionCodeForFlightplan("BAW123", "OMDB", "EGNX", mockFlightPlan, 27000);
        EXPECT_TRUE(data.intentionCode == "EGNX");
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.exitPointIndex);
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.ukExitPointIndex);
        EXPECT_EQ("", data.exitPoint);
        EXPECT_EQ("", data.ukExitPoint);
        EXPECT_EQ(IntentionCodeGenerator::GetInvalidExitPointIndex(), data.exitPointIndex);
    }

    TEST_F(IntentionCodeGeneratorTest, ReturnsCorrectCodeNormalCase)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
        std::vector<std::unique_ptr<AirfieldGroup>> groups;

        ON_CALL(mockFlightPlan, GetPointsNumber()).WillByDefault(Return(3));

        ON_CALL(mockFlightPlan, GetPointName(0)).WillByDefault(Return("EGKK"));

        ON_CALL(mockFlightPlan, GetPointName(1)).WillByDefault(Return("BAKUR"));

        ON_CALL(mockFlightPlan, GetPointName(2)).WillByDefault(Return("KFJK"));

        ON_CALL(mockFlightPlan, GetPointPosition(1)).WillByDefault(Return(exitPositionWest));

        ON_CALL(mockFlightPlan, GetPointPosition(2)).WillByDefault(Return(nextFixPositonWest));

        ON_CALL(mockFlightPlan, GetPointDistanceInMinutes(_)).WillByDefault(Return(999));

        std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
        IntentionCodeGenerator intention(std::move(groups), *exitPoints);
        IntentionCodeData data =
            intention.GetIntentionCodeForFlightplan("BAW123", "EGKK", "KFJK", mockFlightPlan, 24000);
        EXPECT_TRUE(data.intentionCode == "S3");
        EXPECT_EQ("BAKUR", data.exitPoint);
        EXPECT_EQ("BAKUR", data.ukExitPoint);
        EXPECT_EQ(1, data.exitPointIndex);
        EXPECT_EQ(1, data.ukExitPointIndex);
    }

    TEST_F(IntentionCodeGeneratorTest, ItGeneratesTheAppropriateUkAreaExit)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
        std::vector<std::unique_ptr<AirfieldGroup>> groups;

        ON_CALL(mockFlightPlan, GetPointsNumber()).WillByDefault(Return(5));

        ON_CALL(mockFlightPlan, GetPointName(0)).WillByDefault(Return("EGKK"));

        ON_CALL(mockFlightPlan, GetPointName(1)).WillByDefault(Return("ORTAC"));

        ON_CALL(mockFlightPlan, GetPointName(2)).WillByDefault(Return("FOOBY"));

        ON_CALL(mockFlightPlan, GetPointName(3)).WillByDefault(Return("BAKUR"));

        ON_CALL(mockFlightPlan, GetPointName(4)).WillByDefault(Return("KFJK"));

        ON_CALL(mockFlightPlan, GetPointPosition(1)).WillByDefault(Return(exitPositionSouth));

        ON_CALL(mockFlightPlan, GetPointPosition(2)).WillByDefault(Return(nextFixPositonSouth));

        ON_CALL(mockFlightPlan, GetPointPosition(3)).WillByDefault(Return(exitPositionWest));

        ON_CALL(mockFlightPlan, GetPointPosition(4)).WillByDefault(Return(nextFixPositonWest));

        ON_CALL(mockFlightPlan, GetPointDistanceInMinutes(_)).WillByDefault(Return(999));

        std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
        IntentionCodeGenerator intention(std::move(groups), *exitPoints);
        IntentionCodeData data =
            intention.GetIntentionCodeForFlightplan("BAW123", "EGKK", "KFJK", mockFlightPlan, 24000);
        EXPECT_EQ("H", data.intentionCode);
        EXPECT_EQ("ORTAC", data.exitPoint);
        EXPECT_EQ("BAKUR", data.ukExitPoint);
        EXPECT_EQ(1, data.exitPointIndex);
        EXPECT_EQ(3, data.ukExitPointIndex);
    }

    TEST_F(IntentionCodeGeneratorTest, ReturnsCorrectCodeIfClearedBeyondButPointNotPassed)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
        std::vector<std::unique_ptr<AirfieldGroup>> groups;

        ON_CALL(mockFlightPlan, GetPointsNumber()).WillByDefault(Return(3));

        ON_CALL(mockFlightPlan, GetPointName(0)).WillByDefault(Return("EGKK"));

        ON_CALL(mockFlightPlan, GetPointName(1)).WillByDefault(Return("BAKUR"));

        ON_CALL(mockFlightPlan, GetPointName(2)).WillByDefault(Return("KFJK"));

        ON_CALL(mockFlightPlan, GetPointPosition(1)).WillByDefault(Return(exitPositionWest));

        ON_CALL(mockFlightPlan, GetPointPosition(2)).WillByDefault(Return(nextFixPositonWest));

        ON_CALL(mockFlightPlan, GetPointDistanceInMinutes(_)).WillByDefault(Return(999));

        ON_CALL(mockFlightPlan, GetPointsAssignedIndex).WillByDefault(Return(2));

        ON_CALL(mockFlightPlan, GetPointsCalculatedIndex()).WillByDefault(Return(0));

        std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
        IntentionCodeGenerator intention(std::move(groups), *exitPoints);
        IntentionCodeData data =
            intention.GetIntentionCodeForFlightplan("BAW123", "EGKK", "KFJK", mockFlightPlan, 24000);
        EXPECT_TRUE(data.intentionCode == "S3");
        EXPECT_EQ("BAKUR", data.exitPoint);
        EXPECT_EQ("BAKUR", data.ukExitPoint);
        EXPECT_EQ(1, data.exitPointIndex);
        EXPECT_EQ(1, data.ukExitPointIndex);
    }

    TEST_F(IntentionCodeGeneratorTest, ItGeneratesTheAppropriateUkAreaExitButPointNotPassed)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
        std::vector<std::unique_ptr<AirfieldGroup>> groups;

        ON_CALL(mockFlightPlan, GetPointsNumber()).WillByDefault(Return(5));

        ON_CALL(mockFlightPlan, GetPointName(0)).WillByDefault(Return("EGKK"));

        ON_CALL(mockFlightPlan, GetPointName(1)).WillByDefault(Return("ORTAC"));

        ON_CALL(mockFlightPlan, GetPointName(2)).WillByDefault(Return("FOOBY"));

        ON_CALL(mockFlightPlan, GetPointName(3)).WillByDefault(Return("BAKUR"));

        ON_CALL(mockFlightPlan, GetPointName(4)).WillByDefault(Return("KFJK"));

        ON_CALL(mockFlightPlan, GetPointPosition(1)).WillByDefault(Return(exitPositionSouth));

        ON_CALL(mockFlightPlan, GetPointPosition(2)).WillByDefault(Return(nextFixPositonSouth));

        ON_CALL(mockFlightPlan, GetPointPosition(3)).WillByDefault(Return(exitPositionWest));

        ON_CALL(mockFlightPlan, GetPointPosition(4)).WillByDefault(Return(nextFixPositonWest));

        ON_CALL(mockFlightPlan, GetPointDistanceInMinutes(_)).WillByDefault(Return(999));

        ON_CALL(mockFlightPlan, GetPointsAssignedIndex).WillByDefault(Return(2));

        ON_CALL(mockFlightPlan, GetPointsCalculatedIndex()).WillByDefault(Return(0));

        std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
        IntentionCodeGenerator intention(std::move(groups), *exitPoints);
        IntentionCodeData data =
            intention.GetIntentionCodeForFlightplan("BAW123", "EGKK", "KFJK", mockFlightPlan, 24000);
        EXPECT_EQ("H", data.intentionCode);
        EXPECT_EQ("ORTAC", data.exitPoint);
        EXPECT_EQ("BAKUR", data.ukExitPoint);
        EXPECT_EQ(1, data.exitPointIndex);
        EXPECT_EQ(3, data.ukExitPointIndex);
    }

    TEST_F(IntentionCodeGeneratorTest, ItSkipsNonApplicableCodes)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
        std::vector<std::unique_ptr<AirfieldGroup>> groups;

        ON_CALL(mockFlightPlan, GetPointsNumber()).WillByDefault(Return(3));

        ON_CALL(mockFlightPlan, GetPointName(0)).WillByDefault(Return("EGKK"));

        ON_CALL(mockFlightPlan, GetPointName(1)).WillByDefault(Return("LAKEY"));

        ON_CALL(mockFlightPlan, GetPointName(2)).WillByDefault(Return("KFJK"));

        ON_CALL(mockFlightPlan, GetPointPosition(1)).WillByDefault(Return(exitPositionSouth));

        ON_CALL(mockFlightPlan, GetPointPosition(2)).WillByDefault(Return(nextFixPositonSouth));

        ON_CALL(mockFlightPlan, GetPointDistanceInMinutes(_)).WillByDefault(Return(999));

        std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
        IntentionCodeGenerator intention(std::move(groups), *exitPoints);
        IntentionCodeData data =
            intention.GetIntentionCodeForFlightplan("BAW123", "EGKK", "KFJK", mockFlightPlan, 24000);
        EXPECT_TRUE(data.intentionCode == "KFJK");
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.exitPointIndex);
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.ukExitPointIndex);
        EXPECT_EQ("", data.exitPoint);
        EXPECT_EQ("", data.ukExitPoint);
    }

    TEST_F(IntentionCodeGeneratorTest, ReturnsDestinationIcaoNoMatch)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
        std::vector<std::unique_ptr<AirfieldGroup>> groups;

        ON_CALL(mockFlightPlan, GetPointsNumber()).WillByDefault(Return(4));

        ON_CALL(mockFlightPlan, GetPointName(0)).WillByDefault(Return("EGKK"));

        ON_CALL(mockFlightPlan, GetPointName(1)).WillByDefault(Return("BEL"));

        ON_CALL(mockFlightPlan, GetPointName(2)).WillByDefault(Return("NOTAFIX"));

        ON_CALL(mockFlightPlan, GetPointName(3)).WillByDefault(Return("KLAS"));

        std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
        IntentionCodeGenerator intention(std::move(groups), *exitPoints);
        IntentionCodeData data =
            intention.GetIntentionCodeForFlightplan("BAW123", "EGKK", "KLAS", mockFlightPlan, 24000);
        EXPECT_TRUE(data.intentionCode == "KLAS");
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.exitPointIndex);
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.ukExitPointIndex);
        EXPECT_EQ("", data.exitPoint);
        EXPECT_EQ("", data.ukExitPoint);
    }

    TEST_F(IntentionCodeGeneratorTest, GetIntentionCodeForFlightPlanIgnoresFixIfTravellingInWrongDirection)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
        std::vector<std::unique_ptr<AirfieldGroup>> groups;

        ON_CALL(mockFlightPlan, GetPointsNumber()).WillByDefault(Return(3));

        ON_CALL(mockFlightPlan, GetPointName(0)).WillByDefault(Return("KLAS"));

        ON_CALL(mockFlightPlan, GetPointName(1)).WillByDefault(Return("BAKUR"));

        ON_CALL(mockFlightPlan, GetPointName(2)).WillByDefault(Return("NOTAFIX"));

        ON_CALL(mockFlightPlan, GetPointName(3)).WillByDefault(Return("EDDM"));

        ON_CALL(mockFlightPlan, GetPointPosition(1)).WillByDefault(Return(exitPositionEast));

        ON_CALL(mockFlightPlan, GetPointPosition(2)).WillByDefault(Return(nextFixPositonEast));

        ON_CALL(mockFlightPlan, GetPointDistanceInMinutes(_)).WillByDefault(Return(999));

        std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
        IntentionCodeGenerator intention(std::move(groups), *exitPoints);
        IntentionCodeData data =
            intention.GetIntentionCodeForFlightplan("BAW123", "KLAS", "EDDM", mockFlightPlan, 24000);
        EXPECT_TRUE(data.intentionCode == "EDDM");
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.exitPointIndex);
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.ukExitPointIndex);
        EXPECT_EQ("", data.exitPoint);
        EXPECT_EQ("", data.ukExitPoint);
    }

    TEST_F(IntentionCodeGeneratorTest, GetIntentionCodeReturnsDifferentCodeIfFlightplanChanges)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> mockFlightPlan;
        std::vector<std::unique_ptr<AirfieldGroup>> groups;
        std::unique_ptr<MockAirfieldGroup> mockAirfieldGroup(new StrictMock<MockAirfieldGroup>);

        groups.push_back(std::move(mockAirfieldGroup));
        std::unique_ptr<SectorExitRepository> exitPoints = SectorExitRepositoryFactory::Create();
        IntentionCodeGenerator intention(std::move(groups), *exitPoints);
        IntentionCodeData data =
            intention.GetIntentionCodeForFlightplan("BAW123", "KLAS", "EGLL", mockFlightPlan, 33000);
        EXPECT_TRUE(data.intentionCode == "LL");
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.exitPointIndex);
        EXPECT_EQ(IntentionCodeGenerator::GetInvalidExitPointIndex(), data.exitPointIndex);
        data = intention.GetIntentionCodeForFlightplan("BAW123", "KLAS", "EGKK", mockFlightPlan, 35000);
        EXPECT_TRUE(data.intentionCode == "KK");
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.exitPointIndex);
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.ukExitPointIndex);
        EXPECT_EQ("", data.exitPoint);
        EXPECT_EQ("", data.ukExitPoint);
    }
} // namespace UKControllerPluginTest::IntentionCode
