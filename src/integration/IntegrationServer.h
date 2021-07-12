#pragma once

namespace UKControllerPlugin::Integration {
    class IntegrationClientManager;
    /*
     * A class that listens for connections on the integration and responds
     * accordingly
     */
    class IntegrationServer
    {
        public:
            IntegrationServer(std::shared_ptr<IntegrationClientManager> manager);
            ~IntegrationServer();
            void AcceptLoop() const;

        private:
            // The server socket for listening for new connections
            SOCKET serverSocket = INVALID_SOCKET;

            // Whether we fully initialised ourselves
            bool initialised = false;

            // Thread to do all the things
            std::shared_ptr<std::thread> acceptThread;

            // Are we acceptingConnections
            bool acceptingConnections;

            // The connection manager
            std::shared_ptr<IntegrationClientManager> manager;
    };
} // namespace UKControllerPlugin::Integration
