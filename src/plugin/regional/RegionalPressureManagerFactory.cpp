#include "pch/stdafx.h"
#include "regional/RegionalPressureManagerFactory.h"

using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin {
    namespace Regional {

        const std::string ASR_NAME_DEPENDENCY = "DEPENDENCY_ASR";

        std::shared_ptr<RegionalPressureManager> Create(DependencyLoaderInterface& dependency)
        {
            nlohmann::json asrData = dependency.LoadDependency(
                ASR_NAME_DEPENDENCY,
                nlohmann::json::array()
            );

            if (!asrData.is_array()) {
                LogWarning("Invalid ASR data format");
                return std::make_shared<RegionalPressureManager>();
            }

            std::map<std::string, std::string> asrKeys;
            for (nlohmann::json::const_iterator it = asrData.cbegin(); it != asrData.cend(); ++it) {
                if (!ValidAsr(it.value())) {
                    LogWarning("Invalid ASR provided");
                    continue;
                }

                asrKeys[it.value()["key"].get<std::string>()] = it.value()["name"].get<std::string>();
            }

            LogInfo("Loaded " + std::to_string(asrKeys.size()) + " altimeter setting regions");
            return std::make_shared<RegionalPressureManager>(asrKeys);
        }

        bool ValidAsr(const nlohmann::json& asr)
        {
            return asr.is_object() &&
                asr.count("key") &&
                asr.at("key").is_string() &&
                asr.count("name") &&
                asr.at("name").is_string();
        }

    }  // namespace Regional
}  // namespace UKControllerPlugin
