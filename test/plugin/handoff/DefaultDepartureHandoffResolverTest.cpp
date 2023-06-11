#include "controller/ControllerPosition.h"
#include "handoff/DefaultDepartureHandoffResolver.h"
#include "handoff/DepartureHandoffResolutionStrategy.h"
#include "handoff/DepartureHandoffResolvedEvent.h"
#include "handoff/ResolvedHandoff.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "test/EventBusTestCase.h"

namespace UKControllerPluginTest::Handoff {

    class MockResolutionStrategy : public UKControllerPlugin::Handoff::DepartureHandoffResolutionStrategy
    {
        public:
        MockResolutionStrategy()
        {
        }

        auto Resolve(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan) const
            -> std::shared_ptr<const UKControllerPlugin::Handoff::ResolvedHandoff> override
        {
            if (called) {
                throw std::runtime_error("Resolve called more than once");
            }

            called = true;
            calledWith = flightPlan.GetCallsign();

            auto controllerPosition = std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
                1, "LON_S_CTR", 129.420, std::vector<std::string>{}, true, false);
            return std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>(
                calledWith, controllerPosition, nullptr, nullptr);
        }

        mutable bool called = false;
        mutable std::string calledWith;
    };

    class DefaultDepartureHandoffResolverTest : public UKControllerPluginUtilsTest::EventBusTestCase
    {
        public:
        DefaultDepartureHandoffResolverTest()
        {
            auto strategyMock = std::make_unique<MockResolutionStrategy>();
            resolutionStrategyMock = strategyMock.get();
            resolver =
                std::make_unique<UKControllerPlugin::Handoff::DefaultDepartureHandoffResolver>(std::move(strategyMock));

            ON_CALL(flightplanMock, GetCallsign).WillByDefault(testing::Return("BAW123"));
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplanMock;
        MockResolutionStrategy* resolutionStrategyMock;
        std::unique_ptr<UKControllerPlugin::Handoff::DefaultDepartureHandoffResolver> resolver;
    };

    TEST_F(DefaultDepartureHandoffResolverTest, ItReturnsCachedEntryWhenResolveIsCalled)
    {
        const std::string callsign = "BAW123";
        const auto handoff =
            std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>(callsign, nullptr, nullptr, nullptr);
        resolver->AddToCache(callsign, handoff);

        // Check the resolved and cached value
        const auto cacheResult = resolver->GetCached(callsign);
        ASSERT_EQ(cacheResult, handoff);
        const auto resolveResult = resolver->Resolve(flightplanMock);
        ASSERT_EQ(resolveResult, handoff);

        // Check the strategy wasn't called
        ASSERT_FALSE(resolutionStrategyMock->called);
        AssertNoEventsDispatched();
    }

    TEST_F(DefaultDepartureHandoffResolverTest, ResolveUsesTheResolutionStrategyAndCachesTheResult)
    {
        auto resolved = resolver->Resolve(flightplanMock);
        ASSERT_EQ(resolved->callsign, "BAW123");

        // Check the strategy was called
        ASSERT_TRUE(resolutionStrategyMock->called);
        ASSERT_EQ(resolutionStrategyMock->calledWith, "BAW123");

        // Check the result was cached
        auto cached = resolver->GetCached("BAW123");
        ASSERT_EQ(cached, resolved);

        AssertSingleEventDispatched();
        AssertFirstEventDispatched<UKControllerPlugin::Handoff::DepartureHandoffResolvedEvent>(
            [&resolved](const UKControllerPlugin::Handoff::DepartureHandoffResolvedEvent& event) {
                return event.handoff == resolved;
            });
    }

    TEST_F(DefaultDepartureHandoffResolverTest, InvalidateRemovesCorrectEntryFromCache)
    {
        // Add the on we want to remove
        const std::string callsign = "BAW123";
        const auto handoff =
            std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>(callsign, nullptr, nullptr, nullptr);
        resolver->AddToCache(callsign, handoff);

        // Add another one to make sure it doesn't get removed
        const std::string callsign2 = "BAW124";
        const auto handoff2 =
            std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>(callsign2, nullptr, nullptr, nullptr);
        resolver->AddToCache(callsign2, handoff2);

        // Invalidate the first one and check it's gone
        resolver->Invalidate(flightplanMock);
        EXPECT_EQ(resolver->GetCached(callsign), nullptr);
        EXPECT_EQ(resolver->GetCached(callsign2), handoff2);
    }

    TEST_F(DefaultDepartureHandoffResolverTest, InvalidateHandlesMissingCacheEntries)
    {
        EXPECT_NO_THROW(resolver->Invalidate(flightplanMock));
    }
} // namespace UKControllerPluginTest::Handoff
