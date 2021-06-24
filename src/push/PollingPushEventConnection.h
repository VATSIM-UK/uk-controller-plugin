#pragma once
#include "push/PushEventConnectionInterface.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin {

    namespace Api {
        class ApiInterface;
    } // namespace Api

    namespace TaskManager {
        class TaskRunnerInterface;
    } // namespace TaskManager

    namespace Push {
        /*
            A class that imitates the pushed events connection through regular interval polling.
        */
        class PollingPushEventConnection : public PushEventConnectionInterface, public TimedEvent::AbstractTimedEvent
        {
            public:
                PollingPushEventConnection(
                    const Api::ApiInterface& api,
                    TaskManager::TaskRunnerInterface& taskRunner
                );

                // Inherited from WebsocketConnectionInterface
                void WriteMessage(std::string message) override;
                std::string GetNextMessage() override;
                void SetIdleTimeout(std::chrono::seconds timeout) override;
                void TimedEventTrigger() override;

                static bool SyncResponseValid(const nlohmann::json& response);
                static bool LatestPluginEventsResponseValid(const nlohmann::json& response);
                static bool PluginEventValid(const nlohmann::json& pluginEvent);

                std::chrono::system_clock::time_point LastPollTime() const;
                void SetLastPollTime(std::chrono::system_clock::time_point lastPollTime);
                void SetSyncInProgress();
                void SetUpdateInProgress();
                void SetSynced();
                int LastEventId() const;

                // How often we should poll for new updates
                const std::chrono::duration<int64_t> pollInterval = std::chrono::seconds(10);

            private:
                void SyncEvents();
                void GetLatestEvents();

                // Whether or not we're currently syncing events
                bool syncInProgress = false;

                // Whether or not we're currently in the process of retrieving the latest events
                bool updateInProgress = false;

                // The last event that we processed
                int lastEventId = -1;

                // The time that we last made an attempt to poll - default it to one second after the poll interval.
                std::chrono::system_clock::time_point lastPollTime = std::chrono::system_clock::now() -
                    pollInterval - std::chrono::seconds(1);

                // The api for polling for updates
                const Api::ApiInterface& api;

                // Allows polls to be run asynchronously
                TaskManager::TaskRunnerInterface& taskRunner;

                // Messages that are yet to be processed by the rest of the plugin
                std::queue<std::string> inboundMessages;

                // Protects the inbound messages queue
                std::mutex inboundMessageQueueGuard;
        };
    } // namespace Push
} // namespace UKControllerPlugin
