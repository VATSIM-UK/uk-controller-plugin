#include "pch/stdafx.h"
#include "hold/HoldModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyCache.h"
#include "hold/BuildHoldingData.h"
#include "hold/HoldingDataCollection.h"
#include "hold/HoldEventHandler.h"
#include "hold/HoldManager.h"
#include "message/UserMessager.h"
#include "bootstrap/BootstrapWarningMessage.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Hold::HoldingDataCollection;
using UKControllerPlugin::Hold::HoldEventHandler;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;

namespace UKControllerPlugin {
    namespace Hold {

        const std::string dependencyFile = "arrival-holds.json";

        const int timedEventFrequency = 5;

        /*
            Bootstrap the module into the plugin
        */
        void BootstrapPlugin(
            PersistenceContainer & container,
            const DependencyCache & dependencies,
            UserMessager & userMessages
        ) {
            nlohmann::json dependencyJson;
            try {
                if (dependencies.HasDependency(dependencyFile)) {
                    dependencyJson = nlohmann::json::parse(dependencies.GetDependency(dependencyFile));
                } else {
                    LogWarning("Dependency file for holds not found");
                }
            } catch (nlohmann::json::exception) {
                LogError("Unable to parse dependency file for holding data");
            }

            container.holds = std::make_unique<HoldingDataCollection>(BuildHoldingData(dependencyJson));
            container.holdManager.reset(new HoldManager);

            std::shared_ptr<HoldEventHandler> eventHandler = std::make_shared<HoldEventHandler>(
                *container.holdManager,
                *container.plugin
            );

            container.flightplanHandler->RegisterHandler(eventHandler);
            container.timedHandler->RegisterEvent(eventHandler, timedEventFrequency);

            // If there aren't any holds, tell the user this explicitly
            if (container.holds->Count() == 0) {
                BootstrapWarningMessage warning("No holds were loaded for the hold manager");
                userMessages.SendMessageToUser(warning);
            }
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
