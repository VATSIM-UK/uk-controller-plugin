#include "approach/ApproachSequence.h"
#include "approach/ApproachSequencedAircraft.h"
#include "approach/ApproachSequencer.h"
#include "approach/ApproachSequencerDisplayOptions.h"
#include "approach/TargetSelectorList.h"
#include "list/ListItem.h"

using UKControllerPlugin::Approach::ApproachSequencer;
using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;
using UKControllerPlugin::Approach::ApproachSequencingMode;
using UKControllerPlugin::Approach::TargetSelectorList;
using UKControllerPlugin::List::ListItemCheckedStatus;

namespace UKControllerPluginTest::Approach {
    class TargetSelectorListTest : public testing::Test
    {
        public:
        TargetSelectorListTest()
            : options(std::make_shared<ApproachSequencerDisplayOptions>()), list(sequencer, options, plugin)
        {
            flightplan = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
            ON_CALL(*flightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));
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

        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> flightplan;
        ApproachSequencer sequencer;
        std::shared_ptr<ApproachSequencerDisplayOptions> options;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        TargetSelectorList list;
    };

    TEST_F(TargetSelectorListTest, ItHasOneColumn)
    {
        EXPECT_EQ(1, list.ListColumns());
    }

    TEST_F(TargetSelectorListTest, ItHasAName)
    {
        EXPECT_EQ("Target Spacing", list.ListName());
    }

    TEST_F(TargetSelectorListTest, ItReturnsEmptyListIfNoFlightplanFound)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(nullptr));
        EXPECT_TRUE(list.ListItems().empty());
    }

    TEST_F(TargetSelectorListTest, ItReturnsEmptyListIfNoAirfield)
    {
        options->Airfield("");
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(flightplan));
        EXPECT_TRUE(list.ListItems().empty());
    }

    TEST_F(TargetSelectorListTest, ItReturnsEmptyListIfAircraftNotInSequence)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(flightplan));
        EXPECT_TRUE(list.ListItems().empty());
    }

    TEST_F(TargetSelectorListTest, ItReturnsListWithWakeTicked)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.GetForAirfield("EGKK").Get("BAW123")->ExpectedDistance(5.0);

        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(flightplan));
        this->AssertListItems(true);
    }

    TEST_F(TargetSelectorListTest, ItReturnsListWithMinimumDistanceTicked)
    {
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::MinimumDistance);
        sequencer.GetForAirfield("EGKK").Get("BAW123")->ExpectedDistance(5.0);

        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(flightplan));
        this->AssertListItems(false);
    }

    TEST_F(TargetSelectorListTest, ItSelectsNothing)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(nullptr));

        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        list.ItemSelected("Wake");

        EXPECT_EQ(1, sequencer.CountSequences());
        EXPECT_EQ(std::list<std::string>({"BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
    }

    TEST_F(TargetSelectorListTest, ItSetsWakeMode)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(flightplan));

        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::MinimumDistance);
        list.ItemSelected("Wake");

        EXPECT_EQ(1, sequencer.CountSequences());
        EXPECT_EQ(std::list<std::string>({"BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, sequencer.GetForAirfield("EGKK").Get("BAW123")->Mode());
    }

    TEST_F(TargetSelectorListTest, ItSetsDistanceMode)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(flightplan));

        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        list.ItemSelected("3.0");

        EXPECT_EQ(1, sequencer.CountSequences());
        EXPECT_EQ(std::list<std::string>({"BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
        EXPECT_EQ(ApproachSequencingMode::MinimumDistance, sequencer.GetForAirfield("EGKK").Get("BAW123")->Mode());
        EXPECT_DOUBLE_EQ(3.0, sequencer.GetForAirfield("EGKK").Get("BAW123")->ExpectedDistance());
    }
} // namespace UKControllerPluginTest::Approach
