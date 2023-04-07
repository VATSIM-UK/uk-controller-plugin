#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "approach/AirfieldApproachOptions.h"
#include "approach/ApproachSequencedAircraft.h"
#include "approach/ApproachSequencerOptions.h"
#include "approach/ApproachSpacingCalculator.h"
#include "controller/ControllerPositionHierarchy.h"
#include "wake/ArrivalWakeInterval.h"
#include "wake/WakeCategory.h"
#include "wake/WakeCategoryMapperCollection.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Approach::AirfieldApproachOptions;
using UKControllerPlugin::Approach::ApproachSequencedAircraft;
using UKControllerPlugin::Approach::ApproachSequencerOptions;
using UKControllerPlugin::Approach::ApproachSequencingMode;
using UKControllerPlugin::Approach::ApproachSpacingCalculator;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Prenote::PairedAirfieldPrenote;
using UKControllerPlugin::Wake::ArrivalWakeInterval;
using UKControllerPlugin::Wake::DepartureWakeInterval;
using UKControllerPlugin::Wake::WakeCategory;
using UKControllerPlugin::Wake::WakeCategoryMapperCollection;

namespace UKControllerPluginTest::Approach {
    class ApproachSpacingCalculatorTest : public testing::Test
    {
        public:
        ApproachSpacingCalculatorTest()
            : arrivalWakeIntervals({
                  std::make_shared<ArrivalWakeInterval>(3, 3.5),
                  std::make_shared<ArrivalWakeInterval>(6, 4.5),
              }),
              category(std::make_shared<WakeCategory>(
                  3,
                  "LM",
                  "Lower Medium",
                  20,
                  std::list<std::shared_ptr<DepartureWakeInterval>>{},
                  std::list<std::shared_ptr<ArrivalWakeInterval>>{})),
              previousCategory(std::make_shared<WakeCategory>(
                  1,
                  "LM",
                  "Lower Medium",
                  20,
                  std::list<std::shared_ptr<DepartureWakeInterval>>{},
                  arrivalWakeIntervals)),
              flightplan(std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>()),
              previousFlightplan(std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>()),
              wakeMapper1(std::make_shared<testing::NiceMock<Wake::MockWakeCategoryMapper>>()),
              wakeMapper2(std::make_shared<testing::NiceMock<Wake::MockWakeCategoryMapper>>()),
              aircraft(std::make_shared<ApproachSequencedAircraft>("BAW123", ApproachSequencingMode::WakeTurbulence)),
              previousAircraft(
                  std::make_shared<ApproachSequencedAircraft>("EZY456", ApproachSequencingMode::WakeTurbulence)),
              calculator(sequencerOptions, airfields, wakeMappers, plugin)
        {
            ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(flightplan));
            ON_CALL(plugin, GetFlightplanForCallsign("EZY456")).WillByDefault(testing::Return(previousFlightplan));

            // Wake mapping
            ON_CALL(*wakeMapper1, MapForFlightplan(testing::Ref(*flightplan))).WillByDefault(testing::Return(category));

            ON_CALL(*wakeMapper1, MapForFlightplan(testing::Ref(*previousFlightplan)))
                .WillByDefault(testing::Return(previousCategory));

            wakeMappers.Add(1, wakeMapper1);
            wakeMappers.Add(2, wakeMapper1);

            // Airfields
            airfields.AddAirfield(std::make_shared<AirfieldModel>(
                1,
                "EGKK",
                std::make_unique<ControllerPositionHierarchy>(),
                std::vector<std::shared_ptr<PairedAirfieldPrenote>>{},
                4,
                1));

            airfields.AddAirfield(std::make_shared<AirfieldModel>(
                2,
                "EGLL",
                std::make_unique<ControllerPositionHierarchy>(),
                std::vector<std::shared_ptr<PairedAirfieldPrenote>>{},
                4,
                55));

            // Set up the previous
            aircraft->Previous(previousAircraft);
        }

        ApproachSequencerOptions sequencerOptions;
        std::list<std::shared_ptr<ArrivalWakeInterval>> arrivalWakeIntervals;
        std::shared_ptr<WakeCategory> category;
        std::shared_ptr<WakeCategory> previousCategory;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> flightplan;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> previousFlightplan;
        std::shared_ptr<testing::NiceMock<Wake::MockWakeCategoryMapper>> wakeMapper1;
        std::shared_ptr<testing::NiceMock<Wake::MockWakeCategoryMapper>> wakeMapper2;
        AirfieldCollection airfields;
        WakeCategoryMapperCollection wakeMappers;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        std::shared_ptr<ApproachSequencedAircraft> aircraft;
        std::shared_ptr<ApproachSequencedAircraft> previousAircraft;
        ApproachSpacingCalculator calculator;
    };

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsRequiredWakeInterval)
    {
        EXPECT_DOUBLE_EQ(3.5, calculator.Calculate("EGKK", *aircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsMinimumSeparationIfLargetThanWakeInterval)
    {
        sequencerOptions.Set(
            "EGKK", std::make_shared<AirfieldApproachOptions>(ApproachSequencingMode::WakeTurbulence, 3.0, 5.5));
        EXPECT_DOUBLE_EQ(3.5, calculator.Calculate("EGKK", *aircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsAirfieldDefaultSeparationIfNoWakeIntervalPresentInWakeMode)
    {
        previousAircraft->Previous(aircraft);
        EXPECT_DOUBLE_EQ(3.0, calculator.Calculate("EGKK", *previousAircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsTargetDistanceIfNoWakeIntervalPresentInDistanceMode)
    {
        previousAircraft->Mode(ApproachSequencingMode::MinimumDistance);
        previousAircraft->ExpectedDistance(4.2);
        previousAircraft->Previous(aircraft);
        EXPECT_DOUBLE_EQ(4.2, calculator.Calculate("EGKK", *previousAircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsTargetDistanceIfGreaterThanWakeRequirementInDistanceMode)
    {
        aircraft->Mode(ApproachSequencingMode::MinimumDistance);
        aircraft->ExpectedDistance(4.2);
        EXPECT_DOUBLE_EQ(4.2, calculator.Calculate("EGKK", *aircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsMinimumSeparationIfGreaterThanTargetDistanceInDistanceMode)
    {
        sequencerOptions.Set(
            "EGKK", std::make_shared<AirfieldApproachOptions>(ApproachSequencingMode::WakeTurbulence, 3.0, 4.5));
        aircraft->Mode(ApproachSequencingMode::MinimumDistance);
        aircraft->ExpectedDistance(4.2);
        EXPECT_DOUBLE_EQ(4.5, calculator.Calculate("EGKK", *aircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsWakeDistanceIfGreaterThanTargetInDistanceMode)
    {
        aircraft->Mode(ApproachSequencingMode::MinimumDistance);
        aircraft->ExpectedDistance(3.1);
        EXPECT_DOUBLE_EQ(3.5, calculator.Calculate("EGKK", *aircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsAirfieldSeparationIfGreaterThanWakeInDistanceMode)
    {
        sequencerOptions.Set(
            "EGKK", std::make_shared<AirfieldApproachOptions>(ApproachSequencingMode::WakeTurbulence, 3.0, 4.0));
        aircraft->Mode(ApproachSequencingMode::MinimumDistance);
        aircraft->ExpectedDistance(3.1);
        EXPECT_DOUBLE_EQ(4.0, calculator.Calculate("EGKK", *aircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsAirfieldDefaultSpacingIfAircraftDoesntHaveWakeCategory)
    {
        ON_CALL(*wakeMapper1, MapForFlightplan(testing::Ref(*flightplan))).WillByDefault(testing::Return(nullptr));
        EXPECT_DOUBLE_EQ(3.0, calculator.Calculate("EGKK", *aircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsAirfieldDefaultSpacingIfPreviousAircraftDoesntHaveWakeCategory)
    {
        ON_CALL(*wakeMapper1, MapForFlightplan(testing::Ref(*previousFlightplan)))
            .WillByDefault(testing::Return(nullptr));
        EXPECT_DOUBLE_EQ(3.0, calculator.Calculate("EGKK", *aircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsNoSpacingIfAircraftDoesntHaveFlightplan)
    {
        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(nullptr));
        EXPECT_DOUBLE_EQ(ApproachSpacingCalculator::NoSpacing(), calculator.Calculate("EGKK", *aircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsNoSpacingIfPreviousAircraftDoesntHaveFlightplan)
    {
        ON_CALL(plugin, GetFlightplanForCallsign("EZY456")).WillByDefault(testing::Return(nullptr));
        EXPECT_DOUBLE_EQ(ApproachSpacingCalculator::NoSpacing(), calculator.Calculate("EGKK", *aircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsMinimumSeparationIfNoWakeMapperAtAirfield)
    {
        EXPECT_DOUBLE_EQ(3.0, calculator.Calculate("EGLL", *aircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsNoSpacingIfAirfieldNotFound)
    {
        EXPECT_DOUBLE_EQ(ApproachSpacingCalculator::NoSpacing(), calculator.Calculate("EGZZ", *aircraft));
    }

    TEST_F(ApproachSpacingCalculatorTest, ItReturnsNoSpacingIfAirfieldFirstInSequence)
    {
        aircraft->Previous(nullptr);
        EXPECT_DOUBLE_EQ(ApproachSpacingCalculator::NoSpacing(), calculator.Calculate("EGKK", *aircraft));
    }
} // namespace UKControllerPluginTest::Approach
