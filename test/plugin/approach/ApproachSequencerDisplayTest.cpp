#include "approach/ApproachSequence.h"
#include "approach/ApproachSequencer.h"
#include "approach/ApproachSequencerDisplay.h"
#include "approach/ApproachSequencerDisplayOptions.h"

using UKControllerPlugin::Approach::ApproachSequencer;
using UKControllerPlugin::Approach::ApproachSequencerDisplay;
using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;
using UKControllerPlugin::Approach::ApproachSequencingMode;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerDisplayTest : public testing::Test
    {
        public:
        ApproachSequencerDisplayTest()
            : selectorList(std::make_shared<List::MockPopupList>()),
              callsignSelectorList(std::make_shared<List::MockPopupList>()),
              options(std::make_shared<ApproachSequencerDisplayOptions>()),
              display(sequencer, options, selectorList, callsignSelectorList, 55)
        {
        }

        std::shared_ptr<List::MockPopupList> selectorList;
        std::shared_ptr<List::MockPopupList> callsignSelectorList;
        testing::NiceMock<Euroscope::MockEuroscopeRadarScreenLoopbackInterface> radarScreen;
        ApproachSequencer sequencer;
        std::shared_ptr<ApproachSequencerDisplayOptions> options;
        ApproachSequencerDisplay display;
    };

    TEST_F(ApproachSequencerDisplayTest, ItTogglesContentCollapsing)
    {
        EXPECT_FALSE(options->ContentCollapsed());
        display.LeftClick(radarScreen, 1, "collapseButton", {}, {});
        EXPECT_TRUE(options->ContentCollapsed());
        display.LeftClick(radarScreen, 1, "collapseButton", {}, {});
        EXPECT_FALSE(options->ContentCollapsed());
    }

    TEST_F(ApproachSequencerDisplayTest, ItCanBeClosed)
    {
        options->SetVisible(true);
        display.LeftClick(radarScreen, 1, "closeButton", {}, {});
        EXPECT_FALSE(display.IsVisible());
    }

    TEST_F(ApproachSequencerDisplayTest, ItTriggersAirfieldList)
    {
        EXPECT_CALL(*selectorList, Trigger(PointEq(POINT{1, 2}))).Times(1);

        display.LeftClick(radarScreen, 1, "aircraftSelector", {1, 2}, {});
    }

    TEST_F(ApproachSequencerDisplayTest, ItTriggersCallsignList)
    {
        EXPECT_CALL(*callsignSelectorList, Trigger(PointEq(POINT{1, 2}))).Times(1);

        display.LeftClick(radarScreen, 1, "addAircraft", {1, 2}, {});
    }

    TEST_F(ApproachSequencerDisplayTest, ItMovesAnAircraftUp)
    {
        options->Airfield("EGKK");
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGKK", "BAW456", ApproachSequencingMode::WakeTurbulence);

        display.LeftClick(radarScreen, 1, "moveUpBAW456", {1, 2}, {});
        EXPECT_EQ(std::list<std::string>({"BAW456", "BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
    }

    TEST_F(ApproachSequencerDisplayTest, ItMovesAnAircraftDown)
    {
        options->Airfield("EGKK");
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGKK", "BAW456", ApproachSequencingMode::WakeTurbulence);

        display.LeftClick(radarScreen, 1, "moveDownBAW123", {1, 2}, {});
        EXPECT_EQ(std::list<std::string>({"BAW456", "BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
    }

    TEST_F(ApproachSequencerDisplayTest, ItRemovesAnAircraft)
    {
        options->Airfield("EGKK");
        sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
        sequencer.AddAircraftToSequence("EGKK", "BAW456", ApproachSequencingMode::WakeTurbulence);

        display.LeftClick(radarScreen, 1, "deleteButtonBAW123", {1, 2}, {});
        EXPECT_EQ(std::list<std::string>({"BAW456"}), sequencer.GetForAirfield("EGKK").Callsigns());
    }
} // namespace UKControllerPluginTest::Approach
