#include "ecfmp/HomeFirsFlowMeasureFilter.h"
#include "ECFMP/SdkEvents.h"
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"

namespace UKControllerPluginTest::ECFMP {
    class HomeFirsFlowMeasureFilterTest : public testing::Test
    {
        public:
        [[nodiscard]] auto GetMeasure(const std::vector<std::string>& notifiedFirs)
            -> std::shared_ptr<testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock>>
        {
            firIcaos = notifiedFirs;
            auto measure = std::make_shared<testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock>>();
            firs = GetRegions(firIcaos);

            ON_CALL(*measure, NotifiedFlightInformationRegions).WillByDefault(testing::ReturnRef(firs));

            return measure;
        }

        UKControllerPlugin::ECFMP::HomeFirsFlowMeasureFilter filter;

        private:
        [[nodiscard]] static auto GetRegions(const std::vector<std::string>& icaoCodes)
            -> std::vector<std::shared_ptr<const ::ECFMP::FlightInformationRegion::FlightInformationRegion>>
        {
            std::vector<std::shared_ptr<const ::ECFMP::FlightInformationRegion::FlightInformationRegion>> regions;

            for (const auto& icaoCode : icaoCodes) {
                auto mock = std::make_shared<
                    ::testing::NiceMock<::ECFMP::Mock::FlightInformationRegion::FlightInformationRegionMock>>();
                ON_CALL(*mock, Identifier).WillByDefault(testing::ReturnRef(icaoCode));
                regions.push_back(mock);
            }

            return regions;
        }

        std::vector<std::string> firIcaos;
        std::vector<std::shared_ptr<const ::ECFMP::FlightInformationRegion::FlightInformationRegion>> firs;
    };

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnActivatedEventIfIsLondon)
    {
        const auto measure = GetMeasure({"LFFF", "EGTT"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureActivatedEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnActivatedEventIfIsScottish)
    {
        const auto measure = GetMeasure({"LFFF", "EGPX"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureActivatedEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnActivatedEventIfIsScottishAndLondon)
    {
        const auto measure = GetMeasure({"EGTT", "LFFF", "EGPX"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureActivatedEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsFalseOnActivatedEventIfIsNotScottishOrLondon)
    {
        const auto measure = GetMeasure({"EDDM", "LFFF", "EURW"});
        EXPECT_FALSE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureActivatedEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnWithdrawnEventIfIsLondon)
    {
        const auto measure = GetMeasure({"LFFF", "EGTT"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureWithdrawnEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnWithdrawnEventIfIsScottish)
    {
        const auto measure = GetMeasure({"LFFF", "EGPX"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureWithdrawnEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnWithdrawnEventIfIsScottishAndLondon)
    {
        const auto measure = GetMeasure({"EGTT", "LFFF", "EGPX"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureWithdrawnEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsFalseOnWithdrawnEventIfIsNotScottishOrLondon)
    {
        const auto measure = GetMeasure({"EDDM", "LFFF", "EURW"});
        EXPECT_FALSE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureWithdrawnEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnExpiredEventIfIsLondon)
    {
        const auto measure = GetMeasure({"LFFF", "EGTT"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureExpiredEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnExpiredEventIfIsScottish)
    {
        const auto measure = GetMeasure({"LFFF", "EGPX"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureExpiredEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnExpiredEventIfIsScottishAndLondon)
    {
        const auto measure = GetMeasure({"EGTT", "LFFF", "EGPX"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureExpiredEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsFalseOnExpiredEventIfIsNotScottishOrLondon)
    {
        const auto measure = GetMeasure({"EDDM", "LFFF", "EURW"});
        EXPECT_FALSE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureExpiredEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnNotifiedEventIfIsLondon)
    {
        const auto measure = GetMeasure({"LFFF", "EGTT"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureNotifiedEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnNotifiedEventIfIsScottish)
    {
        const auto measure = GetMeasure({"LFFF", "EGPX"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureNotifiedEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnNotifiedEventIfIsScottishAndLondon)
    {
        const auto measure = GetMeasure({"EGTT", "LFFF", "EGPX"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureNotifiedEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsFalseOnNotifiedEventIfIsNotScottishOrLondon)
    {
        const auto measure = GetMeasure({"EDDM", "LFFF", "EURW"});
        EXPECT_FALSE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureNotifiedEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnReissuedEventIfIsLondon)
    {
        const auto measure = GetMeasure({"LFFF", "EGTT"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureReissuedEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnReissuedEventIfIsScottish)
    {
        const auto measure = GetMeasure({"LFFF", "EGPX"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureReissuedEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsTrueOnReissuedEventIfIsScottishAndLondon)
    {
        const auto measure = GetMeasure({"EGTT", "LFFF", "EGPX"});
        EXPECT_TRUE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureReissuedEvent{measure}));
    }

    TEST_F(HomeFirsFlowMeasureFilterTest, ShouldProcessReturnsFalseOnReissuedEventIfIsNotScottishOrLondon)
    {
        const auto measure = GetMeasure({"EDDM", "LFFF", "EURW"});
        EXPECT_FALSE(filter.ShouldProcess(::ECFMP::Plugin::FlowMeasureReissuedEvent{measure}));
    }
} // namespace UKControllerPluginTest::ECFMP
