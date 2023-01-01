#include "AircraftIntentionCode.h"
#include "CachedAircraftIntentionCodeGenerator.h"
#include "CodeGenerator.h"
#include "Condition.h"
#include "IntentionCodeCollection.h"
#include "IntentionCodeModel.h"
#include "IntentionCodeUpdatedMessage.h"
#include "IntentionCodeEventHandlerCollection.h"
#include "controller/ActiveCallsign.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::IntentionCode {

    CachedAircraftIntentionCodeGenerator::CachedAircraftIntentionCodeGenerator(
        std::shared_ptr<const IntentionCodeCollection> intentionCodes,
        std::shared_ptr<const IntentionCodeEventHandlerCollection> eventHandlers)
        : intentionCodes(intentionCodes), eventHandlers(eventHandlers)
    {
        assert(intentionCodes && "intention codes not set in CachedAircraftIntentionCodeGenerator");
        assert(eventHandlers && "event handlers codes not set in CachedAircraftIntentionCodeGenerator");
    }

    void CachedAircraftIntentionCodeGenerator::AddCacheEntry(const std::shared_ptr<AircraftIntentionCode>& entry)
    {
        cache[entry->callsign] = entry;
    }

    auto CachedAircraftIntentionCodeGenerator::GetCacheEntryForCallsign(const std::string& callsign) const
        -> std::shared_ptr<AircraftIntentionCode>
    {
        return cache.contains(callsign) ? cache.at(callsign) : nullptr;
    }

    void CachedAircraftIntentionCodeGenerator::RemoveCacheEntryForCallsign(const std::string& callsign)
    {
        cache.erase(callsign);
    }

    auto CachedAircraftIntentionCodeGenerator::Generate(
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> std::shared_ptr<AircraftIntentionCode>
    {
        auto cachedEntry = GetCacheEntryForCallsign(flightplan.GetCallsign());
        if (cachedEntry) {
            return cachedEntry;
        }

        auto intentionCode = AircraftIntentionCode{};
        intentionCode.callsign = flightplan.GetCallsign();

        const auto matchedIntentionCode =
            intentionCodes->FirstWhere([&flightplan, &radarTarget](const IntentionCodeModel& code) -> bool {
                return code.Conditions().Passes(flightplan, radarTarget);
            });

        intentionCode.intentionCode =
            matchedIntentionCode ? matchedIntentionCode->Generator().GenerateCode(flightplan) : "--";
        intentionCode.matchedIntentionCode = matchedIntentionCode;

        auto cacheItem = std::make_shared<AircraftIntentionCode>(intentionCode);
        AddCacheEntry(cacheItem);
        eventHandlers->IntentionCodeUpdated(*cacheItem);

        return cacheItem;
    }

    void CachedAircraftIntentionCodeGenerator::FlightPlanEvent(
        Euroscope::EuroScopeCFlightPlanInterface& flightPlan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
    {
        RemoveCacheEntryForCallsign(flightPlan.GetCallsign());
    }

    void CachedAircraftIntentionCodeGenerator::FlightPlanDisconnectEvent(
        Euroscope::EuroScopeCFlightPlanInterface& flightPlan)
    {
        RemoveCacheEntryForCallsign(flightPlan.GetCallsign());
    }

    void CachedAircraftIntentionCodeGenerator::ControllerFlightPlanDataEvent(
        Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
        // Nothing to do here
    }

    void CachedAircraftIntentionCodeGenerator::ActiveCallsignAdded(const Controller::ActiveCallsign& callsign)
    {
        if (!callsign.GetIsUser()) {
            return;
        }

        cache.clear();
    }

    void CachedAircraftIntentionCodeGenerator::ActiveCallsignRemoved(const Controller::ActiveCallsign& callsign)
    {
        if (!callsign.GetIsUser()) {
            return;
        }

        cache.clear();
    }

} // namespace UKControllerPlugin::IntentionCode
