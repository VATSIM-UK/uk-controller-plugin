#include "pch/pch.h"
#include "hold/HoldingData.h"
#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/BlockedHoldLevelRestriction.h"

using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::AbstractHoldLevelRestriction;
using UKControllerPlugin::Hold::BlockedHoldLevelRestriction;

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
            std::set<std::unique_ptr<AbstractHoldLevelRestriction>> expectedRestrictions;
            EXPECT_EQ(expectedRestrictions, hold.restrictions);
        }

        TEST(HoldingDataTest, ItCanBeMoveConstructed)
        {
            std::set<std::unique_ptr<AbstractHoldLevelRestriction>> restrictions;
            restrictions.emplace(new BlockedHoldLevelRestriction({ 7000, 8000 }));
            HoldingData hold{ 1, "TIMBA", "TIMBA LOW", 7001, 15001, 51, "left", std::move(restrictions) };

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
        }

        TEST(HoldingDataTest, ItCanBeMoveAssigned)
        {
            std::set<std::unique_ptr<AbstractHoldLevelRestriction>> restrictions;
            restrictions.emplace(new BlockedHoldLevelRestriction({ 7000, 8000 }));
            HoldingData hold{ 1, "TIMBA", "TIMBA LOW", 7001, 15001, 51, "left", std::move(restrictions) };

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
        }

    }  // namespace Hold
}  // namespace UKControllerPluginTest
