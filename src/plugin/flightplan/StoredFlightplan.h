#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Flightplan {

    /*`
        A class that represents, in the broadest sense, a flightplan.
        Does not contain most of the information that EuroScope can provide us,
        just enough to perform functions such as squawk assignment - and also to provide
        a copy for retention if an aircraft logs off. Unlike EuroScopes flightplans, this can persist
        outside the scope of a single function call.
    */
    class StoredFlightplan
    {
        public:
        explicit StoredFlightplan(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& euroscopePlan);
        StoredFlightplan(std::string callsign, std::string origin, std::string destination);
        ~StoredFlightplan();
        StoredFlightplan(const StoredFlightplan&);
        StoredFlightplan(StoredFlightplan&&) noexcept;
        auto operator=(StoredFlightplan&&) noexcept -> StoredFlightplan&;
        [[nodiscard]] auto GetActualOffBlockTime() const -> std::chrono::system_clock::time_point;
        [[nodiscard]] auto GetCallsign() const -> std::string;
        [[nodiscard]] auto GetDestination() const -> std::string;
        [[nodiscard]] auto GetEstimatedDepartureTime() const -> std::chrono::system_clock::time_point;
        [[nodiscard]] auto GetExpectedOffBlockTime() const -> std::chrono::system_clock::time_point;
        [[nodiscard]] auto GetOrigin() const -> std::string;
        [[nodiscard]] auto GetPreviouslyAssignedSquawk() const -> std::string;
        [[nodiscard]] auto GetTimeout() const -> std::time_t;
        [[nodiscard]] auto HasPreviouslyAssignedSquawk() const -> bool;
        [[nodiscard]] auto HasTimedOut() const -> bool;
        void SetActualOffBlockTime(std::chrono::system_clock::time_point time);
        void SetCallsign(std::string callsign);
        void SetDestination(std::string destination);
        void SetEstimatedDepartureTime(std::chrono::system_clock::time_point time);
        void SetOrigin(std::string origin);
        void SetPreviouslyAssignedSquawk(std::string squawk);
        void SetTimeout(int offset);
        void ResetTimeout();
        auto operator==(const StoredFlightplan& compare) const -> bool;
        auto operator==(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& compare) const -> bool;
        auto operator!=(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& compare) const -> bool;
        auto operator!=(const StoredFlightplan& compare) const -> bool;
        auto operator=(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& euroscopePlan)
            -> StoredFlightplan&;
        auto operator=(const StoredFlightplan& flightplan) -> StoredFlightplan&;

        private:
        // Default time, when no timeout is set.
        inline static const std::time_t defaultTime = 0;

        // The squawk code assigned if none allocated by the plugin.
        inline static const std::string noSquawkAllocated = "2200"; // NOLINT

        // Minutes to subtract to calculate EDT
        static const int EDT_MINUTES = 15;

        // The callsign for the aircraft
        std::string callsign;

        // The origin airport
        std::string origin;

        // The destination airport
        std::string destination;

        // The time the flightplan is deemed to "time out"
        std::time_t timeout;

        // The squawk code that has been previously assigned to this flightplan
        std::string assignedSquawk;

        // The expected off block time
        std::chrono::system_clock::time_point expectedOffBlockTime = (std::chrono::system_clock::time_point::max)();

        // The estimated departure time
        std::chrono::system_clock::time_point estimatedDepartureTime = (std::chrono::system_clock::time_point::max)();

        // The time at which the aircraft was reported to have left the blocks
        std::chrono::system_clock::time_point actualOffBlockTime = (std::chrono::system_clock::time_point::max)();
    };
} // namespace UKControllerPlugin::Flightplan
