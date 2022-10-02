#include "push/PushEventSubscription.h"

namespace UKControllerPlugin {
    namespace Push {

        const std::string PushEventSubscription::SUB_TYPE_CHANNEL = "channel";
        const std::string PushEventSubscription::SUB_TYPE_EVENT = "event";
        const std::string PushEventSubscription::SUB_TYPE_ALL = "all";

        bool PushEventSubscription::operator==(const PushEventSubscription& compare) const
        {
            return this->subType == compare.subType && this->subTarget == compare.subTarget;
        }

        bool PushEventSubscription::operator<(const PushEventSubscription& compare) const
        {
            return this->subType == compare.subType ? this->subTarget < compare.subTarget
                                                    : this->subType < compare.subType;
        }

        bool PushEventSubscription::IsEventSubscription(void) const
        {
            return this->subType == this->SUB_TYPE_EVENT;
        }

        bool PushEventSubscription::IsChannelSubscription(void) const
        {
            return this->subType == this->SUB_TYPE_CHANNEL;
        }

        bool PushEventSubscription::IsAllSubscription(void) const
        {
            return this->subType == this->SUB_TYPE_ALL;
        }
    } // namespace Push
} // namespace UKControllerPlugin
