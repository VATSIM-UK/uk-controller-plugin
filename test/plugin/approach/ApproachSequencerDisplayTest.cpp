#include "approach/ApproachSpacingCalculator.h"
#include "approach/ApproachSequence.h"
#include "approach/ApproachSequencer.h"
#include "approach/ApproachSequencerDisplay.h"
#include "approach/ApproachSequencerDisplayOptions.h"
#include "airfield/AirfieldCollection.h"
#include "wake/WakeCategoryMapperCollection.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Approach::ApproachSequencer;
using UKControllerPlugin::Approach::ApproachSequencerDisplay;
using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;
using UKControllerPlugin::Approach::ApproachSequencingMode;
using UKControllerPlugin::Approach::ApproachSpacingCalculator;
using UKControllerPlugin::Wake::WakeCategoryMapperCollection;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerDisplayTest : public testing::Test
    {
        public:
        ApproachSequencerDisplayTest()
            : spacingCalculator(airfields, wakeMapper, plugin), selectorList(std::make_shared<List::MockPopupList>()),
              callsignSelectorList(std::make_shared<List::MockPopupList>()),
              targetList(std::make_shared<List::MockPopupList>()),
              airfieldTargetList(std::make_shared<List::MockPopupList>()),
              airfieldSeparationList(std::make_shared<List::MockPopupList>()),
              options(std::make_shared<ApproachSequencerDisplayOptions>()), display(
                                                                                sequencer,
                                                                                spacingCalculator,
                                                                                options,
                                                                                selectorList,
                                                                                callsignSelectorList,
                                                                                targetList,
                                                                                airfieldTargetList,
                                                                                airfieldSeparationList,
                                                                                plugin,
                                                                                55)
        {
        }

        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        AirfieldCollection airfields;
        WakeCategoryMapperCollection wakeMapper;
        ApproachSpacingCalculator spacingCalculator;
        std::shared_ptr<List::MockPopupList> selectorList;
        std::shared_ptr<List::MockPopupList> callsignSelectorList;
        std::shared_ptr<List::MockPopupList> targetList;
        std::shared_ptr<List::MockPopupList> airfieldTargetList;
        std::shared_ptr<List::MockPopupList> airfieldSeparationList;
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

    TEST_F(ApproachSequencerDisplayTest, ItTriggersAirfieldTargetList)
    {
        EXPECT_CALL(*airfieldTargetList, Trigger(PointEq(POINT{1, 2}))).Times(1);

        display.LeftClick(radarScreen, 1, "airfieldTarget", {1, 2}, {});
    }

    TEST_F(ApproachSequencerDisplayTest, ItTriggersAirfieldSeparationList)
    {
        EXPECT_CALL(*airfieldTargetList, Trigger(PointEq(POINT{1, 2}))).Times(1);

        display.LeftClick(radarScreen, 1, "airfieldSeparation", {1, 2}, {});
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

    TEST_F(ApproachSequencerDisplayTest, ItTriggersTargetListForAircraft)
    {
        auto flightplan = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        ON_CALL(*flightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(flightplan));

        EXPECT_CALL(plugin, SetEuroscopeSelectedFlightplanPointer(testing::_)).Times(1);

        EXPECT_CALL(*targetList, Trigger(PointEq(POINT{1, 2}))).Times(1);

        display.LeftClick(radarScreen, 1, "approachTargetBAW123", {1, 2}, {});
    }

    TEST_F(ApproachSequencerDisplayTest, ItDoesntTriggerTargetListForAircraftIfFlightplanNotFound)
    {
        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(nullptr));

        EXPECT_CALL(plugin, SetEuroscopeSelectedFlightplanPointer(testing::_)).Times(0);

        EXPECT_CALL(*targetList, Trigger(testing::_)).Times(0);

        display.LeftClick(radarScreen, 1, "approachTargetBAW123", {1, 2}, {});
    }
} // namespace UKControllerPluginTest::Approach
