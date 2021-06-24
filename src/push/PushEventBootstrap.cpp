#include "pch/stdafx.h"
#include "push/PushEventBootstrap.h"
#include "push/PushEventProtocolHandler.h"
#include "push/PushEventProxyConnection.h"
#include "push/PollingPushEventConnection.h"
#include "push/PushEventProxyHandler.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Push {

        std::shared_ptr<PushEventConnectionInterface> pushEvents;

        /*
            Bootstrap up the websocket.
        */
        void BootstrapPlugin(PersistenceContainer& container, bool duplicatePlugin)
        {
            // Connect to websocket
            std::string wsHost = container.settingsRepository->HasSetting("websocket_host")
                                     ? container.settingsRepository->GetSetting("websocket_host")
                                     : "ukcp.vatsim.uk";

            std::string wsPort = container.settingsRepository->HasSetting("websocket_port")
                                     ? container.settingsRepository->GetSetting("websocket_port")
                                     : "6001";

            // Set up handler collection
            container.pushEventProcessors.reset(new PushEventProcessorCollection);

            // Create a websocket connection depending on whether we're the main plugin
            if (duplicatePlugin) {
                pushEvents = std::make_shared<PushEventProxyConnection>();
            } else {
                auto pollingEvents = std::make_shared<PollingPushEventConnection>(
                    *container.api,
                    *container.taskRunner
                );
                pushEvents = pollingEvents;
                container.pushEventProcessors->AddProcessor(std::make_shared<PushEventProxyHandler>());
                container.timedHandler->RegisterEvent(pollingEvents, 1);
            }

            container.timedHandler->RegisterEvent(
                std::make_shared<PushEventProtocolHandler>(*pushEvents, *container.pushEventProcessors),
                1
            );
        }
    } // namespace Push
} // namespace UKControllerPlugin
