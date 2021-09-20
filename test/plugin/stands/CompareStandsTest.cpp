#include "stands/Stand.h"
#include "stands/CompareStands.h"

using ::testing::Test;
using UKControllerPlugin::Stands::CompareStands;
using UKControllerPlugin::Stands::Stand;

namespace UKControllerPluginTest {
    namespace Stands {

        class CompareStandsTest : public Test
        {
            public:
            CompareStands compare;
        };

        TEST_F(CompareStandsTest, LessThanIntReturnsTrueIfLessThan)
        {
            EXPECT_TRUE(compare(1, {2, "test"}));
        }

        TEST_F(CompareStandsTest, LessThanStructReturnsTrueIfLessThan)
        {
            EXPECT_TRUE(compare(
                {
                    1,
                    "test",
                },
                2));
        }

        TEST_F(CompareStandsTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            EXPECT_TRUE(compare(
                {
                    1,
                    "test",
                },
                {
                    2,
                    "test",
                }));
        }
    } // namespace Stands
} // namespace UKControllerPluginTest
