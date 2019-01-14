#include "pch/pch.h"
#include "hold/HoldingData.h"

using UKControllerPlugin::Hold::HoldingData;

namespace UKControllerPluginTest {
    namespace Hold {

        TEST(HoldingDataTest, EqualityReturnsTrueIfIdentifierSame)
        {
            HoldingData hold1{ "TIMBA", 7001, 15001, 51, 1 };
            HoldingData hold2{ "TIMBA", 7002, 15002, 52, 0 };
            EXPECT_TRUE(hold1 == hold2);
        }

        TEST(HoldingDataTest, EqualityReturnsFalseIfIdentifierDifferent)
        {
            HoldingData hold1{ "TIMBA", 7000, 15000, 50, 0 };
            HoldingData hold2{ "LUMBA", 7000, 15000, 50, 0 };
            EXPECT_FALSE(hold1 == hold2);
        }

    }  // namespace Hold
}  // namespace UKControllerPluginTest
