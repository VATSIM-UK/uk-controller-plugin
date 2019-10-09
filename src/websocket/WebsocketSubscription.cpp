#pragma once
#include "pch/stdafx.h"
#include "websocket/WebsocketSubscription.h"

namespace UKControllerPlugin {
    namespace Websocket {

        const std::string WebsocketSubscription::SUB_TYPE_CHANNEL = "channel";
        const std::string WebsocketSubscription::SUB_TYPE_EVENT = "event";

        bool WebsocketSubscription::operator==(const WebsocketSubscription & compare) const
        {
            return this->subType == compare.subType && this->subTarget == compare.subTarget;
        }

        bool WebsocketSubscription::operator<(const WebsocketSubscription & compare) const
        {
            return this->subType == compare.subType
                ? this->subTarget < compare.subTarget
                : this->subType < compare.subType;
        }

        bool WebsocketSubscription::IsEventSubscription(void) const
        {
            return this->subType == this->SUB_TYPE_EVENT;
        }

        bool WebsocketSubscription::IsChannelSubscription(void) const
        {
            return this->subType == this->SUB_TYPE_CHANNEL;
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
