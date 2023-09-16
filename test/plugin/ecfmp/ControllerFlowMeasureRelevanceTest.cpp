
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "ecfmp/ControllerFlowMeasureRelevance.h"
#include "mock/FlowMeasureMock.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include <gmock/gmock-actions.h>

namespace UKControllerPluginTest::ECFMP {

    struct ControllerFlowMeasureRelevanceTestCase
    {
        // Test case name
        std::string description;

        // Notified fir ICAOs
        std::vector<std::string> notifiedFirIcaos;

        // User callsign (blank for none)
        std::string userCallsign;

        // Expected result
        bool expectedResult;
    };

    class ControllerFlowMeasureRelevanceTest : public ::testing::TestWithParam<ControllerFlowMeasureRelevanceTestCase>
    {
        public:
        ControllerFlowMeasureRelevanceTest() : controllerFlowMeasureRelevance(activeCallsigns)
        {
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightPlan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        UKControllerPlugin::Controller::ActiveCallsignCollection activeCallsigns;
        UKControllerPlugin::ECFMP::ControllerFlowMeasureRelevance controllerFlowMeasureRelevance;
    };

    TEST_P(ControllerFlowMeasureRelevanceTest, FlowMeasureApplicableToAircraft)
    {
        const auto& testCase = GetParam();

        // Set the user callsign
        UKControllerPlugin::Controller::ControllerPosition position(
            1, testCase.userCallsign, 121.800, {"EGKK"}, true, false);
        UKControllerPlugin::Controller::ActiveCallsign callsign(
            testCase.userCallsign, "Testy McTestface", position, true);
        if (!testCase.userCallsign.empty()) {
            activeCallsigns.AddUserCallsign(callsign);
        }

        // Create the flow measure
        std::vector<std::shared_ptr<const ::ECFMP::FlightInformationRegion::FlightInformationRegion>> notifiedFirs;
        for (const auto& notifiedFirIcao : testCase.notifiedFirIcaos) {
            const auto mockFir = std::make_shared<
                testing::NiceMock<::ECFMP::Mock::FlightInformationRegion::FlightInformationRegionMock>>();
            ON_CALL(*mockFir, Identifier).WillByDefault(testing::ReturnRef(notifiedFirIcao));
            notifiedFirs.push_back(mockFir);
        }

        testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock> mockFlowMeasure;
        ON_CALL(mockFlowMeasure, NotifiedFlightInformationRegions).WillByDefault(testing::ReturnRef(notifiedFirs));

        // Check the result
        EXPECT_EQ(
            controllerFlowMeasureRelevance.FlowMeasureApplicableToAircraft(flightPlan, radarTarget, mockFlowMeasure),
            testCase.expectedResult);
    }

    INSTANTIATE_TEST_SUITE_P(
        ControllerFlowMeasureRelevanceTest,
        ControllerFlowMeasureRelevanceTest,
        testing::Values(
            // No callsigns
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_user_has_no_callsign_london",
                {"EGTT", "LFBB"},
                "",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_user_has_no_callsign_scottish",
                {"EGPX", "LFBB"},
                "",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_user_has_no_callsign_global",
                {"XXXX", "LFBB"},
                "",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_user_has_no_callsign_other",
                {"LFBB"},
                "",
                true,
            },

            // Global FIRs
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_london_bandbox_and_flow_measure_targeting_global",
                {"XXXX"},
                "LON_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_london_split_and_flow_measure_targeting_global",
                {"XXXX"},
                "LON_W_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_scottish_bandbox_and_flow_measure_targeting_global",
                {"XXXX"},
                "SCO_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_scottish_split_and_flow_measure_targeting_global",
                {"XXXX"},
                "SCO_E_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_ltc_and_flow_measure_targeting_global",
                {"XXXX"},
                "LTC_SE_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_man_and_flow_measure_targeting_global",
                {"XXXX"},
                "MAN_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_man_split_and_flow_measure_targeting_global",
                {"XXXX"},
                "MAN_S_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_stc_and_flow_measure_targeting_global",
                {"XXXX"},
                "STC_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_stc_split_and_flow_measure_targeting_global",
                {"XXXX"},
                "STC_A_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_eggd_and_flow_measure_targeting_global",
                {"XXXX"},
                "EGGD_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_egll_and_flow_measure_targeting_global",
                {"XXXX"},
                "EGLL_S_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_egkk_and_flow_measure_targeting_global",
                {"XXXX"},
                "EGKK_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_egph_and_flow_measure_targeting_global",
                {"XXXX"},
                "EGPH_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_sco_mil_and_flow_measure_targeting_global",
                {"XXXX"},
                "EGQP_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_egaa_and_flow_measure_targeting_global",
                {"XXXX"},
                "EGAA_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_essex_and_flow_measure_targeting_global",
                {"XXXX"},
                "ESSEX_APP",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_thames_and_flow_measure_targeting_global",
                {"XXXX"},
                "THAMES_APP",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_solent_and_flow_measure_targeting_global",
                {"XXXX"},
                "SOLENT_APP",
                true,
            },

            // EGTT
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_london_bandbox_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "LON_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_london_split_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "LON_W_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_scottish_bandbox_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "SCO_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_scottish_split_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "SCO_E_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_ltc_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "LTC_SE_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_man_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "MAN_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_man_split_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "MAN_S_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_stc_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "STC_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_stc_split_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "STC_A_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_eggd_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "EGGD_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_egll_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "EGLL_S_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_egkk_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "EGKK_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_egph_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "EGPH_TWR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_sco_mil_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "EGQP_TWR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_egaa_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "EGAA_TWR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_essex_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "ESSEX_APP",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_thames_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "THAMES_APP",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_solent_and_flow_measure_targeting_london",
                {"EGTT", "LFBB"},
                "SOLENT_APP",
                true,
            },

            // EGPX
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_london_bandbox_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "LON_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_london_split_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "LON_W_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_scottish_bandbox_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "SCO_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_scottish_split_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "SCO_E_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_ltc_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "LTC_SE_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_man_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "MAN_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_man_split_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "MAN_S_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_stc_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "STC_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_stc_split_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "STC_A_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_eggd_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "EGGD_TWR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_egll_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "EGLL_S_TWR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_egkk_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "EGKK_TWR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_egph_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "EGPH_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_sco_mil_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "EGQP_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_egaa_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "EGAA_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_essex_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "ESSEX_APP",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_thames_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "THAMES_APP",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_solent_and_flow_measure_targeting_scottish",
                {"EGPX", "LFBB"},
                "SOLENT_APP",
                false,
            },

            // EGTT and EGPX
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_london_bandbox_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "LON_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_london_split_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "LON_W_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_scottish_bandbox_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "SCO_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_scottish_split_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "SCO_E_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_ltc_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "LTC_SE_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_man_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "MAN_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_man_split_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "MAN_S_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_stc_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "STC_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_stc_split_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "STC_A_CTR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_eggd_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "EGGD_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_egll_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "EGLL_S_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_egkk_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "EGKK_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_egph_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "EGPH_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_sco_mil_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "EGQP_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_egaa_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "EGAA_TWR",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_essex_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "ESSEX_APP",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_thames_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "THAMES_APP",
                true,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_true_controller_is_solent_and_flow_measure_targeting_both",
                {"EGTT", "EGPX", "LFBB"},
                "SOLENT_APP",
                true,
            },

            // LFBB
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_london_bandbox_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "LON_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_london_split_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "LON_W_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_scottish_bandbox_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "SCO_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_scottish_split_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "SCO_E_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_ltc_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "LTC_SE_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_man_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "MAN_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_man_split_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "MAN_S_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_stc_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "STC_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_stc_split_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "STC_A_CTR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_eggd_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "EGGD_TWR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_egll_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "EGLL_S_TWR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_egkk_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "EGKK_TWR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_egph_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "EGPH_TWR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_sco_mil_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "EGQP_TWR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_egaa_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "EGAA_TWR",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_essex_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "ESSEX_APP",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_thames_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "THAMES_APP",
                false,
            },
            ControllerFlowMeasureRelevanceTestCase{
                "returns_false_controller_is_solent_and_flow_measure_targeting_others",
                {"EDDB", "LOVV", "LFBB"},
                "SOLENT_APP",
                false,
            }),
        [](const auto& info) { return info.param.description; });
} // namespace UKControllerPluginTest::ECFMP
