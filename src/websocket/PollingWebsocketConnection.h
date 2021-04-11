#pragma once
#include "websocket/WebsocketConnectionInterface.h"

namespace UKControllerPlugin {

    namespace Api {
        class ApiInterface;
    } // namespace Api

    namespace Websocket {
        /*
            A class that imitates the websocket connection but actually gets its data
            through regular polling.
        */
        class PollingWebsocketConnection : public WebsocketConnectionInterface
        {
            public:
                explicit PollingWebsocketConnection(const Api::ApiInterface& api);

                // Inherited from WebsocketConnectionInterface
                void WriteMessage(std::string message) override;
                std::string GetNextMessage() override;
                void SetIdleTimeout(std::chrono::seconds timeout) override;

            private:

                // The api for polling for updates
                const Api::ApiInterface& api;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
