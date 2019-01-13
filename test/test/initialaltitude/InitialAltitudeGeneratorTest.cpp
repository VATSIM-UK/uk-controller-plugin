#include "pch/pch.h"
#include "initialaltitude/InitialAltitudeGenerator.h"

using UKControllerPlugin::InitialAltitude::InitialAltitudeGenerator;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace InitialAltitude {

        class InitialAltitudeGeneratorTest : public Test
        {
            public:
                InitialAltitudeGenerator initial;
        };

        TEST_F(InitialAltitudeGeneratorTest, TestItStartsEmpty)
        {
            EXPECT_EQ(0, this->initial.Count());
        }

        TEST_F(InitialAltitudeGeneratorTest, TestItAddsASid)
        {
            this->initial.AddSid("EGKK", "ADMAG2X", 6000);
            EXPECT_EQ(1, this->initial.Count());
        }

        TEST_F(InitialAltitudeGeneratorTest, TestItDoesntAddDuplicates)
        {
            this->initial.AddSid("EGKK", "ADMAG2X", 6000);
            this->initial.AddSid("EGKK", "ADMAG2X", 6000);
            EXPECT_EQ(1, this->initial.Count());
        }

        TEST_F(InitialAltitudeGeneratorTest, AcceptsSameDesignatorDifferentAirfield)
        {
            this->initial.AddSid("EGKK", "ADMAG2X", 6000);
            EXPECT_NO_THROW(this->initial.AddSid("EGKK", "ADMAG1X", 6000));
        }

        TEST_F(InitialAltitudeGeneratorTest, AddSidAcceptsNonUkAirfields)
        {
            this->initial.AddSid("LXGB", "TEST1A", 6000);
            EXPECT_TRUE(this->initial.HasSid("LXGB", "TEST1A"));
        }

        TEST_F(InitialAltitudeGeneratorTest, HasSidReturnsFalseIfAirfieldNotKnown)
        {
            this->initial.AddSid("EGLL", "DET1G", 6000);
            EXPECT_FALSE(this->initial.HasSid("EGKK", "ADMAG2X"));
        }

        TEST_F(InitialAltitudeGeneratorTest, HasSidReturnsFalseIfSidNotKnown)
        {
            this->initial.AddSid("EGKK", "ADMAG1X", 6000);
            EXPECT_FALSE(this->initial.HasSid("EGKK", "ADMAG2X"));
        }

        TEST_F(InitialAltitudeGeneratorTest, HasSidReturnsTrueIfSidKnown)
        {
            this->initial.AddSid("EGKK", "ADMAG2X", 6000);
            EXPECT_TRUE(this->initial.HasSid("EGKK", "ADMAG2X"));
        }

        TEST_F(InitialAltitudeGeneratorTest, GetInitialAltitudeForDepartureThrowsExceptionIfAirfieldNotKnown)
        {
            EXPECT_THROW(this->initial.GetInitialAltitudeForDeparture("EGKK", "ADMAG2X"), std::out_of_range);
        }

        TEST_F(InitialAltitudeGeneratorTest, GetInitialAltitudeForDepartureReturnsZeroIfSidNotKnown)
        {
            EXPECT_THROW(this->initial.GetInitialAltitudeForDeparture("EGKK", "ADMAG2X"), std::out_of_range);
        }

        TEST_F(InitialAltitudeGeneratorTest, GetInitialAltitudeForDepartureReturnsInitialIfKnown)
        {
            this->initial.AddSid("EGKK", "ADMAG2X", 6000);
            EXPECT_EQ(6000, this->initial.GetInitialAltitudeForDeparture("EGKK", "ADMAG2X"));
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPluginTest
