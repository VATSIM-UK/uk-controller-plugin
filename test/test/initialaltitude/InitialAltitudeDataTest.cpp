#pragma once
#include "pch/pch.h"
#include "initialaltitude/InitialAltitudeData.h"

using UKControllerPlugin::InitialAltitude::InitialAltitudeData;

namespace UKControllerPluginTest {
    namespace InitialAltitude {

        TEST(InitialAltitudeTest, TestLessThanReturnsTrueIfIdLessThan)
        {
            InitialAltitudeData initial1 = { "TEST1A", 6000 };
            InitialAltitudeData initial2 = { "TEST1B", 6000 };
            EXPECT_TRUE(initial1 < initial2);
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPluginTest
