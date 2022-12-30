#include "intention/AircraftFirExit.h"
#include "intention/ExitingFirAtPoint.h"
#include "intention/FirExitPoint.h"

using UKControllerPlugin::IntentionCode::AircraftFirExit;
using UKControllerPlugin::IntentionCode::Condition;
using UKControllerPlugin::IntentionCode::ExitingFirAtPoint;
using UKControllerPlugin::IntentionCode::FirExitPoint;

namespace UKControllerPluginTest::IntentionCode {
    class ExitingFirAtPointTest : public testing::Test
    {
        public:
        ExitingFirAtPointTest()
            : exitDetermination(std::make_shared<testing::NiceMock<MockExitDetermination>>()),
              point1(std::make_shared<FirExitPoint>(1, "KONAN", false, exitDetermination)),
              point2(std::make_shared<FirExitPoint>(2, "KOK", false, exitDetermination)),
              point3(std::make_shared<FirExitPoint>(3, "ETRAT", false, exitDetermination)),
              exitingFirAtPoint(firExitGenerator, 1)
        {
        }

        std::shared_ptr<testing::NiceMock<MockExitDetermination>> exitDetermination;
        std::shared_ptr<FirExitPoint> point1;
        std::shared_ptr<FirExitPoint> point2;
        std::shared_ptr<FirExitPoint> point3;
        testing::NiceMock<MockAircraftFirExitGenerator> firExitGenerator;
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        ExitingFirAtPoint exitingFirAtPoint;
    };

    TEST_F(ExitingFirAtPointTest, ItFailsIfNoFirExitAtAll)
    {
        EXPECT_CALL(firExitGenerator, Generate(testing::Ref(flightplan))).Times(1).WillOnce(testing::Return(nullptr));

        EXPECT_FALSE(exitingFirAtPoint.Passes(flightplan, radarTarget));
    }

    TEST_F(ExitingFirAtPointTest, ItFailsIfNoExitPoints)
    {
        AircraftFirExit exit{"BAW123", nullptr, nullptr};
        const auto exitPointer = std::make_shared<AircraftFirExit>(exit);
        EXPECT_CALL(firExitGenerator, Generate(testing::Ref(flightplan)))
            .Times(1)
            .WillOnce(testing::Return(exitPointer));

        EXPECT_FALSE(exitingFirAtPoint.Passes(flightplan, radarTarget));
    }

    TEST_F(ExitingFirAtPointTest, ItFailsIfNoExitPointsMatch)
    {
        AircraftFirExit exit{"BAW123", point2, point3};
        const auto exitPointer = std::make_shared<AircraftFirExit>(exit);
        EXPECT_CALL(firExitGenerator, Generate(testing::Ref(flightplan)))
            .Times(1)
            .WillOnce(testing::Return(exitPointer));

        EXPECT_FALSE(exitingFirAtPoint.Passes(flightplan, radarTarget));
    }

    TEST_F(ExitingFirAtPointTest, ItPassesOnInternalExitMatch)
    {
        AircraftFirExit exit{"BAW123", point1, point3};
        const auto exitPointer = std::make_shared<AircraftFirExit>(exit);
        EXPECT_CALL(firExitGenerator, Generate(testing::Ref(flightplan)))
            .Times(1)
            .WillOnce(testing::Return(exitPointer));

        EXPECT_TRUE(exitingFirAtPoint.Passes(flightplan, radarTarget));
    }

    TEST_F(ExitingFirAtPointTest, ItPassesOnUkExitMatch)
    {
        AircraftFirExit exit{"BAW123", point2, point1};
        const auto exitPointer = std::make_shared<AircraftFirExit>(exit);
        EXPECT_CALL(firExitGenerator, Generate(testing::Ref(flightplan)))
            .Times(1)
            .WillOnce(testing::Return(exitPointer));

        EXPECT_TRUE(exitingFirAtPoint.Passes(flightplan, radarTarget));
    }
} // namespace UKControllerPluginTest::IntentionCode
