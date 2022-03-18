#include "approach/AirfieldApproachOptions.h"
#include "approach/AirfieldMinimumSeparationSelectorList.h"
#include "approach/ApproachSequencerDisplayOptions.h"
#include "approach/ApproachSequencerOptions.h"
#include "list/ListItem.h"

using UKControllerPlugin::Approach::AirfieldApproachOptions;
using UKControllerPlugin::Approach::AirfieldMinimumSeparationSelectorList;
using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;
using UKControllerPlugin::Approach::ApproachSequencerOptions;
using UKControllerPlugin::Approach::ApproachSequencingMode;
using UKControllerPlugin::List::ListItemCheckedStatus;

namespace UKControllerPluginTest::Approach {
    class AirfieldMinimumSeparationSelectorListTest : public testing::Test
    {
        public:
        AirfieldMinimumSeparationSelectorListTest()
            : options(std::make_shared<ApproachSequencerDisplayOptions>()), list(sequencerOptions, options)
        {
            options->Airfield("EGKK");
        }

        void AssertListItems()
        {
            auto items = list.ListItems();
            auto distanceIterator = items.cbegin();
            const std::list<std::string> distances{
                "2.5",
                "3.0",
                "4.0",
                "5.0",
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

                auto checked = distance == "5.0" ? ListItemCheckedStatus::Checked : ListItemCheckedStatus::NotChecked;
                EXPECT_EQ(checked, distanceItem->checked);
                distanceIterator++;
            }
        }

        ApproachSequencerOptions sequencerOptions;
        std::shared_ptr<ApproachSequencerDisplayOptions> options;
        AirfieldMinimumSeparationSelectorList list;
    };

    TEST_F(AirfieldMinimumSeparationSelectorListTest, ItHasOneColumn)
    {
        EXPECT_EQ(1, list.ListColumns());
    }

    TEST_F(AirfieldMinimumSeparationSelectorListTest, ItHasAName)
    {
        EXPECT_EQ("Separation", list.ListName());
    }

    TEST_F(AirfieldMinimumSeparationSelectorListTest, ItReturnsEmptyListIfNoAirfieldSelected)
    {
        options->Airfield("");
        EXPECT_TRUE(list.ListItems().empty());
    }

    TEST_F(AirfieldMinimumSeparationSelectorListTest, ItReturnsListWithMinimumDistanceTicked)
    {
        sequencerOptions.Set(
            "EGKK", std::make_shared<AirfieldApproachOptions>(ApproachSequencingMode::MinimumDistance, 6.0, 5.0));

        this->AssertListItems();
    }

    TEST_F(AirfieldMinimumSeparationSelectorListTest, ItDoesntSelectIfNoAirfield)
    {
        options->Airfield("");
        list.ItemSelected("4.0");

        EXPECT_EQ(0, sequencerOptions.CountAirfieldOptions());
    }

    TEST_F(AirfieldMinimumSeparationSelectorListTest, ItSetsASeparationRequirement)
    {
        list.ItemSelected("4.12");

        EXPECT_TRUE(sequencerOptions.HasAirfield("EGKK"));
        const auto& options = sequencerOptions.Get("EGKK");
        EXPECT_DOUBLE_EQ(4.12, options.minimumSeparationRequirement);
    }
} // namespace UKControllerPluginTest::Approach
