#include "pch/stdafx.h"
#include "handoff/HandoffCollectionFactory.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPlugin {
    namespace Handoff {
        std::unique_ptr<HandoffCollection> Create(
            const ControllerPositionCollection& controllerPositions,
            nlohmann::json handoffs,
            nlohmann::json sidMappings
        ) {
            std::unique_ptr<HandoffCollection> collection = std::make_unique<HandoffCollection>();

            if (!handoffs.is_object()) {
                LogError("Invalid JSON for handoff orders");
                return std::move(collection);
            }

            // Iterate the handoffs and build them up
            for (
                nlohmann::json::const_iterator itHandoff = handoffs.cbegin();
                itHandoff != handoffs.cend();
                ++itHandoff
            ) {
                if (!HandoffOrderValid(itHandoff.value(), controllerPositions)) {
                    LogInfo("Invalid handoff order " + itHandoff.key());
                    continue;
                }

                // Build the hierarchy
                std::shared_ptr<ControllerPositionHierarchy> hierarchy
                    = std::make_shared<ControllerPositionHierarchy>();
                for (
                    nlohmann::json::const_iterator itController = itHandoff.value().cbegin();
                    itController != itHandoff.value().cend();
                    ++itController
                ) {
                    hierarchy->AddPosition(controllerPositions.FetchPositionByCallsign(itController.value()));
                }

                // Add to collection
                collection->AddHandoffOrder(itHandoff.key(), hierarchy);
            }

            if (!sidMappings.is_object()) {
                LogError("Invalid JSON for handoff order SID mappings");
                return std::move(collection);
            }

            // Iterate the sid mappings and build them up
            for (
                nlohmann::json::const_iterator itAirfield = sidMappings.cbegin();
                itAirfield != sidMappings.cend();
                ++itAirfield
            ) {
                if (!itAirfield.value().is_object()) {
                    LogError("Invalid JSON for airfield SID mapping");
                    continue;
                }

                for (
                    nlohmann::json::const_iterator itSids = itAirfield.value().cbegin();
                    itSids != itAirfield.value().cend();
                    ++itSids
                ) {
                    if (!SidMappingValid(itSids.value())) {
                        LogWarning("Invalid SID mapping for " + itSids.key());
                        continue;
                    }

                    collection->AddSidMapping(itAirfield.key(), itSids.key(), itSids.value());
                }

            }

            LogInfo(
                "Added " + std::to_string(collection->CountHandoffs()) + " handoff orders and " +
                std::to_string(collection->CountSidMappings()) + " SID handoff mappings"
            );
            return std::move(collection);
        }

        /*
            Validate a handoff order
        */
        bool HandoffOrderValid(nlohmann::json order, const ControllerPositionCollection & controllers)
        {
            if (!order.is_array()) {
                return false;
            }

            for (nlohmann::json::const_iterator it = order.cbegin(); it != order.cend(); ++it) {
                if (!it.value().is_string() || !controllers.HasPosition(it.value())) {
                    return false;
                }
            }

            return true;
        }

        /*
            Validate an individual SID mapping
        */
        bool SidMappingValid(nlohmann::json mapping)
        {
            return mapping.is_string();
        }
    }  // namespace Handoff
}  // namespace UKControllerPlugin
