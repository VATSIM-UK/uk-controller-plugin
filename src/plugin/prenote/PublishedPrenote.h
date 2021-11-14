#pragma once

namespace UKControllerPlugin::Controller {
    class ControllerPositionHierarchy;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Prenote {
    /**
     * Represents a published prenote and associated controllers. This may be associated with a particular
     * pairing of airfields, or standard instrument departure and the first controller that is online
     * would receive the prenote.
     */
    using PublishedPrenote = struct PublishedPrenote
    {
        PublishedPrenote(int id, std::string key, std::unique_ptr<Controller::ControllerPositionHierarchy> controllers);
        ~PublishedPrenote();
        PublishedPrenote(const PublishedPrenote&) = delete;
        PublishedPrenote(PublishedPrenote&&) noexcept;
        auto operator=(const PublishedPrenote&) -> PublishedPrenote& = delete;
        auto operator=(PublishedPrenote&&) noexcept -> PublishedPrenote&;

        // The id of the prenote
        int id;

        // A string key to identify the prenote
        std::string key;

        // The controllers that this prenote refers to
        std::unique_ptr<Controller::ControllerPositionHierarchy> controllers;
    };
} // namespace UKControllerPlugin::Prenote
