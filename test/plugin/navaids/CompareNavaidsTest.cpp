#include "navaids/Navaid.h"
#include "navaids/CompareNavaids.h"

using ::testing::Test;
using UKControllerPlugin::Navaids::CompareNavaids;
using UKControllerPlugin::Navaids::Navaid;

namespace UKControllerPluginTest {
    namespace Navaids {

        class CompareNavaidsTest : public Test
        {
            public:
            CompareNavaids compare;
        };

        TEST_F(CompareNavaidsTest, LessThanStringReturnsTrueIfLessThan)
        {
            Navaid navaid = {1, "TIMBA"};
            EXPECT_TRUE(compare("LOGAN", navaid));
        }

        TEST_F(CompareNavaidsTest, LessThanStructReturnsTrueIfLessThan)
        {
            Navaid navaid = {1, "LOGAN"};
            EXPECT_TRUE(compare(navaid, "TIMBA"));
        }

        TEST_F(CompareNavaidsTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            Navaid navaid1 = {1, "LOGAN"};
            Navaid navaid2 = {1, "TIMBA"};
            EXPECT_TRUE(compare(navaid1, navaid2));
        }
    } // namespace Navaids
} // namespace UKControllerPluginTest
