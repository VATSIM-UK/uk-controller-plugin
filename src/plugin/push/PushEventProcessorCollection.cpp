#include "pch/pch.h"
#include "push/PushEventProcessorCollection.h"
#include "push/PushEventSubscription.h"
#include "push/PushEvent.h"
#include "push/PushEventProcessorInterface.h"

namespace UKControllerPlugin {
    namespace Push {

        /*
            Add a processor to the collection and register
            it for the channels it cares about.
        */
        void PushEventProcessorCollection::AddProcessor(
            std::shared_ptr<PushEventProcessorInterface> processor
        )
        {
            std::set<PushEventSubscription> events = processor->GetPushEventSubscriptions();


            for (auto it = events.cbegin(); it != events.cend(); ++it) {

                if (it->IsChannelSubscription()) {
                    if (!this->channelMap[it->subTarget].insert(processor).second) {
                        LogWarning("Attemped to add processor for duplicate channel: " + it->subTarget);
                    }

                    continue;
                }
                if (it->IsEventSubscription()) {
                    if (!this->eventMap[it->subTarget].insert(processor).second) {
                        LogWarning("Attemped to add processor for duplicate event: " + it->subTarget);
                    }

                    continue;
                }
                if (it->IsAllSubscription()) {
                    if (!this->globalEventProcessors.insert(processor).second) {
                        LogWarning("Attemped to add processor for duplicate all: " + it->subTarget);
                    }

                    continue;
                }

                LogWarning("Unknown subscription type " + it->subType);
            }

            this->allEventProcessors.insert(processor);
        }

        size_t PushEventProcessorCollection::CountProcessorsForChannel(std::string event) const
        {
            return this->channelMap.count(event) ? this->channelMap.at(event).size() : 0;
        }

        size_t PushEventProcessorCollection::CountProcessorsForEvent(std::string event) const
        {
            return this->eventMap.count(event) ? this->eventMap.at(event).size() : 0;
        }

        size_t PushEventProcessorCollection::CountProcessorsForAll() const
        {
            return this->globalEventProcessors.size();
        }

        /*
            Get all the channel subscriptions for the channel listeners
        */
        std::set<std::string> PushEventProcessorCollection::GetChannelSubscriptions(void) const
        {
            std::set<std::string> subscriptions;

            for (
                auto it
                    = this->channelMap.cbegin();
                it != this->channelMap.cend();
                ++it
            ) {
                subscriptions.insert(it->first);
            }

            return subscriptions;
        }

        /*
            Pass on the event to interested event processors. Only call event prccessor once.
        */
        void PushEventProcessorCollection::ProcessEvent(const PushEvent& message) const
        {
            std::set<std::shared_ptr<PushEventProcessorInterface>> calledProcessors;


            // Send the event to those listening on its channel
            if (this->channelMap.count(message.channel)) {
                for (
                    auto it
                        = this->channelMap.at(message.channel).cbegin();
                    it != this->channelMap.at(message.channel).cend();
                    ++it
                ) {
                    if (calledProcessors.count(*it)) {
                        continue;
                    }

                    calledProcessors.insert(*it);
                    (*it)->ProcessPushEvent(message);
                }
            }

            // Send the event to those that are listening specifically for it
            if (this->eventMap.count(message.event)) {
                for (
                    auto it
                        = this->eventMap.at(message.event).cbegin();
                    it != this->eventMap.at(message.event).cend();
                    ++it
                ) {
                    if (calledProcessors.count(*it)) {
                        continue;
                    }

                    calledProcessors.insert(*it);
                    (*it)->ProcessPushEvent(message);
                }
            }

            // Send the event to processors that want to know about everything
            for (
                auto it
                    = this->globalEventProcessors.cbegin();
                it != this->globalEventProcessors.cend();
                ++it
            ) {
                if (calledProcessors.count(*it)) {
                    continue;
                }

                calledProcessors.insert(*it);
                (*it)->ProcessPushEvent(message);
            }
        }

        /*
         * Let everyone know that the plugin events have been synced
         */
        void PushEventProcessorCollection::PluginEventsSynced() const
        {
            for (
                auto it = this->allEventProcessors.cbegin();
                it != this->allEventProcessors.cend();
                ++it
            ) {
                (*it)->PluginEventsSynced();
            }
        }
    } // namespace Push
} // namespace UKControllerPlugin
