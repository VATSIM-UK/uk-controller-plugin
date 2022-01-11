#include "ProxyPushDataSync.h"
#include "PushEventProcessorCollection.h"

namespace UKControllerPlugin::Push {

    ProxyPushDataSync::ProxyPushDataSync(const PushEventProcessorCollection& processors)
        : processors(processors), synced(false)
    {
    }

    void ProxyPushDataSync::TimedEventTrigger()
    {
        if (synced) {
            return;
        }

        LogInfo("Syncing proxy push event data");
        processors.PluginEventsSynced();
        synced = true;
    }
} // namespace UKControllerPlugin::Push
