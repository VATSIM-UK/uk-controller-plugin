#include "pch/pch.h"
#include "regional/RegionalPressureRendererConfiguration.h"
#include "regional/RegionalPressureRenderedItem.h"

using UKControllerPlugin::Regional::RegionalPressureRenderedItem;
using UKControllerPlugin::Regional::RegionalPressureRendererConfiguration;
using testing::Test;

namespace UKControllerPluginTest {
    namespace Regional {

        class RegionalPressureRendererConfigurationTest : public Test
        {
            public:

                RegionalPressureRenderedItem item1{ 1, "ASR_LONDON" };
                RegionalPressureRenderedItem item2{ 2, "ASR_SCOTTISH" };
                RegionalPressureRendererConfiguration configuration;
        };


        TEST_F(RegionalPressureRendererConfigurationTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->configuration.CountItems());
        }

        TEST_F(RegionalPressureRendererConfigurationTest, ItAddsAnItem)
        {
            this->configuration.AddItem(this->item1);
            this->configuration.AddItem(this->item2);
            EXPECT_EQ(2, this->configuration.CountItems());
        }

        TEST_F(RegionalPressureRendererConfigurationTest, ItDoesntAddDuplicates)
        {
            this->configuration.AddItem(this->item1);
            this->configuration.AddItem(this->item1);
            EXPECT_EQ(1, this->configuration.CountItems());
        }

        TEST_F(RegionalPressureRendererConfigurationTest, ItRemovesItems)
        {
            this->configuration.AddItem(this->item1);
            this->configuration.AddItem(this->item2);
            this->configuration.RemoveItem(this->item1);
            EXPECT_EQ(this->item2, *this->configuration.cbegin());
        }

        TEST_F(RegionalPressureRendererConfigurationTest, ItRemovesItemsByOrder)
        {
            this->configuration.AddItem(this->item1);
            this->configuration.AddItem(this->item2);
            this->configuration.RemoveItem(1);
            EXPECT_EQ(this->item2, *this->configuration.cbegin());
        }

        TEST_F(RegionalPressureRendererConfigurationTest, ItHandlesNonExistantItems)
        {
            EXPECT_NO_THROW(this->configuration.RemoveItem(55));
        }

        TEST_F(RegionalPressureRendererConfigurationTest, ItCanIterateItemsInOrder)
        {
            this->configuration.AddItem(this->item1);
            this->configuration.AddItem(this->item2);
            auto it = this->configuration.cbegin();
            EXPECT_EQ(this->item1, *it++);
            EXPECT_EQ(this->item2, *it);
        }

        TEST_F(RegionalPressureRendererConfigurationTest, ItReturnsInvalidItem)
        {
            this->configuration.AddItem(this->item1);
            this->configuration.AddItem(this->item2);
            EXPECT_EQ(this->configuration.invalidItem, this->configuration.GetItem("foo"));
        }

        TEST_F(RegionalPressureRendererConfigurationTest, ItReturnsItemsByKey)
        {
            this->configuration.AddItem(this->item1);
            this->configuration.AddItem(this->item2);
            EXPECT_EQ(this->item1, this->configuration.GetItem("ASR_LONDON"));
        }
    }  // namespace Regional
}  // namespace UKControllerPluginTest
