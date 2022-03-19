#include "approach/AirfieldApproachOptions.h"
#include "approach/AirfieldTargetSelectorList.h"
#include "approach/ApproachSequencerDisplayOptions.h"
#include "approach/ApproachSequencerOptions.h"
#include "list/ListItem.h"

using UKControllerPlugin::Approach::AirfieldApproachOptions;
using UKControllerPlugin::Approach::AirfieldTargetSelectorList;
using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;
using UKControllerPlugin::Approach::ApproachSequencerOptions;
using UKControllerPlugin::Approach::ApproachSequencingMode;
using UKControllerPlugin::List::ListItemCheckedStatus;

namespace UKControllerPluginTest::Approach {
    class AirfieldTargetSelectorListTest : public testing::Test
    {
        public:
        AirfieldTargetSelectorListTest()
            : options(std::make_shared<ApproachSequencerDisplayOptions>()), list(sequencerOptions, options)
        {
            options->Airfield("EGKK");
        }

        void AssertListItems(bool wakeTicked)
        {
            auto items = list.ListItems();
            auto wakeItem = *items.cbegin();

            EXPECT_EQ("Wake", wakeItem->firstColumn);
            EXPECT_EQ("", wakeItem->secondColumn);
            EXPECT_FALSE(wakeItem->fixedPosition);
            EXPECT_FALSE(wakeItem->disabled);
            EXPECT_EQ(
                wakeTicked ? ListItemCheckedStatus::Checked : ListItemCheckedStatus::NotChecked, wakeItem->checked);

            auto distanceIterator = ++items.cbegin();
            const std::list<std::string> distances{
                "2.5",
                "3.0",
                "4.0",
                "5.0",
                "6.0",
                "7.0",
                "8.0",
                "9.0",
                "10.0",
                "11.0",
                "12.0",
                "13.0",
                "14.0",
                "15.0",
            };
            for (const auto& distance : distances) {
                auto distanceItem = *distanceIterator;
                EXPECT_EQ(distance, distanceItem->firstColumn);
                EXPECT_EQ("", distanceItem->secondColumn);
                EXPECT_FALSE(distanceItem->fixedPosition);
                EXPECT_FALSE(distanceItem->disabled);

                auto checked = !wakeTicked && distance == "5.0" ? ListItemCheckedStatus::Checked
                                                                : ListItemCheckedStatus::NotChecked;
                EXPECT_EQ(checked, distanceItem->checked);
                distanceIterator++;
            }
        }

        ApproachSequencerOptions sequencerOptions;
        std::shared_ptr<ApproachSequencerDisplayOptions> options;
        AirfieldTargetSelectorList list;
    };

    TEST_F(AirfieldTargetSelectorListTest, ItHasOneColumn)
    {
        EXPECT_EQ(1, list.ListColumns());
    }

    TEST_F(AirfieldTargetSelectorListTest, ItHasAName)
    {
        EXPECT_EQ("Target", list.ListName());
    }

    TEST_F(AirfieldTargetSelectorListTest, ItReturnsEmptyListIfNoAirfieldSelected)
    {
        options->Airfield("");
        EXPECT_TRUE(list.ListItems().empty());
    }

    TEST_F(AirfieldTargetSelectorListTest, ItReturnsListWithWakeTicked)
    {
        sequencerOptions.Set(
            "EGKK", std::make_shared<AirfieldApproachOptions>(ApproachSequencingMode::WakeTurbulence, 5.0, 6.0));
        this->AssertListItems(true);
    }

    TEST_F(AirfieldTargetSelectorListTest, ItReturnsListWithMinimumDistanceTicked)
    {
        sequencerOptions.Set(
            "EGKK", std::make_shared<AirfieldApproachOptions>(ApproachSequencingMode::MinimumDistance, 5.0, 6.0));

        this->AssertListItems(false);
    }

    TEST_F(AirfieldTargetSelectorListTest, ItDoesntSelectIfNoAirfield)
    {
        options->Airfield("");
        list.ItemSelected("Wake");

        EXPECT_EQ(0, sequencerOptions.CountAirfieldOptions());
    }

    TEST_F(AirfieldTargetSelectorListTest, ItSetsWakeMode)
    {
        list.ItemSelected("4.0");
        list.ItemSelected("Wake");

        EXPECT_TRUE(sequencerOptions.HasAirfield("EGKK"));
        const auto& options = sequencerOptions.Get("EGKK");
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, options.defaultMode);
    }

    TEST_F(AirfieldTargetSelectorListTest, ItSetsDistanceMode)
    {
        list.ItemSelected("4.0");

        EXPECT_TRUE(sequencerOptions.HasAirfield("EGKK"));
        const auto& options = sequencerOptions.Get("EGKK");
        EXPECT_EQ(ApproachSequencingMode::MinimumDistance, options.defaultMode);
        EXPECT_DOUBLE_EQ(4.0, options.targetDistance);
    }
} // namespace UKControllerPluginTest::Approach
