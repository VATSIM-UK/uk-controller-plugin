#include "pch/stdafx.h"
#include "websocket/WebsocketBootstrap.h"
#include "websocket/WebsocketConnection.h"
#include "websocket/PusherActivityTimeoutEventHandler.h"
#include "websocket/PusherConnectionChannelSubscriptionEventHandler.h"
#include "websocket/PusherErrorEventHandler.h"
#include "websocket/PusherPingEventHandler.h"
#include "websocket/PusherWebsocketProtocolHandler.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            Bootstrap up the websocket.
        */
        void BootstrapPlugin(PersistenceContainer & container)
        {
            // Connect to websocket
            std::string wsHost = container.settingsRepository->HasSetting("websocket_host")
                ? container.settingsRepository->GetSetting("websocket_host")
                : "ukcp.vatsim.uk";

            std::string wsPort = container.settingsRepository->HasSetting("websocket_port")
                ? container.settingsRepository->GetSetting("websocket_port")
                : "6001";

            container.websocket.reset(
                new WebsocketConnection(
                    wsHost,
                    wsPort
                )
            );

            // Set up handler collection
            container.websocketProcessors.reset(new WebsocketEventProcessorCollection);

            // Set up handlers that look after the websocket protocol
            container.websocketProcessors->AddProcessor(
                std::make_shared<PusherErrorEventHandler>()
            );

            container.websocketProcessors->AddProcessor(
                std::make_shared<PusherPingEventHandler>(*container.websocket)
            );

            std::shared_ptr<PusherActivityTimeoutEventHandler> timeoutHandler = 
                std::make_shared<PusherActivityTimeoutEventHandler>(*container.websocket);

            container.websocketProcessors->AddProcessor(
                timeoutHandler
            );

            container.websocketProcessors->AddProcessor(
                std::make_shared<PusherConnectionChannelSubscriptionEventHandler>(
                    *container.websocket,
                    *container.websocketProcessors,
                    *container.api,
                    *container.taskRunner
                )
            );

            container.timedHandler->RegisterEvent(
                std::make_shared<PusherWebsocketProtocolHandler>(*container.websocket, *container.websocketProcessors),
                1
            );
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
