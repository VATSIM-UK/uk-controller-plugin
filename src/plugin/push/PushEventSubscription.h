#pragma once

namespace UKControllerPlugin {
    namespace Push {

        /*
            Struct representing a type of subscription for push events from the API.
            Either a subscription to a particular channel, or just certain events.
        */
        using PushEventSubscription = struct PushEventSubscription
        {
            // The type of subscription - channel or event
            const std::string subType;

            // The target of the subscription
            const std::string subTarget;

            static const std::string SUB_TYPE_CHANNEL;
            static const std::string SUB_TYPE_EVENT;
            static const std::string SUB_TYPE_ALL;

            bool operator==(const PushEventSubscription& compare) const;
            bool operator<(const PushEventSubscription& compare) const;
            bool IsEventSubscription(void) const;
            bool IsChannelSubscription(void) const;
            bool IsAllSubscription(void) const;
        };
    } // namespace Push
} // namespace UKControllerPlugin
