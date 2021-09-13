#include "CreateWakeMappings.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "message/UserMessager.h"

using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Wake::WakeCategoryMapper;

namespace UKControllerPlugin::Wake {

    /*
        Create the mapper from JSON data
    */
    auto CreateWakeMappings(const nlohmann::json& jsonData, UserMessager& messager, const std::string type)
        -> WakeCategoryMapper
    {
        WakeCategoryMapper mapper;
        int errorCount = 0;
        for (nlohmann::json::const_iterator it = jsonData.cbegin(); it != jsonData.cend(); ++it) {
            if (!it.value().is_string()) {
                errorCount++;
                LogError("Invalid " + type + " category for type " + it.key());
                continue;
            }

            mapper.AddCategoryMapping(it.key(), it.value());
        }

        if (errorCount != 0) {
            BootstrapWarningMessage message(
                "Failed to load " + std::to_string(errorCount) + " " + type + " categories");
            messager.SendMessageToUser(message);
        }

        LogInfo("Loaded " + std::to_string(mapper.Count()) + " " + type + " categories");
        return mapper;
    }
} // namespace UKControllerPlugin::Wake
