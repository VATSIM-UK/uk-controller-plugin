#include "pch/stdafx.h"
#include "integration/InitialistionSuccessMessage.h"
#include "update/PluginVersion.h"

namespace UKControllerPlugin::Integration {

    nlohmann::json InitialisationSuccessMessage::GetMessageData() const
    {
        return nlohmann::json::object({
            {
                "data", nlohmann::json::object({
                    {"ukcp_version", Plugin::PluginVersion::version}
                })
            }
        });
    }

    MessageType InitialisationSuccessMessage::GetMessageType() const
    {
        return {
            "initialisation_success",
            1
        };
    }
} // namespace UKControllerPlugin::Integration
