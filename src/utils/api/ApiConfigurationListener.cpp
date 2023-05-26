#include "ApiConfigurationListener.h"
#include "ApiKeyReceivedEvent.h"
#include "eventhandler/EventBus.h"

#include "cpp-httplib/httplib.h"

namespace UKControllerPluginUtils::Api {

    ApiConfigurationListener::ApiConfigurationListener() : host("localhost")
    {
        ConfigureServer();
    }

    ApiConfigurationListener::~ApiConfigurationListener()
    {
        LogDebug("Shutting down ApiConfigurationListener");
        if (this->server->is_running()) {
            LogDebug("Stopping ApiConfigurationListener server");
            this->server->stop();
        }

        LogDebug("Joining ApiConfigurationListener thread");
        serverListenThread.join();
    }

    void ApiConfigurationListener::ConfigureServer()
    {
        // Bind server to port
        this->server = std::make_unique<httplib::Server>();

        // Setup our route
        server->Get("/", [](const httplib::Request& request, httplib::Response& response) {
            if (!request.params.contains("key") || request.params.find("key")->second.empty()) {
                response.set_content("Invalid request.", "text/plain");
                response.status = 400;
                LogWarning("Received invalid API key");
                return;
            }

            const std::string responseString = "UK Controller Plugin API key received successfully. You may now close "
                                               "this window.";
            response.set_content(responseString, "text/plain");
            LogInfo("Received new API key");
            EventHandler::EventBus::Bus().OnEvent(ApiKeyReceivedEvent(request.params.find("key")->second));
        });

        // Listen on the thread
        serverListenThread = std::thread([this]() {
            host = "localhost";
            port = server->bind_to_any_port(host);
            server->listen_after_bind();
        });

        while (!server->is_running()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    auto ApiConfigurationListener::Port() const -> int
    {
        return port;
    }

    auto ApiConfigurationListener::Host() const -> const std::string&
    {
        return host;
    }
} // namespace UKControllerPluginUtils::Api
