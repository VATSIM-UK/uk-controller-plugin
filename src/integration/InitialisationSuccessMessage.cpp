#include "pch/stdafx.h"
#include "integration/InitialisationSuccessMessage.h"
#include "update/PluginVersion.h"

namespace UKControllerPlugin::Integration {

    nlohmann::json InitialisationSuccessMessage::GetMessageData() const
    {
        return {{"ukcp_version", Plugin::PluginVersion::version}};
    }

    MessageType InitialisationSuccessMessage::GetMessageType() const
    {
        return {
            "initialisation_success",
            1
        };
    }
} // namespace UKControllerPlugin::Integration
