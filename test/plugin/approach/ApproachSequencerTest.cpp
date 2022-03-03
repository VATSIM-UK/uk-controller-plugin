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

    TEST_F(ApproachSequencerTest, SequenceStartsEmpty)
    {
        EXPECT_EQ(nullptr, sequencer.First());
    }

    TEST_F(ApproachSequencerTest, GetReturnsNullptrAircraftNotFound)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        EXPECT_EQ(nullptr, sequencer.Get("BAW456"));
    }

    TEST_F(ApproachSequencerTest, ItAddsAircraftToTheBackOfTheSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        auto firstAircraft = sequencer.First();
        EXPECT_EQ("BAW123", firstAircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::Minimum, firstAircraft->Mode());
        EXPECT_EQ(nullptr, firstAircraft->Previous());
        EXPECT_EQ(nullptr, firstAircraft->Next());

        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::WakeTurbulence);
        EXPECT_EQ(firstAircraft, sequencer.First());
        auto secondAircraft = sequencer.Get("BAW456");
        EXPECT_EQ("BAW456", secondAircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, secondAircraft->Mode());
        EXPECT_EQ(nullptr, firstAircraft->Previous());
        EXPECT_EQ(secondAircraft, firstAircraft->Next());
        EXPECT_EQ(firstAircraft, secondAircraft->Previous());

        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::WakeTurbulence);
        auto thirdAircraft = sequencer.Get("BAW789");
        EXPECT_EQ("BAW789", thirdAircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, thirdAircraft->Mode());
        EXPECT_EQ(firstAircraft, secondAircraft->Previous());
        EXPECT_EQ(thirdAircraft, secondAircraft->Next());
        EXPECT_EQ(secondAircraft, thirdAircraft->Previous());
        EXPECT_EQ(nullptr, thirdAircraft->Next());
    }

    TEST_F(ApproachSequencerTest, ItAddsAircraftBeforeAnotherInSequenceIfSequenceEmpty)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum, "BAW456");
        auto firstAircraft = sequencer.First();
        EXPECT_EQ("BAW123", firstAircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::Minimum, firstAircraft->Mode());
        EXPECT_EQ(nullptr, firstAircraft->Previous());
        EXPECT_EQ(nullptr, firstAircraft->Next());

        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::WakeTurbulence);
        EXPECT_EQ(firstAircraft, sequencer.First());
        auto secondAircraft = sequencer.Get("BAW456");
        EXPECT_EQ("BAW456", secondAircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, secondAircraft->Mode());
        EXPECT_EQ(nullptr, firstAircraft->Previous());
        EXPECT_EQ(secondAircraft, firstAircraft->Next());
        EXPECT_EQ(firstAircraft, secondAircraft->Previous());

        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::WakeTurbulence);
        auto thirdAircraft = sequencer.Get("BAW789");
        EXPECT_EQ("BAW789", thirdAircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, thirdAircraft->Mode());
        EXPECT_EQ(firstAircraft, secondAircraft->Previous());
        EXPECT_EQ(thirdAircraft, secondAircraft->Next());
        EXPECT_EQ(secondAircraft, thirdAircraft->Previous());
        EXPECT_EQ(nullptr, thirdAircraft->Next());
    }

    TEST_F(ApproachSequencerTest, ItAddsAircraftBeforeAnotherInSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        auto firstAircraft = sequencer.First();
        EXPECT_EQ("BAW123", firstAircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::Minimum, firstAircraft->Mode());
        EXPECT_EQ(nullptr, firstAircraft->Previous());
        EXPECT_EQ(nullptr, firstAircraft->Next());

        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::WakeTurbulence, "BAW123");
        auto secondAircraft = sequencer.Get("BAW456");
        EXPECT_EQ(secondAircraft, sequencer.First());
        EXPECT_EQ("BAW456", secondAircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, secondAircraft->Mode());
        EXPECT_EQ(secondAircraft, firstAircraft->Previous());
        EXPECT_EQ(firstAircraft, secondAircraft->Next());

        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::WakeTurbulence, "BAW123");
        EXPECT_EQ(secondAircraft, sequencer.First());
        auto thirdAircraft = sequencer.Get("BAW789");
        EXPECT_EQ("BAW789", thirdAircraft->Callsign());
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, thirdAircraft->Mode());
        EXPECT_EQ(nullptr, secondAircraft->Previous());
        EXPECT_EQ(thirdAircraft, secondAircraft->Next());
        EXPECT_EQ(secondAircraft, thirdAircraft->Previous());
        EXPECT_EQ(firstAircraft, thirdAircraft->Next());
        EXPECT_EQ(thirdAircraft, firstAircraft->Previous());
        EXPECT_EQ(nullptr, firstAircraft->Next());
    }

    TEST_F(ApproachSequencerTest, ItRemovesAnAircraftSingleSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.RemoveAircraft("BAW123");
        EXPECT_EQ(nullptr, sequencer.Get("BAW123"));
    }

    TEST_F(ApproachSequencerTest, ItRemovesAnAircraftMidSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);

        sequencer.RemoveAircraft("BAW456");
        EXPECT_EQ(nullptr, sequencer.Get("BAW456"));
        EXPECT_EQ(sequencer.Get("BAW789"), sequencer.Get("BAW123")->Next());
        EXPECT_EQ(sequencer.Get("BAW123"), sequencer.Get("BAW789")->Previous());
    }

    TEST_F(ApproachSequencerTest, ItRemovesAnAircraftEndOfSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);

        sequencer.RemoveAircraft("BAW789");
        EXPECT_EQ(nullptr, sequencer.Get("BAW789"));
        EXPECT_EQ(nullptr, sequencer.Get("BAW456")->Next());
    }

    TEST_F(ApproachSequencerTest, ItRemovesAnAircraftStartOfSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);

        sequencer.RemoveAircraft("BAW123");
        EXPECT_EQ(nullptr, sequencer.Get("BAW123"));
        EXPECT_EQ(sequencer.Get("BAW456"), sequencer.First());
        EXPECT_EQ(nullptr, sequencer.Get("BAW456")->Previous());
    }

    TEST_F(ApproachSequencerTest, ItHandlesRemoveAircraftNotInSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);

        EXPECT_NO_THROW(sequencer.RemoveAircraft("BAW999"));
    }
} // namespace UKControllerPluginTest::Approach
