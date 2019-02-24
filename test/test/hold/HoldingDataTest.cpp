#include "pch/pch.h"
#include "hold/HoldingData.h"

using UKControllerPlugin::Hold::HoldingData;

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

    }  // namespace Hold
}  // namespace UKControllerPluginTest
