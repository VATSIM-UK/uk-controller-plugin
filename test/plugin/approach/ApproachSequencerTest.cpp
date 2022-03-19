#include "approach/ApproachSequence.h"
#include "approach/ApproachSequencedAircraft.h"
#include "approach/ApproachSequencer.h"

using UKControllerPlugin::Approach::ApproachSequence;
using UKControllerPlugin::Approach::ApproachSequencer;
using UKControllerPlugin::Approach::ApproachSequencingMode;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerTest : public testing::Test
    {
        public:
        ApproachSequencer sequencer;
    };

    TEST_F(ApproachSequencerTest, ItStartsWithNoSequences)
    {
        EXPECT_EQ(0, sequencer.CountSequences());
    }

    TEST_F(ApproachSequencerTest, ItReturnsSequence)
    {
        EXPECT_EQ(nullptr, sequencer.GetForAirfield("EGKK").First());
        EXPECT_EQ(1, sequencer.CountSequences());
    }

    TEST_F(ApproachSequencerTest, ItReturnsTheSameSequence)
    {
        EXPECT_EQ(&sequencer.GetForAirfield("EGKK"), &sequencer.GetForAirfield("EGKK"));
        EXPECT_EQ(1, sequencer.CountSequences());
    }

    TEST_F(ApproachSequencerTest, ItAddsAircraftToASequence)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        auto aircraft = sequencer.GetForAirfield("EGKK").First();
        EXPECT_EQ("BAW123", aircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, aircraft->Mode());
    }

    TEST_F(ApproachSequencerTest, ItRemovesAircraftFromSequenceIfInsertedIntoAnother)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGLL", "BAW123", ApproachSequencingMode::WakeTurbulence);
        EXPECT_EQ(std::list<std::string>{""}, sequencer.GetForAirfield("EGKK").Callsigns());
        EXPECT_EQ(std::list<std::string>{"BAW123"}, sequencer.GetForAirfield("EGLL").Callsigns());
    }

    TEST_F(ApproachSequencerTest, ItAddsAircraftToASequenceBeforeAnother)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGKK", "BAW456", ApproachSequencingMode::MinimumDistance, "BAW123");
        auto aircraft = sequencer.GetForAirfield("EGKK").First();
        EXPECT_EQ("BAW456", aircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::MinimumDistance, aircraft->Mode());
    }

    TEST_F(ApproachSequencerTest, ItRemovesAircraftFromOneSequenceWhenInsertedBeforeIntoAnother)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGKK", "BAW456", ApproachSequencingMode::WakeTurbulence, "BAW123");
        sequencer.AddAircraftToSequence("EGLL", "BAW123", ApproachSequencingMode::WakeTurbulence, "BAW123");
        EXPECT_EQ(std::list<std::string>{"BAW456"}, sequencer.GetForAirfield("EGKK").Callsigns());
    }

    TEST_F(ApproachSequencerTest, ItRemovesAnAircraftFromSequence)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.RemoveAircraftFromSequences("BAW123");
        EXPECT_EQ(nullptr, sequencer.GetForAirfield("EGKK").First());
    }

    TEST_F(ApproachSequencerTest, ItRemovesAnNonExistentAircraftFromSequence)
    {
        sequencer.RemoveAircraftFromSequences("BAW123");
        EXPECT_EQ(nullptr, sequencer.GetForAirfield("EGKK").First());
    }

    TEST_F(ApproachSequencerTest, ItMovesAircraftUp)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGKK", "BAW456", ApproachSequencingMode::WakeTurbulence);
        sequencer.MoveAircraftUp("EGKK", "BAW456");

        EXPECT_EQ(std::list<std::string>({"BAW456", "BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
    }

    TEST_F(ApproachSequencerTest, ItMovesAircraftDown)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGKK", "BAW456", ApproachSequencingMode::WakeTurbulence);
        sequencer.MoveAircraftDown("EGKK", "BAW123");

        EXPECT_EQ(std::list<std::string>({"BAW456", "BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
    }

    TEST_F(ApproachSequencerTest, ItIteratesSequences)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGLL", "BAW456", ApproachSequencingMode::WakeTurbulence);

        std::vector<std::list<std::string>> expectedCallsigns;
        expectedCallsigns.push_back({"BAW123"});
        expectedCallsigns.push_back({"BAW456"});

        std::vector<std::list<std::string>> callsigns;
        sequencer.ForEach(
            [&callsigns](const ApproachSequence& sequence) { callsigns.push_back(sequence.Callsigns()); });

        EXPECT_EQ(expectedCallsigns, callsigns);
    }

    TEST_F(ApproachSequencerTest, ItReturnsEmptyAirfieldIfAircraftNotInSequence)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGLL", "BAW456", ApproachSequencingMode::WakeTurbulence);

        EXPECT_TRUE(sequencer.AirfieldForAircraft("BAW999").empty());
    }

    TEST_F(ApproachSequencerTest, ItReturnsAirfieldForAircraft)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGLL", "BAW456", ApproachSequencingMode::WakeTurbulence);

        EXPECT_EQ("EGLL", sequencer.AirfieldForAircraft("BAW456"));
    }
} // namespace UKControllerPluginTest::Approach
