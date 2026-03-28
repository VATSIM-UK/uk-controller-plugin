#include "CompareStands.h"
#include "StandEventHandler.h"
#include "StandModule.h"
#include "StandSerializer.h"
#include "StandColourConfiguration.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"
#include "euroscope/CallbackFunction.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "integration/ExternalMessageEventHandler.h"
#include "integration/InboundIntegrationMessageHandler.h"
#include "integration/IntegrationPersistenceContainer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "push/PushEventProcessorCollection.h"
#include "tag/TagFunction.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin::Stands {

    // The tag item id for assigned stand
    // Tag item IDs are defined in StandEventHandler
    const int openStandAssignmentPopupTagFunctionId = 9007;
    const int openStandAssignmentEditBoxTagFunctionId = 9008;

    void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependencies)
    {
        // Load stand data from the dependency
        std::set<Stand, CompareStands> stands;
        from_json(dependencies.LoadDependency(GetDependencyKey(), nlohmann::json::object()), stands);

        // Load stand colour configuration from EuroScope user settings
        // If pluginUserSettingHandler is not available (e.g., in tests), creates default-only config
        auto colourConfiguration = container.pluginUserSettingHandler
                                       ? std::make_shared<StandColourConfiguration>(*container.pluginUserSettingHandler)
                                       : std::make_shared<StandColourConfiguration>();

        // Create the event handler
        auto standSelectedCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<StandEventHandler> eventHandler = std::make_shared<StandEventHandler>(
            *container.api,
            *container.taskRunner,
            *container.plugin,
            *container.integrationModuleContainer->outboundMessageHandler,
            container.airfieldOwnership,
            stands,
            StandEventHandler::ConstructorOptions{standSelectedCallbackId, colourConfiguration});

        // Create a tag function for the stand assignment popup list and add a callback
        TagFunction openStandAssignmentPopupMenu(
            openStandAssignmentPopupTagFunctionId,
            "Open Stand Assignment Popup",
            [eventHandler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                std::string context,
                const POINT& mousePos) {
                eventHandler->DisplayStandSelectionMenu(fp, rt, std::move(context), mousePos);
            });

        container.pluginFunctionHandlers->RegisterFunctionCall(openStandAssignmentPopupMenu);

        CallbackFunction standSelectedCallback(
            standSelectedCallbackId,
            "Stand Selected",
            [eventHandler](int functionId, std::string subject, RECT screenObjectArea) {
                eventHandler->StandSelected(functionId, std::move(subject), screenObjectArea);
            });

        container.pluginFunctionHandlers->RegisterFunctionCall(standSelectedCallback);

        // TAG function to trigger the stand assignment edit box, uses existing callback
        TagFunction openStandAssignmentEditBox(
            openStandAssignmentEditBoxTagFunctionId,
            "Open Stand Assignment Edit Box",
            [eventHandler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                std::string context,
                const POINT& mousePos) {
                eventHandler->DisplayStandAssignmentEditBox(fp, rt, std::move(context), mousePos);
            });

        container.pluginFunctionHandlers->RegisterFunctionCall(openStandAssignmentEditBox);

        // Assign to handlers
        container.flightplanHandler->RegisterHandler(eventHandler);
        container.tagHandler->RegisterTagItem(StandEventHandler::assignedStandTagItemId, eventHandler);
        container.tagHandler->RegisterTagItem(StandEventHandler::standAssignmentSourceTagItemId, eventHandler);
        container.pushEventProcessors->AddProcessor(eventHandler);
        container.externalEventHandler->AddHandler(eventHandler);
        container.integrationModuleContainer->inboundMessageHandler->AddProcessor(eventHandler);
    }

    auto GetDependencyKey() -> std::string
    {
        return "DEPENDENCY_STANDS";
    }
} // namespace UKControllerPlugin::Stands
