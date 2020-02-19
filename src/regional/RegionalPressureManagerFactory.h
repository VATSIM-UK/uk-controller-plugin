#pragma once
#include "regional/RegionalPressureManager.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin {
    namespace Regional {

        extern const std::string ASR_NAME_DEPENDENCY;

        std::shared_ptr <UKControllerPlugin::Regional::RegionalPressureManager> Create(
            UKControllerPlugin::Dependency::DependencyLoaderInterface & dependency
        );

        bool ValidAsr(const nlohmann::json& asr);
    }  // namespace Regional
}  // namespace UKControllerPlugin
