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
#include "hold/HoldRenderer.h"
#include "hold/HoldConfigurationMenuItem.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "hold/HoldDisplayManager.h"
#include "command/CommandHandlerCollection.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "hold/HoldDisplayConfigurationDialog.h"
#include "api/ApiException.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Hold::HoldEventHandler;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Hold::HoldSelectionMenu;
using UKControllerPlugin::Hold::HoldConfigurationDialog;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Hold::CreatePublishedHoldCollection;
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
            container.holdManager = std::make_unique<HoldManager>(*container.api, *container.taskRunner);
            container.publishedHolds = CreatePublishedHoldCollection(holdDependency, container);

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
            CallbackFunction holdSelectionCallback(
                holdSelectionCancelId,
                "Hold Selection",
                std::bind(
                    &HoldSelectionMenu::MenuItemClicked,
                    container.holdSelectionMenu,
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(holdSelectionCallback);

            // Create the hold selection dialog
            std::shared_ptr<HoldConfigurationDialog> dialog = std::make_shared<HoldConfigurationDialog>(
                *container.navaids
            );
            container.dialogManager->AddDialog(
                {
                    IDD_HOLD_SELECTION,
                    "Hold Configuration",
                    reinterpret_cast<DLGPROC>(dialog->WndProc),
                    reinterpret_cast<LPARAM>(dialog.get()),
                    dialog
                }
            );

            // Create the hold display configuration dialog
            std::shared_ptr<HoldDisplayConfigurationDialog> displayDialog =
                std::make_shared<HoldDisplayConfigurationDialog>();
            container.dialogManager->AddDialog(
                {
                    IDD_HOLD_PARAMS,
                    "Hold Parameters",
                    reinterpret_cast<DLGPROC>(displayDialog->WndProc),
                    reinterpret_cast<LPARAM>(displayDialog.get()),
                    displayDialog
                }
            );

            // Create the event handler and register
            eventHandler = std::make_shared<HoldEventHandler>(
                *container.holdManager,
                *container.navaids,
                *container.plugin,
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId()
            );

            container.tagHandler->RegisterTagItem(selectedHoldTagItemId, eventHandler);
            container.timedHandler->RegisterEvent(eventHandler, 7);
            container.websocketProcessors->AddProcessor(eventHandler);

            // Create the hold display factory
            container.holdDisplayFactory.reset(
                new HoldDisplayFactory(
                    *container.plugin,
                    *container.holdManager,
                    *container.navaids,
                    *container.publishedHolds,
                    *container.dialogManager
                )
            );

            // Start a task to load all the already assigned holds
            container.taskRunner->QueueAsynchronousTask([&container]() {
                nlohmann::json assignedHolds;
                try {
                    assignedHolds = container.api->GetAssignedHolds();
                } catch (ApiException exception) {
                    LogError("Api exception thrown when loading assigned holds");
                    return;
                }

                if (!assignedHolds.is_array()) {
                    LogError("Assigned holds is not array");
                    return;
                }

                for (nlohmann::json::const_iterator it = assignedHolds.cbegin(); it != assignedHolds.cend(); ++it) {
                    if (
                        !it->is_object() ||
                        !it->contains("callsign") ||
                        !it->at("callsign").is_string() ||
                        !it->contains("navaid") ||
                        !it->at("navaid").is_string()
                    ) {
                        LogError("Invalid assigned hold: " + it->dump());
                        return;
                    }

                    container.holdManager->AssignAircraftToHold(
                        it->at("callsign").get<std::string>(),
                        it->at("navaid").get<std::string>(),
                        false
                    );
                }

                LogInfo(
                    "Loaded " + std::to_string(container.holdManager->CountHoldingAircraft()) +
                        " aircraft into assigned holds"
                );

            });
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
