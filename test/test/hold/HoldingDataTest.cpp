#include "pch/pch.h"
#include "hold/HoldingData.h"
#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/BlockedHoldLevelRestriction.h"
#include "hold/DeemedSeparatedHold.h"

using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::AbstractHoldLevelRestriction;
using UKControllerPlugin::Hold::BlockedHoldLevelRestriction;
using UKControllerPlugin::Hold::DeemedSeparatedHold;

namespace UKControllerPluginTest {
    namespace Hold {

        TEST(HoldingDataTest, EqualityReturnsTrueIfIdentifierSame)
        {
            HoldingData hold1{ 1, "TIMBA", "TIMBA", 7001, 15001, 51, "left" };
            HoldingData hold2{ 1, "TIMBA", "TIMBA 2", 7002, 15002, 52, "right" };
            EXPECT_TRUE(hold1 == hold2);
        }

        TEST(HoldingDataTest, EqualityReturnsFalseIfIdentifierDifferent)
        {
            HoldingData hold1{ 1, "TIMBA", "TIMBA", 7001, 15001, 51, "left" };
            HoldingData hold2{ 2, "TIMBA", "TIMBA", 7002, 15002, 52, "right" };
            EXPECT_FALSE(hold1 == hold2);
        }

        TEST(HoldingDataTest, ItDefaultConstructs)
        {
            HoldingData hold;
            EXPECT_EQ(0, hold.identifier);
            EXPECT_EQ("INVALID", hold.fix);
            EXPECT_EQ("INVALID", hold.description);
            EXPECT_EQ(0, hold.minimum);
            EXPECT_EQ(0, hold.maximum);
            EXPECT_EQ(361, hold.inbound);
            EXPECT_EQ("up", hold.turnDirection);
            EXPECT_EQ(0, hold.restrictions.size());
            EXPECT_EQ(0, hold.deemedSeparatedHolds.size());
        }

        TEST(HoldingDataTest, ItCanBeMoveConstructed)
        {
            std::set<std::unique_ptr<AbstractHoldLevelRestriction>> restrictions;
            restrictions.emplace(new BlockedHoldLevelRestriction({ 7000, 8000 }));

            std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparatedHolds;
            deemedSeparatedHolds.emplace(new DeemedSeparatedHold(1, 2));
            HoldingData hold{ 1, "TIMBA", "TIMBA LOW", 7001, 15001, 51, "left", std::move(restrictions), std::move(deemedSeparatedHolds) };

            HoldingData hold2(std::move(hold));

            EXPECT_EQ(1, hold2.identifier);
            EXPECT_EQ("TIMBA", hold2.fix);
            EXPECT_EQ("TIMBA LOW", hold2.description);
            EXPECT_EQ(7001, hold2.minimum);
            EXPECT_EQ(15001, hold2.maximum);
            EXPECT_EQ(51, hold2.inbound);
            EXPECT_EQ("left", hold2.turnDirection);
            EXPECT_EQ(1, hold2.restrictions.size());

            BlockedHoldLevelRestriction * actualCast =
                dynamic_cast<BlockedHoldLevelRestriction *>(hold2.restrictions.cbegin()->get());

            std::set<unsigned int> expectedLevels = { 7000, 8000 };
            EXPECT_EQ(expectedLevels, actualCast->GetLevels());

            DeemedSeparatedHold * deemedSeparated =
                dynamic_cast<DeemedSeparatedHold *>(hold2.deemedSeparatedHolds.cbegin()->get());

            EXPECT_EQ(1, deemedSeparated->identifier);
        }

        TEST(HoldingDataTest, ItCanBeMoveAssigned)
        {
            std::set<std::unique_ptr<AbstractHoldLevelRestriction>> restrictions;
            restrictions.emplace(new BlockedHoldLevelRestriction({ 7000, 8000 }));

            std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparatedHolds;
            deemedSeparatedHolds.emplace(new DeemedSeparatedHold(1, 2));
            HoldingData hold{ 1, "TIMBA", "TIMBA LOW", 7001, 15001, 51, "left", std::move(restrictions), std::move(deemedSeparatedHolds) };

            HoldingData hold2 = std::move(hold);

            EXPECT_EQ(1, hold2.identifier);
            EXPECT_EQ("TIMBA", hold2.fix);
            EXPECT_EQ("TIMBA LOW", hold2.description);
            EXPECT_EQ(7001, hold2.minimum);
            EXPECT_EQ(15001, hold2.maximum);
            EXPECT_EQ(51, hold2.inbound);
            EXPECT_EQ("left", hold2.turnDirection);
            EXPECT_EQ(1, hold2.restrictions.size());

            BlockedHoldLevelRestriction * actualCast =
                dynamic_cast<BlockedHoldLevelRestriction *>(hold2.restrictions.cbegin()->get());

            std::set<unsigned int> expectedLevels = { 7000, 8000 };
            EXPECT_EQ(expectedLevels, actualCast->GetLevels());

            DeemedSeparatedHold * deemedSeparated =
                dynamic_cast<DeemedSeparatedHold *>(hold2.deemedSeparatedHolds.cbegin()->get());

            EXPECT_EQ(1, deemedSeparated->identifier);
        }

        TEST(HoldingDataTest, LevelWithinHoldReturnsFalseBelowLowerBand)
        {
            HoldingData hold{1, "TIMBA", "TIMBA", 7000, 15000, 51, "left"};
            EXPECT_FALSE(hold.LevelWithinHold(6999));
        }

        TEST(HoldingDataTest, LevelWithinHoldReturnsTrueLowerBound)
        {
            HoldingData hold{1, "TIMBA", "TIMBA", 7000, 15000, 51, "left"};
            EXPECT_TRUE(hold.LevelWithinHold(7000));
        }

        TEST(HoldingDataTest, LevelWithinHoldReturnsTrueAboveLowerBound)
        {
            HoldingData hold{1, "TIMBA", "TIMBA", 7000, 15000, 51, "left"};
            EXPECT_TRUE(hold.LevelWithinHold(7001));
        }

        TEST(HoldingDataTest, LevelWithinHoldReturnsTrueMiddle)
        {
            HoldingData hold{1, "TIMBA", "TIMBA", 7000, 15000, 51, "left"};
            EXPECT_TRUE(hold.LevelWithinHold(11000));
        }

        TEST(HoldingDataTest, LevelWithinHoldReturnsTrueBelowUpperBand)
        {
            HoldingData hold{1, "TIMBA", "TIMBA", 7000, 15000, 51, "left"};
            EXPECT_TRUE(hold.LevelWithinHold(14999));
        }

        TEST(HoldingDataTest, LevelWithinHoldReturnsTrueUpperBand)
        {
            HoldingData hold{1, "TIMBA", "TIMBA", 7000, 15000, 51, "left"};
            EXPECT_TRUE(hold.LevelWithinHold(15000));
        }

        TEST(HoldingDataTest, LevelWithinHoldReturnsFalseAboveUpperBand)
        {
            HoldingData hold{1, "TIMBA", "TIMBA", 7000, 15000, 51, "left"};
            EXPECT_FALSE(hold.LevelWithinHold(15001));
        }
    } // namespace Hold
}  // namespace UKControllerPluginTest
