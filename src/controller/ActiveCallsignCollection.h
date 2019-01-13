#pragma once
#include "controller/ActiveCallsign.h"

namespace UKControllerPlugin {
    namespace Controller {

// Forward declaration
class ActiveCallsign;
// END

/*
    Class that maps connected callsigns to UK controller positions and determines
    priority order.
*/
class ActiveCallsignCollection
{
    public:
        ActiveCallsignCollection(void);
        void AddCallsign(UKControllerPlugin::Controller::ActiveCallsign controller);
        void AddUserCallsign(UKControllerPlugin::Controller::ActiveCallsign controller);
        bool CallsignActive(std::string callsign) const;
        void Flush(void);
        int GetNumberActiveCallsigns() const;
        int GetNumberActivePositions() const;
        UKControllerPlugin::Controller::ActiveCallsign GetCallsign(std::string callsign) const;
        UKControllerPlugin::Controller::ActiveCallsign GetLeadCallsignForPosition(
            std::string normalisedCallsign
        ) const;
        UKControllerPlugin::Controller::ActiveCallsign GetUserCallsign(void) const;
        bool PositionActive(std::string normalisedCallsign) const;
        void RemoveCallsign(UKControllerPlugin::Controller::ActiveCallsign controller);
        bool UserHasCallsign(void) const;

    private:

        // Whether or not the user is active.
        bool userActive;

        // Set of normalised callsign to callsigns actively taking that position. Self ordering.
        std::map<std::string, std::set<UKControllerPlugin::Controller::ActiveCallsign>> activePositions;

        // A map of raw callsign to active position - for easier access when we just want to know who's online
        std::map<std::string, std::set<UKControllerPlugin::Controller::ActiveCallsign>::iterator> activeCallsigns;

        // The callsign for the logged in user, if set.
        std::set<UKControllerPlugin::Controller::ActiveCallsign>::iterator userCallsign;
};

}  // namespace Controller
}  // namespace UKControllerPlugin
