#include "pch/stdafx.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "websocket/WebsocketSubscription.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            Add a processor to the collection and register
            it for the channels it cares about.
        */
        void WebsocketEventProcessorCollection::AddProcessor(
            std::shared_ptr<WebsocketEventProcessorInterface> processor
        ) {
            std::set<WebsocketSubscription> events = processor->GetSubscriptions();


            for (std::set<WebsocketSubscription>::const_iterator it = events.cbegin(); it != events.cend(); ++it) {

                if (it->IsChannelSubscription()) {
                    if (!this->channelMap[it->subTarget].insert(processor).second) {
                        LogWarning("Attemped to add processor for duplicate channel: " + it->subTarget);
                    }

                    continue;
                } else if (it->IsEventSubscription()) {
                    if (!this->eventMap[it->subTarget].insert(processor).second) {
                        LogWarning("Attemped to add processor for duplicate event: " + it->subTarget);
                    }

                    continue;
                }

                LogWarning("Unknown subscription type " + it->subType);
            }
        }

        size_t WebsocketEventProcessorCollection::CountProcessorsForChannel(std::string event) const
        {
            return this->channelMap.count(event) ? this->channelMap.at(event).size() : 0;
        }

        size_t WebsocketEventProcessorCollection::CountProcessorsForEvent(std::string event) const
        {
            return this->eventMap.count(event) ? this->eventMap.at(event).size() : 0;
        }

        /*
            Get all the channel subscriptions for the channel listeners
        */
        std::set<std::string> WebsocketEventProcessorCollection::GetChannelSubscriptions(void) const
        {
            std::set<std::string> subscriptions;

            for (
                std::map<std::string, std::set<std::shared_ptr<WebsocketEventProcessorInterface>>>::const_iterator it
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
        void WebsocketEventProcessorCollection::ProcessEvent(const WebsocketMessage & message) const
        {
            std::set<std::shared_ptr<WebsocketEventProcessorInterface>> calledProcessors;


            // Send the event to those listening on its channel
            if (this->channelMap.count(message.channel)) {
                for (
                    std::set<std::shared_ptr<WebsocketEventProcessorInterface>>::const_iterator it
                    = this->channelMap.at(message.channel).cbegin();
                    it != this->channelMap.at(message.channel).cend();
                    ++it
                ) {
                    if (calledProcessors.count(*it)) {
                        continue;
                    }

                    calledProcessors.insert(*it);
                    (*it)->ProcessWebsocketMessage(message);
                }
            }

            // Send the event to those that are listening specifically for it
            if (this->eventMap.count(message.event)) {
                for (
                    std::set<std::shared_ptr<WebsocketEventProcessorInterface>>::const_iterator it
                    = this->eventMap.at(message.event).cbegin();
                    it != this->eventMap.at(message.event).cend();
                    ++it
                ) {
                    if (calledProcessors.count(*it)) {
                        continue;
                    }

                    calledProcessors.insert(*it);
                    (*it)->ProcessWebsocketMessage(message);
                }
            }
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
