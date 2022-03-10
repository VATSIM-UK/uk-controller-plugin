#include "approach/ApproachSequence.h"
#include "approach/ApproachSequencedAircraft.h"
#include "approach/ApproachSequencer.h"

using UKControllerPlugin::Approach::ApproachSequencer;
using UKControllerPlugin::Approach::ApproachSequencingMode;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerTest : public testing::Test
    {
        public:
        ApproachSequencer sequencer;
    };

    TEST_F(ApproachSequencerTest, ItReturnsSequence)
    {
        EXPECT_EQ(nullptr, sequencer.GetForAirfield("EGKK").First());
    }

    TEST_F(ApproachSequencerTest, ItReturnsTheSameSequence)
    {
        EXPECT_EQ(&sequencer.GetForAirfield("EGKK"), &sequencer.GetForAirfield("EGKK"));
    }

    TEST_F(ApproachSequencerTest, ItAddsAircraftToASequence)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        auto aircraft = sequencer.GetForAirfield("EGKK").First();
        EXPECT_EQ("BAW123", aircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, aircraft->Mode());
    }

    TEST_F(ApproachSequencerTest, ItAssertsAircraftCannotBeInMoreThanOneSequence)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        EXPECT_DEATH(
            sequencer.AddAircraftToSequence("EGLL", "BAW123", ApproachSequencingMode::WakeTurbulence),
            "Aircraft already in arrival sequence");
    }

    TEST_F(ApproachSequencerTest, ItAddsAircraftToASequenceBeforeAnother)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGKK", "BAW456", ApproachSequencingMode::Minimum, "BAW123");
        auto aircraft = sequencer.GetForAirfield("EGKK").First();
        EXPECT_EQ("BAW456", aircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::Minimum, aircraft->Mode());
    }

    TEST_F(ApproachSequencerTest, ItAssertsAircraftCannotBeInMoreThanOneSequenceWhenInsertedBeforeAnother)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGKK", "BAW456", ApproachSequencingMode::WakeTurbulence, "BAW123");
        EXPECT_DEATH(
            sequencer.AddAircraftToSequence("EGLL", "BAW123", ApproachSequencingMode::WakeTurbulence, "BAW123"),
            "Aircraft already in arrival sequence");
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
} // namespace UKControllerPluginTest::Approach
