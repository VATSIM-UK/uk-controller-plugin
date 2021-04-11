#include "pch/stdafx.h"
#include "websocket/PollingWebsocketConnection.h"
#include "api/ApiInterface.h"
#include "task/TaskRunnerInterface.h"
#include "api/ApiException.h"

namespace UKControllerPlugin {
    namespace Websocket {

        PollingWebsocketConnection::PollingWebsocketConnection(
            const Api::ApiInterface& api,
            TaskManager::TaskRunnerInterface& taskRunner
        ): api(api), taskRunner(taskRunner) { }

        void PollingWebsocketConnection::WriteMessage(std::string message)
        {
            // Nothing to do here
        }

        std::string PollingWebsocketConnection::GetNextMessage()
        {
            std::lock_guard<std::mutex> lock(this->inboundMessageQueueGuard);

            if (this->inboundMessages.empty()) {
                return this->noMessage;
            }

            std::string message = this->inboundMessages.front();
            this->inboundMessages.pop();
            return message;
        }

        void PollingWebsocketConnection::SetIdleTimeout(std::chrono::seconds timeout)
        {
            // Nothing to do here
        }

        void PollingWebsocketConnection::TimedEventTrigger()
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
        void PollingWebsocketConnection::SyncEvents()
        {
            if (this->syncInProgress) {
                return;
            }

            this->taskRunner.QueueAsynchronousTask(
                [this]()
                {
                    this->syncInProgress = true;
                    try {
                        nlohmann::json syncResponse = this->api.SyncPluginEvents();

                        if (!this->SyncResponseValid(syncResponse)) {
                            this->lastPollTime = std::chrono::system_clock::now();
                            LogWarning("Invalid plugin event sync response from API");
                            return;
                        }

                        this->lastEventId = syncResponse.at("event_id").get<int>();
                        LogInfo("Plugin events synced at id " + std::to_string(this->lastEventId));

                        // Put a fake connection established message on the message queue so it's picked up by handlers
                        this->inboundMessages.push(
                            nlohmann::json({
                                {"event", "pusher:connection_established"},
                                {"data", nlohmann::json({{"socket_id", "abc.def"}, {"activity_timeout", 30}}).dump()}
                            }).dump()
                        );
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
        void PollingWebsocketConnection::GetLatestEvents()
        {
            if (this->updateInProgress) {
                return;
            }

            this->taskRunner.QueueAsynchronousTask(
                [this]()
                {
                    this->updateInProgress = true;
                    try {
                        nlohmann::json latestEventsResponse = this->api.GetLatestPluginEvents(this->lastEventId);

                        if (!this->LatestPluginEventsResponseValid(latestEventsResponse)) {
                            LogWarning("Invalid plugin events response from API");
                            this->lastPollTime = std::chrono::system_clock::now();
                            return;
                        }

                        std::lock_guard<std::mutex> lock(this->inboundMessageQueueGuard);

                        // Pusher does a thing where the data is JSON encoded within JSON
                        // For now, mimic this behaviour.
                        // TODO(AndyTWF): Once we're happy with this implementation, remove the old and refactor this.
                        for (
                            auto eventIterator = latestEventsResponse.begin();
                            eventIterator != latestEventsResponse.end();
                            ++eventIterator
                        ) {
                            eventIterator->at("event").at("data") = eventIterator->at("event").at("data").dump();
                            this->inboundMessages.push(eventIterator->at("event").dump());

                            int eventId = eventIterator->at("id").get<int>();
                            if (eventId > this->lastEventId) {
                                this->lastEventId = eventId;
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

        bool PollingWebsocketConnection::SyncResponseValid(const nlohmann::json& response)
        {
            return response.is_object() &&
                response.contains("event_id") &&
                response.at("event_id").is_number_integer();
        }

        bool PollingWebsocketConnection::LatestPluginEventsResponseValid(const nlohmann::json& response)
        {
            return response.is_array();
        }

        bool PollingWebsocketConnection::PluginEventValid(const nlohmann::json& pluginEvent)
        {
            return pluginEvent.is_object() &&
                pluginEvent.contains("id") &&
                pluginEvent.at("id").is_number_integer() &&
                pluginEvent.contains("event") &&
                pluginEvent.at("event").is_object() &&
                pluginEvent.at("event").contains("data") &&
                pluginEvent.at("event").at("data").is_object();
        }

        std::chrono::system_clock::time_point PollingWebsocketConnection::LastPollTime() const
        {
            return this->lastPollTime;
        }

        void PollingWebsocketConnection::SetLastPollTime(std::chrono::system_clock::time_point lastPollTime)
        {
            this->lastPollTime = lastPollTime;
        }

        void PollingWebsocketConnection::SetSyncInProgress()
        {
            this->syncInProgress = true;
        }

        void PollingWebsocketConnection::SetUpdateInProgress()
        {
            this->updateInProgress = true;
        }

        void PollingWebsocketConnection::SetSynced()
        {
            this->lastEventId = 0;
        }

        int PollingWebsocketConnection::LastEventId() const
        {
            return this->lastEventId;
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
