#pragma once

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            Struct representing a type of subscription on the websocket -
            to a particular channel or event.
        */
        typedef struct WebsocketSubscription
        {
            // The type of subscription - channel or event
            const std::string subType;

            // The target of the subscription
            const std::string subTarget;

            static const std::string SUB_TYPE_CHANNEL;
            static const std::string SUB_TYPE_EVENT;
            static const std::string SUB_TYPE_ALL;

            bool operator==(const WebsocketSubscription & compare) const;
            bool operator<(const WebsocketSubscription & compare) const;
            bool IsEventSubscription(void) const;
            bool IsChannelSubscription(void) const;
            bool IsAllSubscription(void) const;
        } WebsocketSubscription;
    }  // namespace Websocket
}  // namespace UKControllerPlugin
