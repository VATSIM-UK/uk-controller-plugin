#pragma once
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin::Integration {
    class IntegrationConnection;
    class IntegrationClientManager;

    /*
     * Manages the initialisation of clients and
     * ensures that clients are properly initialised before being
     * allowed to interact with the plugin.
     */
    class ClientInitialisationManager : public TimedEvent::AbstractTimedEvent
    {
        public:
            explicit ClientInitialisationManager(std::shared_ptr<IntegrationClientManager> clientManager);
            ~ClientInitialisationManager() override = default;
            void AddConnection(std::shared_ptr<IntegrationConnection> connection);
            void TimedEventTrigger() override;

        private:
            // Manages clients once fully initialised
            std::shared_ptr<IntegrationClientManager> clientManager;

            // Clients that are fully initialised
            std::map<std::shared_ptr<IntegrationConnection>, std::chrono::system_clock::time_point> connections;
    };
} // namespace UKControllerPlugin::Integration
