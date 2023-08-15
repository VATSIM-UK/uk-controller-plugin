#include "dialog/DialogCallArgument.h"
#include "ecfmp/AircraftFlowMeasureMapInterface.h"
#include "ecfmp/AircraftFlowMeasuresDialog.h"
#include "ecfmp/ListAircraftFlowMeasures.h"
#include "dialog/DialogManager.h"
#include "mock/FlowMeasureFiltersMock.h"
#include "mock/FlowMeasureMock.h"
#include "mock/MeasureMock.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include <gmock/gmock-nice-strict.h>

namespace UKControllerPluginTest::ECFMP {

    class MockListAircraftFlowMeasureMap : public UKControllerPlugin::ECFMP::AircraftFlowMeasureMapInterface
    {
        public:
        MOCK_METHOD(
            const std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>>&,
            GetFlowMeasuresForCallsign,
            (const std::string& callsign),
            (const, override));
    };

    auto MatchesExpectedFlowMeasureString(const UKControllerPlugin::Dialog::DialogCallArgument* const arg) -> int
    {
        const auto castedArg = reinterpret_cast<const UKControllerPlugin::ECFMP::AircraftFlowMeasuresDialogData* const>(
            reinterpret_cast<const UKControllerPlugin::Dialog::DialogCallArgument* const>(arg)->contextArgument);

        std::wstring expected;
        expected += L"Measure: EGTT01A\r\n\r\n";
        expected += L"Minimum Departure Interval: 5 Minutes\r\n\r\n";
        expected += L"Applicable To:\r\n\r\n";
        expected += L"Departing: EGKK\r\n";
        expected += L"Arriving: EGLL\r\n\r\n";
        expected += L"End of measure EGTT01A\r\n\r\n";
        expected += L"Measure: EGTT01B\r\n\r\n";
        expected += L"Minimum Departure Interval: 3 Minutes\r\n\r\n";
        expected += L"Applicable To:\r\n\r\n";
        expected += L"Departing: EGCC\r\n";
        expected += L"Arriving: EGPH\r\n\r\n";
        expected += L"End of measure EGTT01B";

        return L"BAW123" == castedArg->callsign && expected == castedArg->flowMeasures ? 1 : 0;
    }

    class ListAircraftFlowMeasuresTest : public testing::Test
    {
        public:
        ListAircraftFlowMeasuresTest()
            : dialogManager(dialogProvider),
              flowMeasureMap(std::make_shared<testing::NiceMock<MockListAircraftFlowMeasureMap>>()),
              listAircraftFlowMeasures(flowMeasureMap, dialogManager)
        {
            dialogManager.AddDialog(dialogData);
            ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        UKControllerPlugin::Dialog::DialogData dialogData = {IDD_FLOW_MEASURE_LIST, "", nullptr, NULL, nullptr};
        testing::NiceMock<Dialog::MockDialogProvider> dialogProvider;
        UKControllerPlugin::Dialog::DialogManager dialogManager;
        std::shared_ptr<MockListAircraftFlowMeasureMap> flowMeasureMap;
        UKControllerPlugin::ECFMP::ListAircraftFlowMeasures listAircraftFlowMeasures;
    };

    TEST_F(ListAircraftFlowMeasuresTest, ItDoesNothingIfNoFlowMeasures)
    {
        auto measures = std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>>{};
        EXPECT_CALL(*flowMeasureMap, GetFlowMeasuresForCallsign("BAW123")).WillOnce(testing::ReturnRef(measures));

        EXPECT_CALL(dialogProvider, OpenDialog).Times(0);

        listAircraftFlowMeasures.ListForAircraft(mockFlightplan);
    }

    TEST_F(ListAircraftFlowMeasuresTest, ItOpensDialogForFlowMeasures)
    {
        // Mock measure 1
        auto mockMeasure1 = std::make_shared<testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock>>();
        std::string mockMeasure1Identifier = "EGTT01A";
        ON_CALL(*mockMeasure1, Identifier).WillByDefault(testing::ReturnRef(mockMeasure1Identifier));

        // Measure 1 measure
        auto measure1Measure = std::make_shared<testing::NiceMock<::ECFMP::Mock::FlowMeasure::MeasureMock>>();
        ON_CALL(*measure1Measure, MeasureDescription)
            .WillByDefault(testing::Return("Minimum Departure Interval: 5 Minutes"));

        ON_CALL(*mockMeasure1, Measure).WillByDefault(testing::ReturnRef(*measure1Measure));

        // Measure 1 filters
        auto measure1Filters =
            std::make_shared<testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureFiltersMock>>();

        auto measure1FilterDescriptions = std::vector<std::string>{"Departing: EGKK", "Arriving: EGLL"};
        ON_CALL(*measure1Filters, FilterDescriptions).WillByDefault(testing::Return(measure1FilterDescriptions));

        ON_CALL(*mockMeasure1, Filters).WillByDefault(testing::ReturnRef(*measure1Filters));

        // Mock measure 2
        auto mockMeasure2 = std::make_shared<testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock>>();
        std::string mockMeasure2Identifier = "EGTT01B";
        ON_CALL(*mockMeasure2, Identifier).WillByDefault(testing::ReturnRef(mockMeasure2Identifier));

        // Measure 2 measure
        auto measure2Measure = std::make_shared<testing::NiceMock<::ECFMP::Mock::FlowMeasure::MeasureMock>>();
        ON_CALL(*measure2Measure, MeasureDescription)
            .WillByDefault(testing::Return("Minimum Departure Interval: 3 Minutes"));
        ON_CALL(*mockMeasure2, Measure).WillByDefault(testing::ReturnRef(*measure2Measure));

        // Measure 2 filters
        auto measure2Filters =
            std::make_shared<testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureFiltersMock>>();

        auto measure2FilterDescriptions = std::vector<std::string>{"Departing: EGCC", "Arriving: EGPH"};
        ON_CALL(*measure2Filters, FilterDescriptions).WillByDefault(testing::Return(measure2FilterDescriptions));

        ON_CALL(*mockMeasure2, Filters).WillByDefault(testing::ReturnRef(*measure2Filters));

        // Return measures from mocks
        auto measures =
            std::unordered_set<std::shared_ptr<const ::ECFMP::FlowMeasure::FlowMeasure>>{mockMeasure1, mockMeasure2};
        EXPECT_CALL(*flowMeasureMap, GetFlowMeasuresForCallsign("BAW123")).WillOnce(testing::ReturnRef(measures));

        EXPECT_CALL(dialogProvider, OpenDialog(dialogData, testing::Truly(MatchesExpectedFlowMeasureString))).Times(1);

        listAircraftFlowMeasures.ListForAircraft(mockFlightplan);
    }
} // namespace UKControllerPluginTest::ECFMP
