#pragma once
#include "push/PushEventConnectionInterface.h"
#include "push/PushEventProcessorInterface.h"

namespace UKControllerPlugin::Push {
    /**
     *  Handles push events proxied to a secondary plugin
     *  from the primary instance.
     */
    class PushEventProxyHandler : public PushEventProcessorInterface
    {
        public:
        void ProcessPushEvent(const PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const -> std::set<PushEventSubscription> override;
        void PluginEventsSynced() override;

        private:
        static auto CALLBACK EnumerateWindows(HWND hwnd, LPARAM lparam) -> BOOL;

        static const int WINDOW_NAME_BUFFER_SIZE = 1000;
    };
} // namespace UKControllerPlugin::Push
