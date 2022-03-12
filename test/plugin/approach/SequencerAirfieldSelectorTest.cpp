#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "approach/ApproachSequencerDisplayOptions.h"
#include "approach/SequencerAirfieldSelector.h"
#include "controller/ControllerPositionHierarchy.h"
#include "list/ListItem.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;
using UKControllerPlugin::Approach::SequencerAirfieldSelector;
using UKControllerPlugin::List::ListItemCheckedStatus;

namespace UKControllerPluginTest::Approach {
    class SequencerAirfieldSelectorTest : public testing::Test
    {
        public:
        SequencerAirfieldSelectorTest()
            : options(std::make_shared<ApproachSequencerDisplayOptions>()), selector(options, airfields)
        {
            airfields.AddAirfield(std::make_shared<AirfieldModel>(1, "EGKK", nullptr));
            airfields.AddAirfield(std::make_shared<AirfieldModel>(2, "EGBB", nullptr));
            airfields.AddAirfield(std::make_shared<AirfieldModel>(3, "EGLL", nullptr));
        }

        AirfieldCollection airfields;
        std::shared_ptr<ApproachSequencerDisplayOptions> options;
        SequencerAirfieldSelector selector;
    };

    TEST_F(SequencerAirfieldSelectorTest, ItHasOneColumn)
    {
        EXPECT_EQ(1, selector.ListColumns());
    }

    TEST_F(SequencerAirfieldSelectorTest, ItHasAListName)
    {
        EXPECT_EQ("Select Airfield", selector.ListName());
    }

    TEST_F(SequencerAirfieldSelectorTest, ItHasListItems)
    {
        const auto items = selector.ListItems();
        EXPECT_EQ(3, items.size());

        auto itemIterator = items.cbegin();
        const auto item1 = *itemIterator++;
        EXPECT_EQ("EGBB", item1->firstColumn);
        EXPECT_EQ("", item1->secondColumn);
        EXPECT_FALSE(item1->fixedPosition);
        EXPECT_FALSE(item1->disabled);
        EXPECT_EQ(ListItemCheckedStatus::NoCheckbox, item1->checked);

        const auto item2 = *itemIterator++;
        EXPECT_EQ("EGKK", item2->firstColumn);
        EXPECT_EQ("", item2->secondColumn);
        EXPECT_FALSE(item2->fixedPosition);
        EXPECT_FALSE(item2->disabled);
        EXPECT_EQ(ListItemCheckedStatus::NoCheckbox, item2->checked);

        const auto item3 = *itemIterator++;
        EXPECT_EQ("EGLL", item3->firstColumn);
        EXPECT_EQ("", item3->secondColumn);
        EXPECT_FALSE(item3->fixedPosition);
        EXPECT_FALSE(item3->disabled);
        EXPECT_EQ(ListItemCheckedStatus::NoCheckbox, item3->checked);
    }

    TEST_F(SequencerAirfieldSelectorTest, ItSelectsAnAirfield)
    {
        selector.ItemSelected("EGBB");
        EXPECT_EQ("EGBB", options->Airfield());
    }
} // namespace UKControllerPluginTest::Approach
