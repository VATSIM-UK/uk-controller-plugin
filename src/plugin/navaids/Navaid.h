#pragma once

namespace UKControllerPlugin::Navaids {

    /*
        Struct containing data about a Navaid
    */
    using Navaid = struct Navaid
    {

        // The database ID of the navaid
        int id;

        // The identifier for the navaid
        std::string identifier;

        // The navaid coordinates
        EuroScopePlugIn::CPosition coordinates;

        auto operator==(const Navaid& compare) const -> bool;
        auto operator!=(const Navaid& compare) const -> bool;
    };
} // namespace UKControllerPlugin::Navaids
