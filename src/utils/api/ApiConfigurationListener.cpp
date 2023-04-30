#include "ApiConfigurationListener.h"

#include "cpp-httplib/httplib.h"

namespace UKControllerPluginUtils::Api {

    ApiConfigurationListener::ApiConfigurationListener() : host("localhost")
    {
        ConfigureServer();
    }

    ApiConfigurationListener::~ApiConfigurationListener()
    {
        if (this->server->is_running()) {
            this->server->stop();
        }

        serverListenThread.join();
    }

    void ApiConfigurationListener::ConfigureServer()
    {
        // Bind server to port
        this->server = std::make_unique<httplib::Server>();
        this->port = server->bind_to_any_port("localhost");

        // Setup our route
        server->Get("/", [](const httplib::Request& request, httplib::Response& response) {
            if (!request.params.contains("key") || request.params.find("key")->second.empty()) {
                response.set_content("Invalid request.", "text/plain");
                response.status = 400;
                return;
            }

            const std::string responseString = "UK Controller Plugin API key received successfully. You may now close "
                                               "this window.";
            response.set_content(responseString, "text/plain");
        });

        serverListenThread = std::thread([this]() { server->listen_after_bind(); });
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
