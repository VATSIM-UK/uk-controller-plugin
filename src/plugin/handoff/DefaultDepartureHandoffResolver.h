#pragma once
#include "DepartureHandoffResolver.h"

namespace UKControllerPlugin::Handoff {

    class DepartureHandoffResolutionStrategy;

    class DefaultDepartureHandoffResolver : public DepartureHandoffResolver
    {
        public:
        DefaultDepartureHandoffResolver(std::unique_ptr<DepartureHandoffResolutionStrategy> strategy);
        void AddToCache(const std::string& callsign, std::shared_ptr<const ResolvedHandoff> handoff);
        [[nodiscard]] auto GetCached(const std::string& callsign) const -> std::shared_ptr<const ResolvedHandoff>;
        auto Resolve(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
            -> std::shared_ptr<const ResolvedHandoff> override;
        auto Invalidate(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) -> void override;

        private:
        // The strategy
        const std::unique_ptr<DepartureHandoffResolutionStrategy> strategy;

        // Cache of resolved handoffs
        std::unordered_map<std::string, std::shared_ptr<const ResolvedHandoff>> cache;
    };

} // namespace UKControllerPlugin::Handoff
