#include "approach/ApproachSequencedAircraft.h"
#include "approach/ApproachSequence.h"

using UKControllerPlugin::Approach::ApproachSequence;
using UKControllerPlugin::Approach::ApproachSequencingMode;

namespace UKControllerPluginTest::Approach {
    class ApproachSequenceTest : public testing::Test
    {
        public:
        ApproachSequence sequencer;
    };

    TEST_F(ApproachSequenceTest, SequenceStartsEmpty)
    {
        EXPECT_EQ(nullptr, sequencer.First());
    }

    TEST_F(ApproachSequenceTest, GetReturnsNullptrAircraftNotFound)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        EXPECT_EQ(nullptr, sequencer.Get("BAW456"));
    }

    TEST_F(ApproachSequenceTest, ItAddsAircraftToTheBackOfTheSequence)
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
        
        EXPECT_EQ(std::list<std::string>({"BAW123", "BAW456", "BAW789"}), sequencer.Callsigns());
    }

    TEST_F(ApproachSequenceTest, ItAddsAircraftBeforeAnotherInSequenceIfSequenceEmpty)
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
        
        EXPECT_EQ(std::list<std::string>({"BAW123", "BAW456", "BAW789"}), sequencer.Callsigns());
    }

    TEST_F(ApproachSequenceTest, ItAddsAircraftBeforeAnotherInSequence)
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
        
        EXPECT_EQ(std::list<std::string>({"BAW456", "BAW789", "BAW123"}), sequencer.Callsigns());
    }

    TEST_F(ApproachSequenceTest, ItRemovesAnAircraftSingleSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.RemoveAircraft("BAW123");
        EXPECT_EQ(nullptr, sequencer.Get("BAW123"));
        
        EXPECT_EQ(std::list<std::string>({}), sequencer.Callsigns());
    }

    TEST_F(ApproachSequenceTest, ItRemovesAnAircraftMidSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);

        sequencer.RemoveAircraft("BAW456");
        EXPECT_EQ(nullptr, sequencer.Get("BAW456"));
        EXPECT_EQ(sequencer.Get("BAW789"), sequencer.Get("BAW123")->Next());
        EXPECT_EQ(sequencer.Get("BAW123"), sequencer.Get("BAW789")->Previous());
        
        EXPECT_EQ(std::list<std::string>({"BAW123", "BAW789"}), sequencer.Callsigns());
    }

    TEST_F(ApproachSequenceTest, ItRemovesAnAircraftEndOfSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);

        sequencer.RemoveAircraft("BAW789");
        EXPECT_EQ(nullptr, sequencer.Get("BAW789"));
        EXPECT_EQ(nullptr, sequencer.Get("BAW456")->Next());
        
        EXPECT_EQ(std::list<std::string>({"BAW123", "BAW456"}), sequencer.Callsigns());
    }

    TEST_F(ApproachSequenceTest, ItRemovesAnAircraftStartOfSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);

        sequencer.RemoveAircraft("BAW123");
        EXPECT_EQ(nullptr, sequencer.Get("BAW123"));
        EXPECT_EQ(sequencer.Get("BAW456"), sequencer.First());
        EXPECT_EQ(nullptr, sequencer.Get("BAW456")->Previous());
        
        EXPECT_EQ(std::list<std::string>({"BAW456", "BAW789"}), sequencer.Callsigns());
    }

    TEST_F(ApproachSequenceTest, ItHandlesRemoveAircraftNotInSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);

        EXPECT_NO_THROW(sequencer.RemoveAircraft("BAW999"));
    }
    
    TEST_F(ApproachSequenceTest, ItDoesntMoveAircraftUpSequenceIfTop)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW112", ApproachSequencingMode::Minimum);
        sequencer.MoveAircraftUp("BAW123");
        
        EXPECT_EQ("BAW123", sequencer.First()->Callsign());
        EXPECT_EQ(nullptr, sequencer.First()->Previous());
        EXPECT_EQ(sequencer.Get("BAW456"), sequencer.First()->Next());
        EXPECT_EQ(sequencer.First(), sequencer.Get("BAW456")->Previous());
        EXPECT_EQ(std::list<std::string>({"BAW123", "BAW456", "BAW789", "BAW112"}), sequencer.Callsigns());
    }
    
    TEST_F(ApproachSequenceTest, ItDoesntMoveAircraftUpSequenceIfNotInSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW112", ApproachSequencingMode::Minimum);
        sequencer.MoveAircraftUp("BAW111");
        
        EXPECT_EQ("BAW123", sequencer.First()->Callsign());
        EXPECT_EQ(nullptr, sequencer.First()->Previous());
        EXPECT_EQ(sequencer.Get("BAW456"), sequencer.First()->Next());
        EXPECT_EQ(sequencer.First(), sequencer.Get("BAW456")->Previous());
        EXPECT_EQ(std::list<std::string>({"BAW123", "BAW456", "BAW789", "BAW112"}), sequencer.Callsigns());
    }
    
    TEST_F(ApproachSequenceTest, ItMovesSecondInSequenceToTop)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW112", ApproachSequencingMode::Minimum);
        sequencer.MoveAircraftUp("BAW456");
        
        EXPECT_EQ("BAW456", sequencer.First()->Callsign());
        EXPECT_EQ(nullptr, sequencer.First()->Previous());
        EXPECT_EQ(sequencer.Get("BAW123"), sequencer.First()->Next());
        
        EXPECT_EQ(sequencer.Get("BAW456"), sequencer.Get("BAW123")->Previous());
        EXPECT_EQ(sequencer.Get("BAW789"), sequencer.Get("BAW123")->Next());
        EXPECT_EQ(sequencer.Get("BAW123"), sequencer.Get("BAW789")->Previous());
        EXPECT_EQ(std::list<std::string>({"BAW456", "BAW123", "BAW789", "BAW112"}), sequencer.Callsigns());
    }
    
    TEST_F(ApproachSequenceTest, ItMovesThirdInSequenceUp)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW112", ApproachSequencingMode::Minimum);
        sequencer.MoveAircraftUp("BAW789");
        
        EXPECT_EQ("BAW123", sequencer.First()->Callsign());
        EXPECT_EQ(nullptr, sequencer.First()->Previous());
        EXPECT_EQ(sequencer.Get("BAW789"), sequencer.First()->Next());
        
        EXPECT_EQ(sequencer.Get("BAW123"), sequencer.Get("BAW789")->Previous());
        EXPECT_EQ(sequencer.Get("BAW456"), sequencer.Get("BAW789")->Next());
        EXPECT_EQ(sequencer.Get("BAW789"), sequencer.Get("BAW456")->Previous());
        EXPECT_EQ(sequencer.Get("BAW112"), sequencer.Get("BAW456")->Next());
        EXPECT_EQ(sequencer.Get("BAW456"), sequencer.Get("BAW112")->Previous());
        
        EXPECT_EQ(std::list<std::string>({"BAW123", "BAW789", "BAW456", "BAW112"}), sequencer.Callsigns());
    }
    
    TEST_F(ApproachSequenceTest, ItDoesntMoveAircraftDownSequenceIfBottom)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW112", ApproachSequencingMode::Minimum);
        sequencer.MoveAircraftDown("BAW112");
        
        EXPECT_EQ(sequencer.Get("BAW789"), sequencer.Get("BAW112")->Previous());
        EXPECT_EQ(sequencer.Get("BAW112"), sequencer.Get("BAW789")->Next());
        EXPECT_EQ(std::list<std::string>({"BAW123", "BAW456", "BAW789", "BAW112"}), sequencer.Callsigns());
    }
    
    TEST_F(ApproachSequenceTest, ItDoesntMoveAircraftDownSequenceIfNotInSequence)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW112", ApproachSequencingMode::Minimum);
        sequencer.MoveAircraftDown("BAW111");
        
        EXPECT_EQ(sequencer.Get("BAW789"), sequencer.Get("BAW112")->Previous());
        EXPECT_EQ(sequencer.Get("BAW112"), sequencer.Get("BAW789")->Next());
        
        EXPECT_EQ(std::list<std::string>({"BAW123", "BAW456", "BAW789", "BAW112"}), sequencer.Callsigns());
    }
    
    TEST_F(ApproachSequenceTest, ItMovesSecondToLastInSequenceToBottom)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW112", ApproachSequencingMode::Minimum);
        sequencer.MoveAircraftDown("BAW789");
        
        EXPECT_EQ("BAW123", sequencer.First()->Callsign());
        EXPECT_EQ(nullptr, sequencer.First()->Previous());
        EXPECT_EQ(sequencer.Get("BAW456"), sequencer.First()->Next());
        
        EXPECT_EQ(nullptr, sequencer.Get("BAW789")->Next());
        EXPECT_EQ(sequencer.Get("BAW112"), sequencer.Get("BAW789")->Previous());
        EXPECT_EQ(sequencer.Get("BAW789"), sequencer.Get("BAW112")->Next());
        EXPECT_EQ(sequencer.Get("BAW456"), sequencer.Get("BAW112")->Previous());
        EXPECT_EQ(sequencer.Get("BAW112"), sequencer.Get("BAW456")->Next());
        
        EXPECT_EQ(std::list<std::string>({"BAW123", "BAW456", "BAW112", "BAW789"}), sequencer.Callsigns());
    }
    
    TEST_F(ApproachSequenceTest, ItMovesThirdToLastInSequenceDown)
    {
        sequencer.AddAircraftToSequence("BAW123", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW456", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW789", ApproachSequencingMode::Minimum);
        sequencer.AddAircraftToSequence("BAW112", ApproachSequencingMode::Minimum);
        sequencer.MoveAircraftDown("BAW456");
        
        EXPECT_EQ("BAW123", sequencer.First()->Callsign());
        EXPECT_EQ(nullptr, sequencer.First()->Previous());
        EXPECT_EQ(sequencer.Get("BAW789"), sequencer.First()->Next());
        
        EXPECT_EQ(sequencer.Get("BAW123"), sequencer.Get("BAW789")->Previous());
        EXPECT_EQ(sequencer.Get("BAW789"), sequencer.Get("BAW123")->Next());
        EXPECT_EQ(sequencer.Get("BAW456"), sequencer.Get("BAW789")->Next());
        EXPECT_EQ(sequencer.Get("BAW789"), sequencer.Get("BAW456")->Previous());
        EXPECT_EQ(sequencer.Get("BAW112"), sequencer.Get("BAW456")->Next());
        EXPECT_EQ(sequencer.Get("BAW456"), sequencer.Get("BAW112")->Previous());
        
        EXPECT_EQ(std::list<std::string>({"BAW123", "BAW789", "BAW456", "BAW112"}), sequencer.Callsigns());
    }
} // namespace UKControllerPluginTest::Approach
