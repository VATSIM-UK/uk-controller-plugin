#pragma once
#include "ApiConfigurationListenerInterface.h"

// Predeclare the server class we're going to use
namespace httplib {
    class Server;
} // namespace httplib

namespace UKControllerPluginUtils::Api {
    class ApiConfigurationListener : public ApiConfigurationListenerInterface
    {
        public:
        ApiConfigurationListener();
        ~ApiConfigurationListener();
        auto Port() const -> int override;
        auto Host() const -> const std::string& override;

        private:
        void ConfigureServer();

        // Server
        std::unique_ptr<httplib::Server> server;

        // Host
        std::string host;

        // Port
        int port = 0;

        // A thread to start the server on
        std::thread serverListenThread;
    };
} // namespace UKControllerPluginUtils::Api
