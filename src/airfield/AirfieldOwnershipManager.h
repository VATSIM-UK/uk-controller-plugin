#pragma once
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsignCollection;
    }  // namespace Controller
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Airfield {

        // More forward declarations
        class AirfieldCollection;
        class Airfield;
        // END

        /*
            A class for managing who "owns" a particular airfield. This can then be used
            to determine whether or not the client should be setting initial altitudes or
            requesting squawks. Naturally, there are server side checks for this too,
            but nobody likes a hammered server.
        */
        class AirfieldOwnershipManager
        {
            public:
                AirfieldOwnershipManager(
                    const UKControllerPlugin::Airfield::AirfieldCollection & airfields,
                    const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns
                );
                bool AirfieldOwnedBy(
                    std::string icao,
                    const UKControllerPlugin::Controller::ActiveCallsign & position
                ) const;
                bool AirfieldOwnedByUser(std::string icao) const;
                bool AirfieldHasOwner(std::string icao) const;
                void Flush(void);
                const UKControllerPlugin::Controller::ActiveCallsign & GetOwner(std::string icao) const;
                std::vector<UKControllerPlugin::Airfield::Airfield> GetOwnedAirfields(std::string callsign) const;
                void RefreshOwner(std::string icao);

                // A callsign to return when a lookup is done but the callsign cant be found
                const UKControllerPlugin::Controller::ActiveCallsign notFoundCallsign;

            private:

                // A controller position to return when a lookup is done but the callsign cant be found
                const UKControllerPlugin::Controller::ControllerPosition notFoundControllerPosition;

                // All the active users.
                const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns;

                // Collection of all airfields
                const UKControllerPlugin::Airfield::AirfieldCollection & airfields;

                // Map of callsign to ownership
                std::map<std::string, std::unique_ptr<UKControllerPlugin::Controller::ActiveCallsign>> ownershipMap;
        };
    }  // namespace Airfield
}  // namespace UKControllerPlugin
