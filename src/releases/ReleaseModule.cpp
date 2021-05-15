#include "pch/stdafx.h"
#include "releases/ReleaseModule.h"
#include "releases/EnrouteReleaseEventHandler.h"
#include "releases/EnrouteReleaseType.h"
#include "releases/CompareEnrouteReleaseTypes.h"
#include "releases/EnrouteReleaseTypesSerializer.h"
#include "tag/TagFunction.h"
#include "euroscope/CallbackFunction.h"
#include "releases/DepartureReleaseEventHandler.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Releases::DepartureReleaseEventHandler;

namespace UKControllerPlugin {
    namespace Releases {

        const std::string enrouteReleaseTypesDependency = "DEPENDENCY_ENROUTE_RELEASE_TYPES";
        const unsigned int enrouteReleaseTypeTagItemId = 9005;
        const unsigned int enrouteReleasePointTagItemId = 9006;

        void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependencies)
        {
            // Load everything to do with ENROUTE releases
            std::set<EnrouteReleaseType, CompareEnrouteReleaseTypes> releaseTypes;
            from_json(
                dependencies.LoadDependency(enrouteReleaseTypesDependency, nlohmann::json::array()),
                releaseTypes
            );

            // Create the handler
            std::shared_ptr<EnrouteReleaseEventHandler> handler = std::make_shared<EnrouteReleaseEventHandler>(
                *container.api,
                *container.plugin,
                *container.taskRunner,
                releaseTypes,
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId(),
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId()
            );

            // TAG function to trigger the release type menu and receive the changes
            TagFunction openReleaseTypePopupMenu(
                enrouteReleaseTypeTagItemId,
                "Open Enroute Release Type Menu",
                std::bind(
                    &EnrouteReleaseEventHandler::DisplayReleaseTypeMenu,
                    handler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openReleaseTypePopupMenu);

            CallbackFunction releaseTypeSelectedCallback(
                handler->releaseTypeSelectedCallbackId,
                "Release Type Selected",
                std::bind(
                    &EnrouteReleaseEventHandler::ReleaseTypeSelected,
                    handler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(releaseTypeSelectedCallback);

            // TAG function to trigger the release edit popup and receive the changes
            TagFunction openReleasePointEditBox(
                enrouteReleasePointTagItemId,
                "Edit Enroute Release Point",
                std::bind(
                &EnrouteReleaseEventHandler::DisplayReleasePointEditBox,
                    handler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openReleasePointEditBox);

            CallbackFunction editReleaseTypeCallback(
                handler->editReleasePointCallbackId,
                "Release Point Edited",
                std::bind(
                &EnrouteReleaseEventHandler::EditReleasePoint,
                    handler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(editReleaseTypeCallback);

            // Add to events
            container.websocketProcessors->AddProcessor(handler);
            container.tagHandler->RegisterTagItem(handler->enrouteReleaseTypeTagItemId, handler);
            container.tagHandler->RegisterTagItem(handler->enrouteReleasePointTagItemId, handler);
            container.tagHandler->RegisterTagItem(handler->enrouteReleasePointOrBlankTagItemId, handler);
            container.timedHandler->RegisterEvent(handler, 10);
            container.controllerHandoffHandlers->RegisterHandler(handler);


            // Everything to do with DEPARTURE releases
            std::shared_ptr<DepartureReleaseEventHandler> departureHandler =
                std::make_shared<DepartureReleaseEventHandler>(
                    *container.controllerPositions
                );

            // Add to handlers
            container.timedHandler->RegisterEvent(departureHandler, 15);
        }
    }  // namespace Releases
}  // namespace UKControllerPlugin
