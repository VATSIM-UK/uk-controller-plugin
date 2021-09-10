#pragma once
namespace UKControllerPlugin::Stands {

    /*
        Represents a stand where an aircraft can park
    */
    using Stand = struct Stand
    {

        // The id of the stand in the API
        int id;

        // The airfield that the stand is at
        std::string airfieldCode;

        // The identifier of the stand
        std::string identifier;
    };
} // namespace UKControllerPlugin::Stands
