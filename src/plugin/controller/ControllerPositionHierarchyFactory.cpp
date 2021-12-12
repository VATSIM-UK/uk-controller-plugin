#include "ControllerPosition.h"
#include "ControllerPositionCollection.h"
#include "ControllerPositionHierarchy.h"
#include "ControllerPositionHierarchyFactory.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPlugin::Controller {

    ControllerPositionHierarchyFactory::ControllerPositionHierarchyFactory(
        const ControllerPositionCollection& controllers)
        : controllers(controllers)
    {
    }

    /*
        Takes an array of callsigns in JSON form and converts these into a hierarchy of controllers.
    */
    auto ControllerPositionHierarchyFactory::CreateFromJsonByCallsign(const nlohmann::json& json) const
        -> std::unique_ptr<ControllerPositionHierarchy>
    {
        if (!json.is_array()) {
            LogError("Controller hierarchy is not an array: " + json.dump());
            throw std::invalid_argument("Json is not array");
        }

        std::unique_ptr<ControllerPositionHierarchy> hierarchy = std::make_unique<ControllerPositionHierarchy>();

        for (nlohmann::json::const_iterator it = json.cbegin(); it != json.cend(); ++it) {
            if (!it->is_string()) {
                throw std::invalid_argument("Controller in hierarchy not a string " + json.dump());
            }

            auto position = this->controllers.FetchPositionByCallsign(*it);
            if (!position) {
                throw std::invalid_argument("Controller position not found " + json.dump());
            }

            hierarchy->AddPosition(position);
        }

        return std::move(hierarchy);
    }

    auto ControllerPositionHierarchyFactory::CreateFromJsonById(const nlohmann::json& json) const
        -> std::unique_ptr<ControllerPositionHierarchy>
    {
        auto hierarchy = std::make_unique<ControllerPositionHierarchy>();
        return this->AddPositionsById(json, hierarchy.get()) ? std::move(hierarchy) : nullptr;
    }

    auto ControllerPositionHierarchyFactory::CreateSharedFromJsonById(const nlohmann::json& json) const
        -> std::shared_ptr<ControllerPositionHierarchy>
    {
        auto hierarchy = std::make_shared<ControllerPositionHierarchy>();
        return this->AddPositionsById(json, hierarchy.get()) ? hierarchy : nullptr;
    }

    auto ControllerPositionHierarchyFactory::AddPositionsById(
        const nlohmann::json& positions, ControllerPositionHierarchy* const hierarchy) const -> bool
    {
        if (!positions.is_array()) {
            LogError("Controller hierarchy is not an array: " + positions.dump());
            return false;
        }

        for (const auto& position : positions) {
            if (!position.is_number_integer()) {
                LogError("Controller position in hierarchy is not an integer");
                return false;
            }

            auto resolvedPosition = this->controllers.FetchPositionById(position.get<int>());
            if (!resolvedPosition) {
                LogError("Controller position in hierarchy does not exist " + positions.dump());
                return false;
            }

            hierarchy->AddPosition(resolvedPosition);
        }

        return true;
    }

    auto ControllerPositionHierarchyFactory::GetPositionsCollection() const -> const ControllerPositionCollection&
    {
        return this->controllers;
    }
} // namespace UKControllerPlugin::Controller
