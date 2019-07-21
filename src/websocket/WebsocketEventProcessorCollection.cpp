#include "pch/stdafx.h"
#include "WebsocketEventProcessorCollection.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            Add a processor to the collection and register
            it for the channels it cares about.
        */
        void WebsocketEventProcessorCollection::AddProcessor(
            std::shared_ptr<WebsocketEventProcessorInterface> processor
        ) {
            std::set<std::string> events = processor->GetSubscriptions();

            for (std::set<std::string>::const_iterator it = events.cbegin(); it != events.cend(); ++it) {
                if (!this->channelMap[*it].insert(processor).second) {
                    LogWarning("Attemped to add processor for duplicate event: " + *it);
                }
            }
        }

        /*
            Add a processor to the collection that handles protocol messages and
            register it for the events it listens for.
        */
        void WebsocketEventProcessorCollection::AddProtocolProcessor(
            std::shared_ptr<WebsocketProtocolProcessorInterface> processor
        ) {
            std::set<std::string> events = processor->GetEventSubscriptions();

            for (std::set<std::string>::const_iterator it = events.cbegin(); it != events.cend(); ++it) {
                if (!this->protocolEventMap[*it].insert(processor).second) {
                    LogWarning("Attemped to add processor for duplicate protocol event: " + *it);
                }
            }
        }

        size_t WebsocketEventProcessorCollection::CountProcessorsForChannel(std::string event) const
        {
            return this->channelMap.count(event) ? this->channelMap.at(event).size() : 0;
        }

        size_t WebsocketEventProcessorCollection::CountProcessorsForProtocolEvent(std::string event) const
        {
            return this->protocolEventMap.count(event) ? this->protocolEventMap.at(event).size() : 0;
        }

        /*
            Get all the channel subscriptions for the channel listeners
        */
        std::set<std::string> WebsocketEventProcessorCollection::GetAllSubscriptions(void) const
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
            Pass on the event to interested listeners
        */
        void WebsocketEventProcessorCollection::ProcessEvent(const WebsocketMessage & message) const
        {
            if (!this->channelMap.count(message.channel)) {
                return;
            }

            for (
                std::set<std::shared_ptr<WebsocketEventProcessorInterface>>::const_iterator it
                    = this->channelMap.at(message.channel).cbegin();
                it != this->channelMap.at(message.channel).cend();
                ++it
            ) {
                (*it)->ProcessWebsocketMessage(message);
            }
        }

        /*
            Process protocol events
        */
        void WebsocketEventProcessorCollection::ProcessProtocolEvent(const WebsocketMessage & message) const
        {
            if (!this->protocolEventMap.count(message.event)) {
                return;
            }

            for (
                std::set<std::shared_ptr<WebsocketProtocolProcessorInterface>>::const_iterator it
                    = this->protocolEventMap.at(message.event).cbegin();
                it != this->protocolEventMap.at(message.event).cend();
                ++it
            ) {
                (*it)->ProcessProtocolMessage(message);
            }
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
