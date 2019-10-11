#pragma once
#include "websocket/WebsocketMessage.h"

const UKControllerPlugin::Websocket::WebsocketMessage invalidMessage = { "error_invalid", "error_invalid" };

UKControllerPlugin::Websocket::WebsocketMessage InterpretPusherMessage(std::string message);
