#include "pch/stdafx.h"
#include "hold/HoldModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyCache.h"
#include "hold/BuildHoldingData.h"
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
#include "tag/TagFunction.h"
#include "hold/HoldSelectionMenu.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
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
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Hold::HoldSelectionMenu;

namespace UKControllerPlugin {
    namespace Hold {

        const unsigned int popupMenuTagItemId = 9003;

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
            container.holdManager = BuildHoldingData(dependencies.GetJsonDependency(dependencyFile));

            // Create the object to manage the popup menu
            container.holdSelectionMenu = std::make_shared<HoldSelectionMenu>(
                *container.holdManager,
                *container.plugin,
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId()
            );

            // TAG function to trigger the popup menu
            TagFunction openHoldPopupMenu(
                popupMenuTagItemId,
                "Open Hold Selection Menu",
                std::bind(
                    &HoldSelectionMenu::DisplayMenu,
                    container.holdSelectionMenu.get(),
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openHoldPopupMenu);

            // Add the hold selection callback function for each hold in the collection
            unsigned int i = 0;
            while (i < container.holdManager->CountHolds()) {
                CallbackFunction holdSelectionCallback(
                    container.pluginFunctionHandlers->ReserveNextDynamicFunctionId(),
                    "Hold Selection",
                    std::bind(
                        &HoldSelectionMenu::MenuItemClicked,
                        container.holdSelectionMenu,
                        std::placeholders::_1,
                        std::placeholders::_2
                    )
                );
                container.pluginFunctionHandlers->RegisterFunctionCall(holdSelectionCallback);
                i++;
            }
            container.holdSelectionMenu->AddHoldToMenu(1);
            container.holdSelectionMenu->AddHoldToMenu(2);

            // Create the event handler and register
            container.holdWindows = std::make_unique<HoldWindowManager>(
                GetActiveWindow(),
                container.windows->GetDllInstance(),
                *container.holdManager,
                *container.plugin
            );

            eventHandler = std::make_shared<HoldEventHandler>(
                *container.holdManager,
                *container.plugin,
                *container.holdWindows,
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId()
            );

            container.flightplanHandler->RegisterHandler(eventHandler);
            container.timedHandler->RegisterEvent(eventHandler, timedEventFrequency);
            container.commandHandlers->RegisterHandler(eventHandler);

            CallbackFunction openWindowCallback(
                eventHandler->popupMenuItemId,
                "Show Hold Manager",
                std::bind(&HoldEventHandler::Configure, eventHandler, std::placeholders::_1, std::placeholders::_2)
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openWindowCallback);

            // If there aren't any holds, tell the user this explicitly
            if (container.holdManager->CountHolds() == 0) {
                BootstrapWarningMessage warning("No holds were loaded for the hold manager");
                userMessages.SendMessageToUser(warning);
            }
        }

        /*
            Register the handler to make it configurable from the menu
        */
        void BootstrapRadarScreen(
            ConfigurableDisplayCollection & configurableDisplay
        ) {
            configurableDisplay.RegisterDisplay(eventHandler);
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
