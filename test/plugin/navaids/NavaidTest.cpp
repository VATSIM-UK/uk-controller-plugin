#pragma once
#include "pch/pch.h"
#include "navaids/Navaid.h"
#include "navaids/CompareNavaids.h"

using UKControllerPlugin::Navaids::Navaid;
using UKControllerPlugin::Navaids::CompareNavaids;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Navaids {

        class NavaidTest : public Test
        {
            public:
                Navaid navaid1 = { 1, "TIMBA" };
                Navaid navaid1a = { 1, "WILLO" };
                Navaid navaid2 = { 2, "TIMBA" };
            CompareNavaids compare;
        };

        TEST_F(NavaidTest, EqualityReturnsTrueIfIdentifierMatches)
        {
            EXPECT_TRUE(navaid1 == navaid2);
        }

        TEST_F(NavaidTest, EqualityReturnsFalseIfIdentifierDoesntMatch)
        {
            EXPECT_FALSE(navaid1 == navaid1a);
        }
    }  // namespace Navaids
}  // namespace UKControllerPluginTest
