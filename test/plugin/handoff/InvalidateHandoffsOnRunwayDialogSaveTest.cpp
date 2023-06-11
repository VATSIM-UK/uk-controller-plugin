#include "handoff/InvalidateHandoffsOnRunwayDialogSave.h"

using UKControllerPlugin::Handoff::InvalidateHandoffsOnRunwayDialogSave;
using UKControllerPlugin::Handoff::ResolvedHandoff;

namespace UKControllerPluginTest::Handoff {

    class InvalidateHandoffsOnRunwayDialogSaveTest : public testing::Test
    {
        public:
        InvalidateHandoffsOnRunwayDialogSaveTest()
            : resolver(std::make_shared<testing::NiceMock<MockDepartureHandoffResolver>>()), clear(resolver, plugin)
        {
        }

        std::shared_ptr<testing::NiceMock<MockDepartureHandoffResolver>> resolver;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        InvalidateHandoffsOnRunwayDialogSave clear;
    };

    TEST_F(InvalidateHandoffsOnRunwayDialogSaveTest, SavingTheRunwayDialogInvalidatesAndReResolvesFlightplans)
    {
        auto fp1 = std::make_shared<Euroscope::MockEuroScopeCFlightPlanInterface>();
        auto rt1 = std::make_shared<Euroscope::MockEuroScopeCRadarTargetInterface>();
        auto fp2 = std::make_shared<Euroscope::MockEuroScopeCFlightPlanInterface>();
        auto rt2 = std::make_shared<Euroscope::MockEuroScopeCRadarTargetInterface>();
        auto fp3 = std::make_shared<Euroscope::MockEuroScopeCFlightPlanInterface>();
        auto rt3 = std::make_shared<Euroscope::MockEuroScopeCRadarTargetInterface>();

        plugin.AddAllFlightplansItem({fp1, rt1});
        plugin.AddAllFlightplansItem({fp2, rt2});
        plugin.AddAllFlightplansItem({fp3, rt3});

        EXPECT_CALL(*resolver, Invalidate(testing::Ref(*fp1)));
        EXPECT_CALL(*resolver, Resolve(testing::Ref(*fp1)));
        EXPECT_CALL(*resolver, Invalidate(testing::Ref(*fp2)));
        EXPECT_CALL(*resolver, Resolve(testing::Ref(*fp2)));
        EXPECT_CALL(*resolver, Invalidate(testing::Ref(*fp3)));
        EXPECT_CALL(*resolver, Resolve(testing::Ref(*fp3)));

        clear.RunwayDialogSaved();
    }
} // namespace UKControllerPluginTest::Handoff
