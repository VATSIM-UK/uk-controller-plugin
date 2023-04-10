#pragma once
#include "departure/UserShouldClearDepartureDataEvent.h"
#include "eventhandler/EventHandler.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }
    namespace Sid {
        class SidMapperInterface;
    } // namespace Sid
} // namespace UKControllerPlugin

namespace UKControllerPlugin::InitialHeading {

    /**
     * Clears the initial altitude when a user should be clearing departure settings.
     */
    class ClearInitialHeading : public EventHandler::EventHandler<Departure::UserShouldClearDepartureDataEvent>
    {
        public:
        ClearInitialHeading(
            const Euroscope::EuroscopePluginLoopbackInterface& plugin, const Sid::SidMapperInterface& sidMapper);
        void OnEvent(const Departure::UserShouldClearDepartureDataEvent& event) override;

        private:
        // For getting flightplans
        const Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // For mapping flightplans to sid
        const Sid::SidMapperInterface& sidMapper;

        // The euroscope value for "no cleared level"
        const int EUROSCOPE_FLIGHTPLAN_NO_HEADING = 0;
    };
} // namespace UKControllerPlugin::InitialHeading
