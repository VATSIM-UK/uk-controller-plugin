#include "ecfmp/AircraftFlowMeasureTagItem.h"
#include "ecfmp/AircraftFlowMeasureMapInterface.h"
#include "mock/FlowMeasureMock.h"
#include "tag/TagData.h"
#include <gmock/gmock-actions.h>
#include <gmock/gmock-nice-strict.h>

namespace UKControllerPluginTest::ECFMP {

    class MockFlowMeasureMap : public UKControllerPlugin::ECFMP::AircraftFlowMeasureMapInterface
    {
        public:
        MOCK_METHOD(
            const std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>>&,
            GetFlowMeasuresForCallsign,
            (const std::string& callsign),
            (const, override));
    };

    class AircraftFlowMeasureTagItemTest : public testing::Test
    {
        public:
        AircraftFlowMeasureTagItemTest()
            : tagData(
                  mockFlightplan,
                  mockRadarTarget,
                  1,
                  EuroScopePlugIn::TAG_DATA_CORRELATED,
                  itemString,
                  &euroscopeColourCode,
                  &tagColour,
                  &fontSize),
              mockFlowMeasure1(std::make_shared<testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock>>()),
              mockFlowMeasure2(std::make_shared<testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock>>()),
              mockFlowMeasure3(std::make_shared<testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock>>()),
              mockFlowMeasureMap(std::make_shared<testing::NiceMock<const MockFlowMeasureMap>>()),
              tagItem(mockFlowMeasureMap)
        {
            ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));
            ON_CALL(*mockFlowMeasure1, Identifier).WillByDefault(testing::ReturnRef(measure1Identifier));
            ON_CALL(*mockFlowMeasure2, Identifier).WillByDefault(testing::ReturnRef(measure2Identifier));
            ON_CALL(*mockFlowMeasure3, Identifier).WillByDefault(testing::ReturnRef(measure3Identifier));
        }

        std::string measure1Identifier = "EGTT01A";
        std::string measure2Identifier = "EGTT01B";
        std::string measure3Identifier = "EGTT01C";
        double fontSize = 24.1;
        COLORREF tagColour = RGB(255, 255, 255);
        int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
        char itemString[16] = "Foooooo";
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> mockRadarTarget;
        UKControllerPlugin::Tag::TagData tagData;
        std::shared_ptr<testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock>> mockFlowMeasure1;
        std::shared_ptr<testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock>> mockFlowMeasure2;
        std::shared_ptr<testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock>> mockFlowMeasure3;
        std::shared_ptr<testing::NiceMock<const MockFlowMeasureMap>> mockFlowMeasureMap;
        UKControllerPlugin::ECFMP::AircraftFlowMeasureTagItem tagItem;
    };

    TEST_F(AircraftFlowMeasureTagItemTest, ItHasADescription)
    {
        EXPECT_EQ(tagItem.GetTagItemDescription(0), "Relevant ECFMP Flow Measures");
    }

    TEST_F(AircraftFlowMeasureTagItemTest, ItSetsTagItemDataNoMeasures)
    {
        const auto emptyMeasures = std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>>{};
        ON_CALL(*mockFlowMeasureMap, GetFlowMeasuresForCallsign("BAW123"))
            .WillByDefault(testing::ReturnRef(emptyMeasures));

        tagItem.SetTagItemData(tagData);
        EXPECT_EQ("Foooooo", tagData.GetItemString());
    }

    TEST_F(AircraftFlowMeasureTagItemTest, ItSetsTagItemDataOneMeasure)
    {
        const auto measures =
            std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>>{mockFlowMeasure1};
        ON_CALL(*mockFlowMeasureMap, GetFlowMeasuresForCallsign("BAW123")).WillByDefault(testing::ReturnRef(measures));

        tagItem.SetTagItemData(tagData);
        EXPECT_EQ("EGTT01A", tagData.GetItemString());
    }

    TEST_F(AircraftFlowMeasureTagItemTest, ItSetsTagItemDataTwoMeasures)
    {
        const auto measures = std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>>{
            mockFlowMeasure1, mockFlowMeasure2};
        ON_CALL(*mockFlowMeasureMap, GetFlowMeasuresForCallsign("BAW123")).WillByDefault(testing::ReturnRef(measures));

        tagItem.SetTagItemData(tagData);
        EXPECT_EQ("(2)EGTT01A", tagData.GetItemString());
    }

    TEST_F(AircraftFlowMeasureTagItemTest, ItSetsTagItemDataManyMeasures)
    {
        const auto measures = std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>>{
            mockFlowMeasure1, mockFlowMeasure2, mockFlowMeasure3};
        ON_CALL(*mockFlowMeasureMap, GetFlowMeasuresForCallsign("BAW123")).WillByDefault(testing::ReturnRef(measures));

        tagItem.SetTagItemData(tagData);
        EXPECT_EQ("(3)EGTT01A", tagData.GetItemString());
    }
} // namespace UKControllerPluginTest::ECFMP
