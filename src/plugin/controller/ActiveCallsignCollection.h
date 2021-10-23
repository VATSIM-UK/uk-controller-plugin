#pragma once
#include "ActiveCallsign.h"
#include "ActiveCallsignEventHandlerInterface.h"

namespace UKControllerPlugin::Controller {
    class ActiveCallsign;

    /*
        Class that maps connected callsigns to UK controller positions and determines
        priority order.
    */
    class ActiveCallsignCollection
    {
        public:
        ActiveCallsignCollection();
        void AddCallsign(const UKControllerPlugin::Controller::ActiveCallsign& controller);
        void AddUserCallsign(const UKControllerPlugin::Controller::ActiveCallsign& controller);
        bool CallsignActive(const std::string& callsign) const;
        void Flush();
        int GetNumberActiveCallsigns() const;
        int GetNumberActivePositions() const;
        UKControllerPlugin::Controller::ActiveCallsign GetCallsign(const std::string& callsign) const;
        auto GetLeadCallsignForPosition(const std::string& normalisedCallsign) const
            -> UKControllerPlugin::Controller::ActiveCallsign;
        auto GetUserCallsign() const -> UKControllerPlugin::Controller::ActiveCallsign;
        bool PositionActive(const std::string& normalisedCallsign) const;
        void RemoveCallsign(const UKControllerPlugin::Controller::ActiveCallsign& controller);
        auto UserHasCallsign() const -> bool;
        void
        AddHandler(const std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsignEventHandlerInterface>& handler);
        auto CountHandlers() const -> size_t;

        private:
        // Whether or not the user is active.
        bool userActive = false;

        // Set of normalised callsign to callsigns actively taking that position. Self ordering.
        std::map<std::string, std::set<UKControllerPlugin::Controller::ActiveCallsign>> activePositions;

        // A map of raw callsign to active position - for easier access when we just want to know who's online
        std::map<std::string, std::set<UKControllerPlugin::Controller::ActiveCallsign>::iterator> activeCallsigns;

        // The callsign for the logged in user, if set.
        std::set<UKControllerPlugin::Controller::ActiveCallsign>::iterator userCallsign;

        // All the handlers for these events
        std::list<std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsignEventHandlerInterface>> handlers;
    };
} // namespace UKControllerPlugin::Controller
