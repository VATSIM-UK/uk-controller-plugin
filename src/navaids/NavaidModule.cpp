#include "pch/stdafx.h"
#include "navaids/NavaidModule.h"
#include "navaids/NavaidCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin {
    namespace Navaids {

        void BootstrapPlugin(
            PersistenceContainer& container,
            DependencyLoaderInterface& dependencies
        ) {
            container.navaids = std::make_unique<NavaidCollection>();

            nlohmann::json navaidData = dependencies.LoadDependency("DEPENDENCY_NAVAIDS", "{}"_json);

            if (!navaidData.is_object()) {
                LogWarning("Navaid data is invalid");
            }

            for (
                nlohmann::json::const_iterator it = navaidData.cbegin();
                it != navaidData.cend();
                ++it
            ) {
                if (!NavaidValid(*it)) {
                    LogWarning("Invalid navaid detected " + it->dump());
                    continue;
                }

                container.navaids->AddNavaid(
                    { 
                        it->at("id").get<int>(),
                        it->at("identifier").get<std::string>(),
                        it->at("type").get<std::string>()
                    }
                );
            }

            LogInfo("Loaded " + std::to_string(container.navaids->Count()) + " navaids");
        }

        bool NavaidValid(const nlohmann::json& navaid)
        {
            return navaid.is_object() &&
                navaid.contains("id") &&
                navaid.at("id").is_number_integer() &&
                navaid.contains("type") &&
                navaid.at("type").is_string() &&
                (
                    navaid.at("type").get<std::string>() == "VOR" ||
                    navaid.at("type").get<std::string>() == "NDB" ||
                    navaid.at("type").get<std::string>() == "FIX"
                ) &&
                navaid.contains("identifier") && 
                navaid.at("identifier").is_string();
        }
    }  // namespace Navaids
}  // namespace UKControllerPlugin
