#include "pch/stdafx.h"
#include "hold/HoldModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyCache.h"
#include "hold/HoldManagerFactory.h"
#include "hold/HoldEventHandler.h"
#include "hold/HoldManager.h"
#include "message/UserMessager.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "hold/HoldWindowManager.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "euroscope/CallbackFunction.h"
#include "api/ApiHelper.h"
#include "windows/WinApiInterface.h"
#include "dependency/DependencyProviderInterface.h"
#include "tag/TagFunction.h"
#include "hold/HoldSelectionMenu.h"
#include "hold/HoldConfigurationDialog.h"
#include "dialog/DialogData.h"
#include "dependency/DependencyConfig.h"
#include "hold/HoldManagerFactory.h"
#include "hold/HoldConfigurationDialogFactory.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Hold::HoldEventHandler;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Hold::HoldWindowManager;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Dependency::DependencyProviderInterface;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Hold::HoldSelectionMenu;
using UKControllerPlugin::Hold::HoldConfigurationDialog;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dependency::DependencyConfig;
using UKControllerPlugin::Hold::CreateHoldManager;
using UKControllerPlugin::Hold::CreateHoldConfigurationDialog;

namespace UKControllerPlugin {
    namespace Hold {

        // The id of the tag item for selected hold
        const int selectedHoldTagItemId = 106;

        // The id of the popup menu tag function
        const unsigned int popupMenuTagItemId = 9003;

        // How often holds should be updated
        const int timedEventFrequency = 5;

        // The event handler
        std::shared_ptr<HoldEventHandler> eventHandler;

        /*
            Bootstrap the module into the plugin
        */
        void BootstrapPlugin(
            const DependencyProviderInterface & dependencyProvider,
            PersistenceContainer & container,
            UserMessager & userMessages
        ) {
            // Update local dependencies and build hold data
            nlohmann::json holdDependency = dependencyProvider.GetDependency(DependencyConfig::holds);
            container.holdManager = CreateHoldManager(holdDependency);

            // Create the object to manage the popup menu
            int holdSelectionCancelId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            container.holdSelectionMenu = std::make_shared<HoldSelectionMenu>(
                *container.holdManager,
                *container.plugin,
                holdSelectionCancelId
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

            // The selection cancel function takes the base id
            CallbackFunction holdSelectionCancelCallback(
                holdSelectionCancelId,
                "Hold Selection Cancel",
                std::bind(
                    &HoldSelectionMenu::MenuItemClicked,
                    container.holdSelectionMenu,
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(holdSelectionCancelCallback);

            // Add the hold selection callback function for each hold in the collection so we can tell between them
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

            // Create the window manager
            container.holdWindows = std::make_unique<HoldWindowManager>(
                GetActiveWindow(),
                container.windows->GetDllInstance(),
                *container.holdManager,
                *container.plugin
            );

            // Create the hold dialog and add to the manager
            std::shared_ptr<HoldConfigurationDialog> dialog = CreateHoldConfigurationDialog(
                holdDependency,
                *container.holdWindows,
                *container.holdSelectionMenu
            );
            container.dialogManager->AddDialog(
                {
                    HOLD_SELECTOR_DIALOG,
                    "Hold Configuration",
                    reinterpret_cast<DLGPROC>(dialog->WndProc),
                    reinterpret_cast<LPARAM>(dialog.get()),
                    dialog
                }
            );

            // Create the event handler and register
            eventHandler = std::make_shared<HoldEventHandler>(
                *container.holdManager,
                *container.plugin,
                *container.holdWindows,
                *container.dialogManager,
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId()
            );

            container.flightplanHandler->RegisterHandler(eventHandler);
            container.timedHandler->RegisterEvent(eventHandler, timedEventFrequency);
            container.commandHandlers->RegisterHandler(eventHandler);
            container.tagHandler->RegisterTagItem(selectedHoldTagItemId, eventHandler);

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
