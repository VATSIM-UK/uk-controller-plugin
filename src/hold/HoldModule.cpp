#include "pch/stdafx.h"
#include "hold/HoldModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "hold/PublishedHoldCollectionFactory.h"
#include "hold/HoldEventHandler.h"
#include "hold/HoldManager.h"
#include "message/UserMessager.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "euroscope/CallbackFunction.h"
#include "api/ApiHelper.h"
#include "windows/WinApiInterface.h"
#include "dependency/DependencyLoaderInterface.h"
#include "tag/TagFunction.h"
#include "hold/HoldSelectionMenu.h"
#include "hold/HoldConfigurationDialog.h"
#include "dialog/DialogData.h"
#include "hold/HoldConfigurationDialogFactory.h"
#include "hold/HoldProfileManagerFactory.h"
#include "hold/HoldRenderer.h"
#include "hold/HoldConfigurationMenuItem.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "hold/HoldDisplayManager.h"
#include "command/CommandHandlerCollection.h"
#include "euroscope/AsrEventHandlerCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Hold::HoldEventHandler;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Hold::HoldSelectionMenu;
using UKControllerPlugin::Hold::HoldConfigurationDialog;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Hold::CreatePublishedHoldCollection;
using UKControllerPlugin::Hold::CreateHoldConfigurationDialog;
using UKControllerPlugin::Hold::CreateHoldProfileManager;
using UKControllerPlugin::Hold::HoldRenderer;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Hold::HoldConfigurationMenuItem;
using UKControllerPlugin::Command::CommandHandlerCollection;

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

        // Dependencies
        const std::string holdDependencyKey = "DEPENDENCY_HOLDS";
        const std::string holdProfileDependencyKey = "DEPENDENCY_HOLD_PROFILE";

        /*
            Bootstrap the module into the plugin
        */
        void BootstrapPlugin(
            DependencyLoaderInterface & dependencyProvider,
            PersistenceContainer & container,
            UserMessager & userMessages
        ) {
            // Update local dependencies and build hold data
            nlohmann::json holdDependency = dependencyProvider.LoadDependency(
                holdDependencyKey,
                nlohmann::json::array()
            );
            container.holdManager = std::make_unique<HoldManager>();
            container.publishedHolds = CreatePublishedHoldCollection(holdDependency, container);

            // Create the object to manage the popup menu
            int holdSelectionCancelId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            container.holdSelectionMenu = std::make_shared<HoldSelectionMenu>(
                *container.holdManager,
                *container.holdProfiles,
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

            // Create the hold dialog and profile manager
            container.holdProfiles = CreateHoldProfileManager(
                dependencyProvider.LoadDependency(holdProfileDependencyKey, nlohmann::json::array()),
                *container.api
            );

            std::shared_ptr<HoldConfigurationDialog> dialog = CreateHoldConfigurationDialog(
                holdDependency,
                *container.holdProfiles
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
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId()
            );

            container.flightplanHandler->RegisterHandler(eventHandler);
            container.timedHandler->RegisterEvent(eventHandler, timedEventFrequency);
            container.tagHandler->RegisterTagItem(selectedHoldTagItemId, eventHandler);


            // Create the hold display factory
            container.holdDisplayFactory.reset(new HoldDisplayFactory(*container.plugin, *container.holdManager));
        }

        /*
            Register the handler to make it configurable from the menu
        */
        void BootstrapRadarScreen(
            ConfigurableDisplayCollection & configurableDisplay,
            RadarRenderableCollection & radarRenderables,
            AsrEventHandlerCollection & asrEvents,
            CommandHandlerCollection & commandHandlers,
            const PersistenceContainer & container
        ) {
            // Display manager
            std::shared_ptr<HoldDisplayManager> displayManager = std::make_shared<HoldDisplayManager>(
                *container.holdProfiles,
                *container.holdManager,
                *container.holdDisplayFactory
            );
            asrEvents.RegisterHandler(displayManager);
            container.holdSelectionMenu->AddDisplayManager(displayManager);

            // Renderer
            const int rendererId = radarRenderables.ReserveRendererIdentifier();
            std::shared_ptr<HoldRenderer> renderer = std::make_shared<HoldRenderer>(
                displayManager,
                radarRenderables.ReserveScreenObjectIdentifier(rendererId),
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId()
            );

            radarRenderables.RegisterRenderer(
                rendererId,
                renderer,
                radarRenderables.afterTags
            );
            configurableDisplay.RegisterDisplay(renderer);
            asrEvents.RegisterHandler(renderer);

            CallbackFunction renderToggleCallback(
                renderer->toggleCallbackFunctionId,
                "Toggle Hold Rendering",
                std::bind(
                    &HoldRenderer::Configure,
                    renderer,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(renderToggleCallback);

            // Profile selector
            std::shared_ptr<HoldConfigurationMenuItem> selector = std::make_shared<HoldConfigurationMenuItem>(
                *container.dialogManager,
                displayManager,
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId()
            );
            configurableDisplay.RegisterDisplay(selector);
            commandHandlers.RegisterHandler(selector);

            CallbackFunction openWindowCallback(
                selector->selectorMenuOpenCallbackId,
                "Open Hold Manager Dialog",
                std::bind(
                &HoldConfigurationMenuItem::Configure,
                    selector,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openWindowCallback);
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
