#include "list/ListItem.h"
#include "wake/WakeCalculatorOptions.h"
#include "wake/WakeSchemeCollection.h"
#include "wake/WakeSchemeProvider.h"
#include "wake/WakeScheme.h"

using UKControllerPlugin::List::ListItemCheckedStatus;
using UKControllerPlugin::Wake::WakeCalculatorOptions;
using UKControllerPlugin::Wake::WakeScheme;
using UKControllerPlugin::Wake::WakeSchemeCollection;
using UKControllerPlugin::Wake::WakeSchemeProvider;

namespace UKControllerPluginTest::Wake {
    class WakeSchemeProviderTest : public testing::Test
    {
        public:
        WakeSchemeProviderTest()
            : scheme1(std::make_shared<WakeScheme>(1, "UK", "UK", std::list<std::shared_ptr<WakeCategory>>{})),
              scheme2(std::make_shared<WakeScheme>(2, "RECAT", "RECAT-EU", std::list<std::shared_ptr<WakeCategory>>{})),
              options(std::make_shared<WakeCalculatorOptions>()), provider(options, schemes, typeMapper)
        {
            schemes.Add(scheme1);
            schemes.Add(scheme2);
        }

        testing::NiceMock<Aircraft::MockAircraftTypeMapper> typeMapper;
        std::shared_ptr<WakeScheme> scheme1;
        std::shared_ptr<WakeScheme> scheme2;
        std::shared_ptr<WakeCalculatorOptions> options;
        WakeSchemeCollection schemes;
        WakeSchemeProvider provider;
    };

    TEST_F(WakeSchemeProviderTest, ItHasOneColumn)
    {
        EXPECT_EQ(1, provider.ListColumns());
    }

    TEST_F(WakeSchemeProviderTest, ItHasAName)
    {
        EXPECT_EQ("Scheme", provider.ListName());
    }

    TEST_F(WakeSchemeProviderTest, ItHasItems)
    {
        auto items = provider.ListItems();
        EXPECT_EQ(2, items.size());

        auto iterator = items.cbegin();
        auto first = *iterator++;

        EXPECT_EQ("UK", first->firstColumn);
        EXPECT_EQ("", first->secondColumn);
        EXPECT_FALSE(first->fixedPosition);
        EXPECT_FALSE(first->disabled);
        EXPECT_EQ(ListItemCheckedStatus::NoCheckbox, first->checked);

        auto second = *iterator++;

        EXPECT_EQ("RECAT-EU", second->firstColumn);
        EXPECT_EQ("", first->secondColumn);
        EXPECT_FALSE(second->fixedPosition);
        EXPECT_FALSE(second->disabled);
        EXPECT_EQ(ListItemCheckedStatus::NoCheckbox, second->checked);
    }

    TEST_F(WakeSchemeProviderTest, ItSelectsAScheme)
    {
        provider.ItemSelected("UK");
        EXPECT_EQ("UK", options->Scheme());
        EXPECT_NE(nullptr, options->SchemeMapper());
    }

    TEST_F(WakeSchemeProviderTest, ItDoesntSetSchemeIfNotFound)
    {
        provider.ItemSelected("ABC");
        EXPECT_TRUE(options->Scheme().empty());
        EXPECT_EQ(nullptr, options->SchemeMapper());
    }
} // namespace UKControllerPluginTest::Wake
