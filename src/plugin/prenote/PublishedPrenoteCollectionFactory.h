#pragma once

namespace UKControllerPlugin::Controller {
    class ControllerPositionHierarchyFactory;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Prenote {
    class PublishedPrenoteCollection;

    [[nodiscard]] auto CreatePublishedPrenoteCollection(
        const nlohmann::json& prenotes, const Controller::ControllerPositionHierarchyFactory& hierarchyFactory)
        -> std::unique_ptr<PublishedPrenoteCollection>;

    [[nodiscard]] auto
    PrenoteValid(const nlohmann::json& prenote, const Controller::ControllerPositionHierarchyFactory& hierarchyFactory)
        -> bool;
} // namespace UKControllerPlugin::Prenote
