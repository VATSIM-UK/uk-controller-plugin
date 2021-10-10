#pragma once
namespace UKControllerPlugin::MissedApproach {
    /*
     * Stores details about a missed approach / go around
     */
    class MissedApproach
    {
        public:
        MissedApproach(int id, std::string callsign, std::chrono::system_clock::time_point expiresAt);
        [[nodiscard]] auto Callsign() const -> const std::string&;
        [[nodiscard]] auto ExpiresAt() const -> const std::chrono::system_clock::time_point&;
        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto IsExpired() const -> bool;

        private:
        // The id of the missed approach
        int id;

        // The callsign associated with the missed approach
        std::string callsign;

        // The time the missed approach expires and another can be created
        std::chrono::system_clock::time_point expiresAt;
    };
} // namespace UKControllerPlugin::MissedApproach
