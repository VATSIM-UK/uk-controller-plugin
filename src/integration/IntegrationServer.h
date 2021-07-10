#pragma once

namespace UKControllerPlugin::Integration {
    /*
     * A class that listens for connections on the integration and responds
     * accordingly
     */
    class IntegrationServer
    {
        public:
            IntegrationServer();
            ~IntegrationServer();

        private:
            SOCKET serverSocket = INVALID_SOCKET;

            bool initialised = false;
    };
} // namespace UKControllerPlugin::Integration
