#include "DefaultDepartureHandoffResolver.h"
#include "DepartureHandoffResolutionStrategy.h"
#include "DepartureHandoffResolvedEvent.h"
#include "ResolvedHandoff.h"
#include "controller/ControllerPosition.h"
#include "datablock/DatablockFunctions.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "eventhandler/EventBus.h"

namespace UKControllerPlugin::Handoff {

    DefaultDepartureHandoffResolver::DefaultDepartureHandoffResolver(
        std::unique_ptr<DepartureHandoffResolutionStrategy> strategy)
        : strategy(std::move(strategy))
    {
        assert(this->strategy != nullptr && "Strategy must not be null");
    }

    auto DefaultDepartureHandoffResolver::Resolve(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
        -> std::shared_ptr<const ResolvedHandoff>
    {
        // Check the cache
        auto cached = this->GetCached(flightplan.GetCallsign());
        if (cached != nullptr) {
            return cached;
        }

        // Resolve the handoff and cache it
        const auto handoff = this->strategy->Resolve(flightplan);
        this->AddToCache(flightplan.GetCallsign(), handoff);
        LogDebug(
            "Resolved departure handoff of " + handoff->resolvedController->GetCallsign() + " (" +
            Datablock::FrequencyStringFromDouble(handoff->resolvedController->GetFrequency()) + ") for " +
            handoff->callsign);
        UKControllerPluginUtils::EventHandler::EventBus::Bus().OnEvent<DepartureHandoffResolvedEvent>({handoff});
        return handoff;
    }

    auto DefaultDepartureHandoffResolver::Invalidate(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) -> void
    {
        this->cache.erase(flightplan.GetCallsign());
    }

    void DefaultDepartureHandoffResolver::AddToCache(
        const std::string& callsign, std::shared_ptr<const ResolvedHandoff> handoff)
    {
        this->cache[callsign] = handoff;
    }

    auto DefaultDepartureHandoffResolver::GetCached(const std::string& callsign) const
        -> std::shared_ptr<const ResolvedHandoff>
    {
        auto found = this->cache.find(callsign);
        return found != this->cache.end() ? found->second : nullptr;
    }
} // namespace UKControllerPlugin::Handoff
