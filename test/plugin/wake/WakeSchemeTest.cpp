#include "wake/DepartureWakeInterval.h"
#include "wake/WakeCategory.h"
#include "wake/WakeScheme.h"

using UKControllerPlugin::Wake::DepartureWakeInterval;
using UKControllerPlugin::Wake::WakeCategory;
using UKControllerPlugin::Wake::WakeScheme;

namespace UKControllerPluginTest::Wake {
    class WakeSchemeTest : public testing::Test
    {
        public:
        WakeSchemeTest()
            : categories({std::make_shared<WakeCategory>(
                  456, "LM", "Lower Medium", 21, std::list<std::shared_ptr<DepartureWakeInterval>>{})}),
              scheme(123, "RECAT", "RECAT-EU", categories)
        {
        }

        std::list<std::shared_ptr<WakeCategory>> categories;
        WakeScheme scheme;
    };

    TEST_F(WakeSchemeTest, ItHasAnId)
    {
        EXPECT_EQ(123, scheme.Id());
    }

    TEST_F(WakeSchemeTest, ItHasAKey)
    {
        EXPECT_EQ("RECAT", scheme.Key());
    }

    TEST_F(WakeSchemeTest, ItHasAName)
    {
        EXPECT_EQ("RECAT-EU", scheme.Name());
    }

    TEST_F(WakeSchemeTest, ItHasCategories)
    {
        EXPECT_EQ(categories, scheme.Categories());
    }
} // namespace UKControllerPluginTest::Wake
