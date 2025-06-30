#include "AbstractHoldLevelRestriction.h"
#include "AircraftEnteredHoldingAreaEventHandler.h"
#include "AircraftExitedHoldingAreaEventHandler.h"
#include "AssignHoldCommand.h"
#include "DeemedSeparatedHold.h"
#include "HoldConfigurationDialog.h"
#include "HoldConfigurationMenuItem.h"
#include "HoldDisplayConfigurationDialog.h"
#include "HoldDisplayFactory.h"
#include "HoldDisplayManager.h"
#include "HoldEventHandler.h"
#include "HoldManager.h"
#include "HoldModule.h"
#include "HoldRenderer.h"
#include "HoldSelectionMenu.h"
#include "PublishedHoldCollection.h"
#include "PublishedHoldCollectionFactory.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "bootstrap/PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "dependency/DependencyLoaderInterface.h"
#include "dialog/DialogData.h"
#include "dialog/DialogManager.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/CallbackFunction.h"
#include "message/UserMessager.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "push/PushEventProcessorCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "task/TaskRunnerInterface.h"
#include "tag/TagItemCollection.h"
#include "tag/TagFunction.h"
#include "timedevent/TimedEventCollection.h"
#include "windows/WinApiInterface.h"
#include "graphics/GdiplusBrushes.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Hold::CreatePublishedHoldCollection;
using UKControllerPlugin::Hold::HoldConfigurationDialog;
using UKControllerPlugin::Hold::HoldConfigurationMenuItem;
using UKControllerPlugin::Hold::HoldEventHandler;
using UKControllerPlugin::Hold::HoldRenderer;
using UKControllerPlugin::Hold::HoldSelectionMenu;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPlugin::Hold {

    // The id of the tag item for selected hold
    const int selectedHoldTagItemId = 106;

    // The id of the popup menu tag function
    const unsigned int popupMenuTagItemId = 9003;

    const int eventHandlerTimedEventFrequency = 7;

    /*
        Bootstrap the module into the plugin
    */
    void BootstrapPlugin(DependencyLoaderInterface& dependencyProvider, PersistenceContainer& container)
    {
        // Update local dependencies and build hold data
        nlohmann::json holdDependency = dependencyProvider.LoadDependency(GetDependencyKey(), nlohmann::json::array());
        container.holdManager = std::make_unique<HoldManager>(*container.api, *container.taskRunner);
        container.publishedHolds = CreatePublishedHoldCollection(holdDependency, container);

        // Create the object to manage the popup menu
        int holdSelectionCancelId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        container.holdSelectionMenu =
            std::make_shared<HoldSelectionMenu>(*container.holdManager, *container.plugin, holdSelectionCancelId);
        auto holdSelectionMenu = container.holdSelectionMenu;

        // TAG function to trigger the popup menu
        TagFunction openHoldPopupMenu(
            popupMenuTagItemId,
            "Open Hold Selection Menu",
            [holdSelectionMenu](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { holdSelectionMenu->DisplayMenu(fp, rt, context, mousePos); });
        container.pluginFunctionHandlers->RegisterFunctionCall(openHoldPopupMenu);

        // The selection cancel function takes the base id
        CallbackFunction holdSelectionCallback(
            holdSelectionCancelId,
            "Hold Selection",
            [holdSelectionMenu](int functionId, const std::string& subject, RECT screenObjectArea) {
                holdSelectionMenu->MenuItemClicked(functionId, subject);
            });
        container.pluginFunctionHandlers->RegisterFunctionCall(holdSelectionCallback);

        // Create the hold selection dialog
        std::shared_ptr<HoldConfigurationDialog> dialog = std::make_shared<HoldConfigurationDialog>(*container.navaids);
        container.dialogManager->AddDialog(
            {IDD_HOLD_SELECTION,
             "Hold Configuration",
             reinterpret_cast<DLGPROC>(dialog->WndProc), // NOLINT
             reinterpret_cast<LPARAM>(dialog.get()),     // NOLINT
             dialog});

        // Create the hold display configuration dialog
        std::shared_ptr<HoldDisplayConfigurationDialog> displayDialog =
            std::make_shared<HoldDisplayConfigurationDialog>();
        container.dialogManager->AddDialog(
            {IDD_HOLD_PARAMS,
             "Hold Parameters",
             reinterpret_cast<DLGPROC>(displayDialog->WndProc), // NOLINT
             reinterpret_cast<LPARAM>(displayDialog.get()),     // NOLINT
             displayDialog});

        // Create the event handler and register
        auto eventHandler =
            std::make_shared<HoldEventHandler>(*container.holdManager, *container.navaids, *container.plugin);

        container.tagHandler->RegisterTagItem(selectedHoldTagItemId, eventHandler);
        container.timedHandler->RegisterEvent(eventHandler, eventHandlerTimedEventFrequency);
        container.pushEventProcessors->AddProcessor(eventHandler);

        // Create the hold display factory
        container.holdDisplayFactory = std::make_unique<HoldDisplayFactory>(
            *container.plugin,
            *container.holdManager,
            *container.navaids,
            *container.publishedHolds,
            *container.dialogManager,
            *container.callsignSelectionListFactory,
            *container.brushes);

        // Command to assign holds
        container.commandHandlers->RegisterHandler(
            std::make_shared<AssignHoldCommand>(*container.holdManager, *container.plugin));

        // Start a task to load all the already assigned holds
        container.taskRunner->QueueAsynchronousTask([&container]() {
            nlohmann::json assignedHolds;
            try {
                assignedHolds = container.api->GetAssignedHolds();
            } catch (ApiException& exception) {
                LogError("Api exception thrown when loading assigned holds");
                return;
            }

            if (!assignedHolds.is_array()) {
                LogError("Assigned holds is not array");
                return;
            }

            for (nlohmann::json::const_iterator it = assignedHolds.cbegin(); it != assignedHolds.cend(); ++it) {
                if (!it->is_object() || !it->contains("callsign") || !it->at("callsign").is_string() ||
                    !it->contains("navaid") || !it->at("navaid").is_string()) {
                    LogError("Invalid assigned hold: " + it->dump());
                    return;
                }

                container.holdManager->AssignAircraftToHold(
                    it->at("callsign").get<std::string>(), it->at("navaid").get<std::string>(), false);
            }

            LogInfo(
                "Loaded " + std::to_string(container.holdManager->CountHoldingAircraft()) +
                " aircraft into assigned holds");
        });

        // Hold proximity detection handlers
        container.pushEventProcessors->AddProcessor(
            std::make_shared<AircraftEnteredHoldingAreaEventHandler>(*container.holdManager, *container.navaids));
        container.pushEventProcessors->AddProcessor(
            std::make_shared<AircraftExitedHoldingAreaEventHandler>(*container.holdManager, *container.navaids));
    }

    /*
        Register the handler to make it configurable from the menu
    */
    void BootstrapRadarScreen(
        ConfigurableDisplayCollection& configurableDisplay,
        RadarRenderableCollection& radarRenderables,
        AsrEventHandlerCollection& asrEvents,
        CommandHandlerCollection& commandHandlers,
        const GdiplusBrushes& brushes,
        const PersistenceContainer& container)
    {
        // Display manager
        std::shared_ptr<HoldDisplayManager> displayManager =
            std::make_shared<HoldDisplayManager>(*container.holdDisplayFactory);
        asrEvents.RegisterHandler(displayManager);
        container.holdSelectionMenu->AddDisplayManager(displayManager);

        // Renderer
        const int rendererId = radarRenderables.ReserveRendererIdentifier();
        std::shared_ptr<HoldRenderer> renderer = std::make_shared<HoldRenderer>(
            displayManager,
            radarRenderables.ReserveScreenObjectIdentifier(rendererId),
            container.pluginFunctionHandlers->ReserveNextDynamicFunctionId());

        radarRenderables.RegisterRenderer(rendererId, renderer, RadarRenderableCollection::afterTags);
        configurableDisplay.RegisterDisplay(renderer);
        asrEvents.RegisterHandler(renderer);

        CallbackFunction renderToggleCallback(
            renderer->GetToggleCallbackId(),
            "Toggle Hold Rendering",
            [renderer](int functionId, std::string subject, RECT screenObjectArea) {
                renderer->Configure(functionId, std::move(subject), screenObjectArea);
            });
        container.pluginFunctionHandlers->RegisterFunctionCall(renderToggleCallback);

        // Profile selector
        std::shared_ptr<HoldConfigurationMenuItem> selector = std::make_shared<HoldConfigurationMenuItem>(
            *container.dialogManager, displayManager, container.pluginFunctionHandlers->ReserveNextDynamicFunctionId());
        configurableDisplay.RegisterDisplay(selector);
        commandHandlers.RegisterHandler(selector);

        CallbackFunction openWindowCallback(
            selector->selectorMenuOpenCallbackId, // NOLINT
            "Open Hold Manager Dialog",
            [selector](int functionId, std::string subject, RECT screenObjectArea) {
                selector->Configure(functionId, std::move(subject), screenObjectArea);
            });
        container.pluginFunctionHandlers->RegisterFunctionCall(openWindowCallback);
    }

    auto GetDependencyKey() -> std::string
    {
        return "DEPENDENCY_HOLDS";
    }
} // namespace UKControllerPlugin::Hold
