#pragma once

namespace UKControllerPlugin::MissedApproach {
    /*
     * Stores details about a missed approach / go around
     */
    class MissedApproach
    {
        public:
        MissedApproach(
            int id, std::string callsign, std::chrono::system_clock::time_point expiresAt, bool createdByUser);
        [[nodiscard]] auto Callsign() const -> const std::string&;
        [[nodiscard]] auto CreatedAt() const -> const std::chrono::system_clock::time_point&;
        [[nodiscard]] auto CreatedByUser() const -> bool;
        [[nodiscard]] auto ExpiresAt() const -> const std::chrono::system_clock::time_point&;
        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto IsExpired() const -> bool;
        void Acknowledge(std::string acknowledgedBy, std::string remarks);
        [[nodiscard]] auto IsAcknowledged() const -> bool;
        [[nodiscard]] auto Remarks() const -> const std::string&;
        [[nodiscard]] auto AcknowledgedBy() const -> const std::string&;

        private:
        // The id of the missed approach
        int id;

        // The callsign associated with the missed approach
        std::string callsign;

        // The time the missed approach expires and another can be created
        std::chrono::system_clock::time_point createdAt;

        // The time the missed approach expires and another can be created
        std::chrono::system_clock::time_point expiresAt;

        // Did the user create this
        bool createdByUser;

        // Remarks from the acknowledger
        std::string acknowledgedBy;

        // Remarks from the acknowledger
        std::string remarks;
    };
} // namespace UKControllerPlugin::MissedApproach
