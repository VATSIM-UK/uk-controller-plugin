#include "PollingPushEventConnection.h"
#include "ProxyPushDataSync.h"
#include "PushEventBootstrap.h"
#include "PushEventProtocolHandler.h"
#include "PushEventProxyConnection.h"
#include "PushEventProxyHandler.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin::Push {

    /*
        Bootstrap up the websocket.
    */
    void BootstrapPlugin(PersistenceContainer& container, bool duplicatePlugin)
    {
        std::shared_ptr<PushEventConnectionInterface> pushEvents;

        // Set up handler collection
        container.pushEventProcessors = std::make_shared<PushEventProcessorCollection>();

        // Create a websocket connection depending on whether we're the main plugin
        if (duplicatePlugin) {
            pushEvents = std::make_shared<PushEventProxyConnection>();
            container.timedHandler->RegisterEvent(
                std::make_shared<ProxyPushDataSync>(*container.pushEventProcessors), 5);
        } else {
            const auto pollingEvents = std::make_shared<PollingPushEventConnection>(
                *container.api, *container.taskRunner, *container.pushEventProcessors);
            pushEvents = pollingEvents;
            container.pushEventProcessors->AddProcessor(std::make_shared<PushEventProxyHandler>());
            container.timedHandler->RegisterEvent(pollingEvents, 1);
        }

        container.timedHandler->RegisterEvent(
            std::make_shared<PushEventProtocolHandler>(pushEvents, *container.pushEventProcessors), 1);
    }
} // namespace UKControllerPlugin::Push
