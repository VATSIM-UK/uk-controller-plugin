#pragma once
#include "pch/pch.h"
#include "initialaltitude/InitialAltitudeData.h"
#include "initialaltitude/CompareInitialAltitudes.h"

using UKControllerPlugin::InitialAltitude::InitialAltitudeData;
using UKControllerPlugin::InitialAltitude::CompareInitialAltitudes;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace InitialAltitude {

        class CompareInitialAltitudesTest : public Test
        {
            public:
                CompareInitialAltitudes compare;
        };

        TEST_F(CompareInitialAltitudesTest, LessThanStringReturnsTrueIfLessThan)
        {
            InitialAltitudeData initial = { "TEST1A", 6000 };
            EXPECT_TRUE(compare(initial, "TEST1B"));
        }

        TEST_F(CompareInitialAltitudesTest, LessThanStructReturnsTrueIfLessThan)
        {
            InitialAltitudeData initial = { "TEST1B", 6000 };
            EXPECT_TRUE(compare("TEST1A", initial));
        }

        TEST_F(CompareInitialAltitudesTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            InitialAltitudeData initial1 = { "TEST1A", 6000 };
            InitialAltitudeData initial2 = { "TEST1B", 6000 };
            EXPECT_TRUE(compare(initial1, initial2));
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPluginTest
