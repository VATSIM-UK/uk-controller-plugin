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
#include "hold/HoldWindow.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "euroscope/CallbackFunction.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Hold::HoldingDataCollection;
using UKControllerPlugin::Hold::HoldEventHandler;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Hold::HoldWindow;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;

namespace UKControllerPlugin {
    namespace Hold {

        const std::string dependencyFile = "arrival-holds.json";

        const int timedEventFrequency = 5;

        std::shared_ptr<HoldEventHandler> eventHandler;

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

            // Create the event handler and register
            eventHandler = std::make_shared<HoldEventHandler>(
                *container.holdManager,
                *container.plugin,
                HoldWindow(GetActiveWindow(), container.windows->GetDllInstance()),
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId()
            );

            container.flightplanHandler->RegisterHandler(eventHandler);
            container.timedHandler->RegisterEvent(eventHandler, timedEventFrequency);
            container.commandHandlers->RegisterHandler(eventHandler);

            // If there aren't any holds, tell the user this explicitly
            if (container.holds->Count() == 0) {
                BootstrapWarningMessage warning("No holds were loaded for the hold manager");
                userMessages.SendMessageToUser(warning);
            }
        }

        /*
            Register the handler to make it configurable from the menu
        */
        void BootstrapRadarScreen(
            FunctionCallEventHandler & functionCalls,
            ConfigurableDisplayCollection & configurableDisplay
        ) {
            CallbackFunction openWindowCallback(
                eventHandler->popupMenuItemId,
                "Show Hold Manager",
                std::bind(&HoldEventHandler::Configure, *eventHandler, std::placeholders::_1)
            );
            functionCalls.RegisterFunctionCall(openWindowCallback);
            configurableDisplay.RegisterDisplay(eventHandler);
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
