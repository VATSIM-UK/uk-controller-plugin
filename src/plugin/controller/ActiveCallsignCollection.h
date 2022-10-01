#pragma once
#include "ActiveCallsignEventHandlerInterface.h"
#include "CompareActiveCallsigns.h"

namespace UKControllerPlugin::Controller {
    class ActiveCallsign;

    /*
        Class that maps connected callsigns to UK controller positions and determines
        priority order.
    */
    class ActiveCallsignCollection
    {
        public:
        void AddCallsign(const UKControllerPlugin::Controller::ActiveCallsign& controller);
        void AddUserCallsign(const UKControllerPlugin::Controller::ActiveCallsign& controller);
        auto CallsignActive(const std::string& callsign) const -> bool;
        void Flush();
        auto GetNumberActiveCallsigns() const -> int;
        auto GetNumberActivePositions() const -> int;
        auto GetCallsign(const std::string& callsign) const -> UKControllerPlugin::Controller::ActiveCallsign;
        auto GetLeadCallsignForPosition(const std::string& normalisedCallsign) const
            -> const UKControllerPlugin::Controller::ActiveCallsign&;
        auto GetUserCallsign() const -> const UKControllerPlugin::Controller::ActiveCallsign&;
        auto PositionActive(const std::string& normalisedCallsign) const -> bool;
        void RemoveCallsign(const UKControllerPlugin::Controller::ActiveCallsign& controller);
        auto UserHasCallsign() const -> bool;
        void
        AddHandler(const std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsignEventHandlerInterface>& handler);
        auto CountHandlers() const -> size_t;

        private:
        // Whether or not the user is active.
        bool userActive = false;

        // Set of normalised callsign to callsigns actively taking that position. Self ordering.
        std::map<std::string, std::set<std::shared_ptr<ActiveCallsign>, CompareActiveCallsigns>> activePositions;

        // A map of raw callsign to active position - for easier access when we just want to know who's online
        std::map<std::string, std::set<std::shared_ptr<ActiveCallsign>, CompareActiveCallsigns>::iterator>
            activeCallsigns;

        // The callsign for the logged in user, if set.
        std::set<std::shared_ptr<ActiveCallsign>, CompareActiveCallsigns>::iterator userCallsign;

        // All the handlers for these events
        std::list<std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsignEventHandlerInterface>> handlers;
    };
} // namespace UKControllerPlugin::Controller
