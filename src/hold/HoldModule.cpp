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
#include "hold/HoldWindowManager.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "euroscope/CallbackFunction.h"
#include "hold/HoldDependency.h"
#include "api/ApiHelper.h"
#include "windows/WinApiInterface.h"
#include "dependency/DependencyCache.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Hold::HoldingDataCollection;
using UKControllerPlugin::Hold::HoldEventHandler;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Hold::HoldWindowManager;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Hold::UpdateHoldDependency;
using UKControllerPlugin::Hold::GetLocalHoldData;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Dependency::DependencyCache;

namespace UKControllerPlugin {
    namespace Hold {

        // The file where the holds are stored locally
        const std::string dependencyFile = "holds.json";

        // How often holds should be updated
        const int timedEventFrequency = 5;

        // The event handler
        std::shared_ptr<HoldEventHandler> eventHandler;

        /*
            Update and load the hold dependencies from the API
        */
        void LoadDependencies(
            DependencyCache * const dependencies,
            const ApiInterface & webApi, 
            WinApiInterface & windowsApi
        ) {
            UpdateHoldDependency(webApi, windowsApi);
            dependencies->AddJsonDependency(dependencyFile, GetLocalHoldData(windowsApi));
        }

        /*
            Bootstrap the module into the plugin
        */
        void BootstrapPlugin(
            const DependencyCache & dependencies,
            PersistenceContainer & container,
            UserMessager & userMessages
        ) {
            // Update local dependencies and build hold data
            container.holds = std::make_unique<HoldingDataCollection>(
                BuildHoldingData(dependencies.GetJsonDependency(dependencyFile))
            );

            container.holdManager.reset(new HoldManager);

            // Create the event handler and register
            eventHandler = std::make_shared<HoldEventHandler>(
                *container.holdManager,
                *container.plugin,
                HoldWindowManager(GetActiveWindow(), container.windows->GetDllInstance()),
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
                std::bind(&HoldEventHandler::Configure, eventHandler, std::placeholders::_1)
            );
            functionCalls.RegisterFunctionCall(openWindowCallback);
            configurableDisplay.RegisterDisplay(eventHandler);
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
