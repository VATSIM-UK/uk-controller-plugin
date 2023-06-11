#pragma once
#include "controller/ActiveCallsignEventHandlerInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Handoff {
    class DepartureHandoffResolver;
    class ResolvedHandoff;

    class InvalidateHandoffsOnActiveCallsignChanges : public Controller::ActiveCallsignEventHandlerInterface
    {
        public:
        InvalidateHandoffsOnActiveCallsignChanges(
            const std::shared_ptr<DepartureHandoffResolver>& resolver,
            Euroscope::EuroscopePluginLoopbackInterface& plugin);
        void ActiveCallsignAdded(const Controller::ActiveCallsign& callsign) override;
        void ActiveCallsignRemoved(const Controller::ActiveCallsign& callsign) override;
        void CallsignsFlushed() override;

        private:
        [[nodiscard]] static auto
        ShouldInvalidateOnCallsignAdded(const ResolvedHandoff& handoff, const Controller::ActiveCallsign& callsign)
            -> bool;

        // The resolver
        std::shared_ptr<DepartureHandoffResolver> resolver;

        // The plugin for flightplan looping
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };
} // namespace UKControllerPlugin::Handoff
