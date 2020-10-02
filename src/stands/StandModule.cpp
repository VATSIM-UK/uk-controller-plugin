#pragma once
#include "pch/stdafx.h"
#include "stands/StandModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"
#include "stands/StandEventHandler.h"
#include "stands/Stand.h"
#include "stands/CompareStands.h"
#include "stands/StandSerializer.h"
#include "tag/TagFunction.h"
#include "euroscope/CallbackFunction.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Euroscope::CallbackFunction;

namespace UKControllerPlugin {
    namespace Stands {

        // The tag item id for assigned stand
        const int assignedStandTagItemId = 110;
        const int openStandAssignmentPopupTagFunctionId = 9007;
        const int openStandAssignmentEditBoxTagFunctionId = 9008;

        const std::string standDependency = "DEPENDENCY_STANDS";

        void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependencies)
        {
            // Load stand data from the dependency
            std::set<Stand, CompareStands> stands;
            from_json(
                dependencies.LoadDependency(standDependency, nlohmann::json::object()),
                stands
            );

            // Create the event handler
            std::shared_ptr<StandEventHandler> eventHandler = std::make_shared<StandEventHandler>(
                *container.api,
                *container.taskRunner,
                *container.plugin,
                stands,
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId()
            );

            // Create a tag function for the stand assignment popup list and add a callback
            TagFunction openStandAssignmentPopupMenu(
                openStandAssignmentPopupTagFunctionId,
                "Open Stand Assignment Popup",
                std::bind(
                    &StandEventHandler::DisplayStandSelectionMenu,
                    eventHandler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openStandAssignmentPopupMenu);

            CallbackFunction standSelectedCallback(
                eventHandler->standSelectedCallbackId,
                "Stand Selected",
                std::bind(
                    &StandEventHandler::StandSelected,
                    eventHandler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(standSelectedCallback);

            // TAG function to trigger the stand assignment edit box, uses existing callback
            TagFunction openStandAssignmentEditBox(
                openStandAssignmentEditBoxTagFunctionId,
                "Edit Enroute Release Point",
                std::bind(
                    &StandEventHandler::DisplayStandAssignmentEditBox,
                    eventHandler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openStandAssignmentEditBox);

            // Assign to handlers
            container.tagHandler->RegisterTagItem(assignedStandTagItemId, eventHandler);
            container.websocketProcessors->AddProcessor(eventHandler);
        }
    }  // namespace Stands
}  // namespace UKControllerPlugin
