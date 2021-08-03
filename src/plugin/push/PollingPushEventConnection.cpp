#include "pch/pch.h"
#include "push/PollingPushEventConnection.h"

#include "PushEventProcessorCollection.h"
#include "api/ApiInterface.h"
#include "task/TaskRunnerInterface.h"
#include "api/ApiException.h"

namespace UKControllerPlugin {
    namespace Push {

        PollingPushEventConnection::PollingPushEventConnection(
            const Api::ApiInterface& api,
            TaskManager::TaskRunnerInterface& taskRunner,
            const PushEventProcessorCollection& pushEventHandlers
        ): api(api), taskRunner(taskRunner), pushEventHandlers(pushEventHandlers) { }

        void PollingPushEventConnection::WriteMessage(std::string message)
        {
            // Nothing to do here
        }

        std::string PollingPushEventConnection::GetNextMessage()
        {
            std::lock_guard<std::mutex> lock(this->inboundMessageQueueGuard);

            if (this->inboundMessages.empty()) {
                return this->noMessage;
            }

            std::string message = this->inboundMessages.front();
            this->inboundMessages.pop();
            return message;
        }

        void PollingPushEventConnection::TimedEventTrigger()
        {
            // We've polled recently for something, so let's not do it again.
            if (std::chrono::system_clock::now() - this->lastPollTime < this->pollInterval) {
                return;
            }

            // Either sync, or get latest events, depending on whether we've synced successfully
            if (this->lastEventId == -1) {
                this->SyncEvents();
            } else {
                this->GetLatestEvents();
            }
        }

        /*
         * Get the latest event id from the API so we can sync up the plugin
         * to only receive the events that it needs.
         */
        void PollingPushEventConnection::SyncEvents()
        {
            if (this->syncInProgress) {
                return;
            }

            this->syncInProgress = true;
            this->taskRunner.QueueAsynchronousTask(
                [this]()
                {
                    try {
                        nlohmann::json syncResponse = this->api.SyncPluginEvents();

                        if (!this->SyncResponseValid(syncResponse)) {
                            this->lastPollTime = std::chrono::system_clock::now();
                            LogWarning("Invalid plugin event sync response from API");
                            this->updateInProgress = false;
                            return;
                        }

                        this->lastEventId = syncResponse.at("event_id").get<int>();
                        LogInfo("Plugin events synced at id " + std::to_string(this->lastEventId));

                        // Let everyone know that events are synced
                        this->pushEventHandlers.PluginEventsSynced();
                    } catch (Api::ApiException apiException) {
                        LogError("ApiException when syncing plugin events: -" + std::string(apiException.what()));
                        this->lastPollTime = std::chrono::system_clock::now();
                    }
                    this->syncInProgress = false;
                }
            );
        }

        /*
         * Given our current 
         */
        void PollingPushEventConnection::GetLatestEvents()
        {
            if (this->updateInProgress) {
                return;
            }

            this->updateInProgress = true;
            this->taskRunner.QueueAsynchronousTask(
                [this]()
                {
                    try {
                        nlohmann::json latestEventsResponse = this->api.GetLatestPluginEvents(this->lastEventId);

                        if (!LatestPluginEventsResponseValid(latestEventsResponse)) {
                            LogWarning("Invalid plugin events response from API");
                            this->lastPollTime = std::chrono::system_clock::now();
                            this->updateInProgress = false;
                            return;
                        }

                        // Push the event to the inbound message queue
                        std::lock_guard lock(this->inboundMessageQueueGuard);
                        for (
                            auto eventIterator = latestEventsResponse.cbegin();
                            eventIterator != latestEventsResponse.cend();
                            ++eventIterator
                        ) {
                            this->inboundMessages.push(eventIterator->at("event").dump());

                            if (eventIterator->at("id").get<int>() > this->lastEventId) {
                                this->lastEventId = eventIterator->at("id").get<int>();
                            }

                            LogDebug("Received websocket message: " + eventIterator->dump());
                        }


                        LogDebug("Plugin events updated, last event id is now " + std::to_string(this->lastEventId));

                    } catch (Api::ApiException apiException) {
                        LogError("ApiException when getting latest plugin events: " + std::string(apiException.what()));
                    }
                    this->lastPollTime = std::chrono::system_clock::now();
                    this->updateInProgress = false;
                }
            );
        }

        bool PollingPushEventConnection::SyncResponseValid(const nlohmann::json& response)
        {
            return response.is_object() &&
                response.contains("event_id") &&
                response.at("event_id").is_number_integer();
        }

        bool PollingPushEventConnection::LatestPluginEventsResponseValid(const nlohmann::json& response)
        {
            return response.is_array();
        }

        bool PollingPushEventConnection::PluginEventValid(const nlohmann::json& pluginEvent)
        {
            return pluginEvent.is_object() &&
                pluginEvent.contains("id") &&
                pluginEvent.at("id").is_number_integer() &&
                pluginEvent.contains("event") &&
                pluginEvent.at("event").is_object() &&
                pluginEvent.at("event").contains("data") &&
                pluginEvent.at("event").at("data").is_object();
        }

        std::chrono::system_clock::time_point PollingPushEventConnection::LastPollTime() const
        {
            return this->lastPollTime;
        }

        void PollingPushEventConnection::SetLastPollTime(std::chrono::system_clock::time_point lastPollTime)
        {
            this->lastPollTime = lastPollTime;
        }

        void PollingPushEventConnection::SetSyncInProgress()
        {
            this->syncInProgress = true;
        }

        void PollingPushEventConnection::SetUpdateInProgress()
        {
            this->updateInProgress = true;
        }

        void PollingPushEventConnection::SetSynced()
        {
            this->lastEventId = 0;
        }

        int PollingPushEventConnection::LastEventId() const
        {
            return this->lastEventId;
        }
    } // namespace Push
} // namespace UKControllerPlugin
