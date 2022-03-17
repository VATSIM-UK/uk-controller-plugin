#include "approach/AircraftSelectionProvider.h"
#include "approach/AirfieldApproachOptions.h"
#include "approach/ApproachSequence.h"
#include "approach/ApproachSequencedAircraft.h"
#include "approach/ApproachSequencer.h"
#include "approach/ApproachSequencerDisplayOptions.h"
#include "approach/ApproachSequencerOptions.h"

using UKControllerPlugin::Approach::AircraftSelectionProvider;
using UKControllerPlugin::Approach::AirfieldApproachOptions;
using UKControllerPlugin::Approach::ApproachSequencer;
using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;
using UKControllerPlugin::Approach::ApproachSequencerOptions;
using UKControllerPlugin::Approach::ApproachSequencingMode;

namespace UKControllerPluginTest::Approach {
    class AircraftSelectionProviderTest : public testing::Test
    {
        public:
        AircraftSelectionProviderTest()
            : options(std::make_shared<ApproachSequencerDisplayOptions>()),
              provider(sequencer, sequencerOptions, options, plugin)
        {
            flightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
            radarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
            flightplan2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
            radarTarget2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();

            ON_CALL(*flightplan1, GetCallsign).WillByDefault(testing::Return("EZY413"));
            ON_CALL(*flightplan1, GetDestination).WillByDefault(testing::Return("EGLL"));
            ON_CALL(*radarTarget1, GetGroundSpeed).WillByDefault(testing::Return(55));
            ON_CALL(*flightplan2, GetCallsign).WillByDefault(testing::Return("BAW123"));
            ON_CALL(*flightplan2, GetDestination).WillByDefault(testing::Return("EGLL"));
            ON_CALL(*radarTarget2, GetGroundSpeed).WillByDefault(testing::Return(56));

            plugin.AddAllFlightplansItem({flightplan1, radarTarget1});
            plugin.AddAllFlightplansItem({flightplan2, radarTarget2});
        }

        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> flightplan1;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> radarTarget1;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> flightplan2;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> radarTarget2;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        ApproachSequencer sequencer;
        std::shared_ptr<ApproachSequencerDisplayOptions> options;
        ApproachSequencerOptions sequencerOptions;
        AircraftSelectionProvider provider;
    };

    TEST_F(AircraftSelectionProviderTest, ItReturnsEmptyCallsignListIfAirfieldNotSelected)
    {
        EXPECT_EQ(0, provider.GetCallsigns().size());
    }

    TEST_F(AircraftSelectionProviderTest, ItReturnsAllCallsigns)
    {
        options->Airfield("EGLL");
        EXPECT_EQ(std::set<std::string>({"BAW123", "EZY413"}), provider.GetCallsigns());
    }

    TEST_F(AircraftSelectionProviderTest, ItIgnoresCallsignsWithIncorrectDestination)
    {
        ON_CALL(*flightplan1, GetDestination).WillByDefault(testing::Return("EGKK"));
        options->Airfield("EGLL");
        EXPECT_EQ(std::set<std::string>({"BAW123"}), provider.GetCallsigns());
    }

    TEST_F(AircraftSelectionProviderTest, ItIgnoresCallsignsAlreadyInSequence)
    {
        sequencer.AddAircraftToSequence("EGLL", "BAW123", ApproachSequencingMode::WakeTurbulence);
        options->Airfield("EGLL");
        EXPECT_EQ(std::set<std::string>({"EZY413"}), provider.GetCallsigns());
    }

    TEST_F(AircraftSelectionProviderTest, ItIgnoresCallsignsMovingTooSlowly)
    {
        ON_CALL(*radarTarget2, GetGroundSpeed).WillByDefault(testing::Return(30));
        options->Airfield("EGLL");
        EXPECT_EQ(std::set<std::string>({"EZY413"}), provider.GetCallsigns());
    }

    TEST_F(AircraftSelectionProviderTest, ItAddsCallsign)
    {
        EXPECT_EQ(0, provider.GetCallsigns().size());
    }

    TEST_F(AircraftSelectionProviderTest, ItDoesNothingNoAirfieldSelected)
    {
        provider.CallsignSelected("BAW123");
        EXPECT_EQ(0, sequencer.CountSequences());
    }

    TEST_F(AircraftSelectionProviderTest, ItAddsAircraftToSequenceForAirfieldInWakeMode)
    {
        options->Airfield("EGLL");
        provider.CallsignSelected("BAW123");

        auto sequence = sequencer.GetForAirfield("EGLL");
        EXPECT_EQ(1, sequencer.CountSequences());
        EXPECT_EQ(1, sequence.Callsigns().size());
        auto aircraft = sequence.Get("BAW123");
        EXPECT_EQ("BAW123", aircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, aircraft->Mode());
    }

    TEST_F(AircraftSelectionProviderTest, ItAddsAircraftToSequenceForAirfieldInDistanceMode)
    {
        sequencerOptions.Set(
            "EGLL", std::make_shared<AirfieldApproachOptions>(ApproachSequencingMode::MinimumDistance, 4.5, 3.5));
        options->Airfield("EGLL");
        provider.CallsignSelected("BAW123");

        auto sequence = sequencer.GetForAirfield("EGLL");
        EXPECT_EQ(1, sequencer.CountSequences());
        EXPECT_EQ(1, sequence.Callsigns().size());
        auto aircraft = sequence.Get("BAW123");
        EXPECT_EQ("BAW123", aircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::MinimumDistance, aircraft->Mode());
        EXPECT_DOUBLE_EQ(4.5, aircraft->ExpectedDistance());
    }

    TEST_F(AircraftSelectionProviderTest, ItRemovesAircraftFromOtherSequencesIfSelected)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        options->Airfield("EGLL");
        provider.CallsignSelected("BAW123");

        auto sequence = sequencer.GetForAirfield("EGLL");
        EXPECT_EQ(1, sequence.Callsigns().size());
        auto aircraft = sequence.Get("BAW123");
        EXPECT_EQ("BAW123", aircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, aircraft->Mode());

        EXPECT_EQ(0, sequencer.GetForAirfield("EGKK").Callsigns().size());
    }
} // namespace UKControllerPluginTest::Approach
