#include "ClearCacheOnActiveCallsignChanges.h"
#include "DepartureHandoffResolver.h"
#include "ResolvedHandoff.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::Handoff {

    ClearCacheOnActiveCallsignChanges::ClearCacheOnActiveCallsignChanges(
        const std::shared_ptr<DepartureHandoffResolver>& resolver, Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : resolver(std::move(resolver)), plugin(plugin)
    {
        assert(this->resolver != nullptr && "Resolver must not be null");
    }

    /**
     * When new controllers come online, we need to evict from the cache any controller who preceeds them
     * in the hierarchy.
     */
    void ClearCacheOnActiveCallsignChanges::ActiveCallsignAdded(const Controller::ActiveCallsign& callsign)
    {
        plugin.ApplyFunctionToAllFlightplans([&callsign, this](const auto& fp, const auto& rt) -> void {
            auto resolvedHandoff = this->resolver->Resolve(*fp);

            if (!ShouldInvalidateOnCallsignAdded(*resolvedHandoff, callsign)) {
                return;
            }

            this->resolver->Invalidate(*fp);
            static_cast<void>(this->resolver->Resolve(*fp));
        });
    }

    /**
     * If we have a controller that logs off, we only need to evict from the cache instances where
     * this controller is the resolved controller.
     */
    void ClearCacheOnActiveCallsignChanges::ActiveCallsignRemoved(const Controller::ActiveCallsign& callsign)
    {
        plugin.ApplyFunctionToAllFlightplans([&callsign, this](const auto& fp, const auto& rt) -> void {
            auto resolvedHandoff = this->resolver->Resolve(*fp);
            if (*resolvedHandoff->resolvedController != callsign.GetNormalisedPosition()) {
                return;
            }

            this->resolver->Invalidate(*fp);
            static_cast<void>(this->resolver->Resolve(*fp));
        });
    }

    /**
     * If we lose track of who's online alltogether, time to clear the cache.
     */
    void ClearCacheOnActiveCallsignChanges::CallsignsFlushed()
    {
        plugin.ApplyFunctionToAllFlightplans(
            [this](const auto& fp, const auto& rt) -> void { this->resolver->Invalidate(*fp); });
    }

    auto ClearCacheOnActiveCallsignChanges::ShouldInvalidateOnCallsignAdded(
        const ResolvedHandoff& handoff, const Controller::ActiveCallsign& callsign) -> bool
    {
        /*
         * The SID hierarchy has the highest priority. So if the resolved controller is from the SIDs hierarchy,
         * then the controller coming online must preceed it.
         */
        if (handoff.sidHierarchy->PositionInHierarchy(*handoff.resolvedController)) {
            return handoff.sidHierarchy->PositionPreceeds(
                callsign.GetNormalisedPosition(), *handoff.resolvedController);
        }

        /*
         * If the resolved controller isn't int he SID hierarchy, but the controller logging on is, then
         * this takes precedence.
         */
        if (handoff.sidHierarchy->PositionInHierarchy(callsign.GetNormalisedPosition())) {
            return true;
        }

        /*
         * If the resolved controller is in the airfield fallback hierarchy, then the controller coming online must
         * preceed them in that.
         */
        if (handoff.airfieldHierarchy->PositionInHierarchy(*handoff.resolvedController)) {
            return handoff.airfieldHierarchy->PositionPreceeds(
                callsign.GetNormalisedPosition(), *handoff.resolvedController);
        }

        /*
         * Finally, if the resolved controller isn't from a hierarchy, it must be unicom.
         *
         * The last check here is to see if the controller in question is in the airfield hierarchy.
         */
        return handoff.airfieldHierarchy->PositionInHierarchy(callsign.GetNormalisedPosition());
    }
} // namespace UKControllerPlugin::Handoff
