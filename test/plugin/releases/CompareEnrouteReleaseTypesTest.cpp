#include "releases/EnrouteReleaseType.h"
#include "releases/CompareEnrouteReleaseTypes.h"

using ::testing::Test;
using UKControllerPlugin::Releases::CompareEnrouteReleaseTypes;
using UKControllerPlugin::Releases::EnrouteReleaseType;

namespace UKControllerPluginTest {
    namespace Releases {

        class CompareEnrouteReleaseTypesTest : public Test
        {
            public:
            CompareEnrouteReleaseTypes compare;
        };

        TEST_F(CompareEnrouteReleaseTypesTest, LessThanIntReturnsTrueIfLessThan)
        {
            EXPECT_TRUE(compare(1, {2, "test", "testdesc"}));
        }

        TEST_F(CompareEnrouteReleaseTypesTest, LessThanStructReturnsTrueIfLessThan)
        {
            EXPECT_TRUE(compare({1, "test", "testdesc"}, 2));
        }

        TEST_F(CompareEnrouteReleaseTypesTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            EXPECT_TRUE(compare({1, "test", "testdesc"}, {2, "test", "testdesc"}));
        }
    } // namespace Releases
} // namespace UKControllerPluginTest
