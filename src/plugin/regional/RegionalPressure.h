#pragma once

namespace UKControllerPlugin::Regional {

    /*
        Represents a regional pressure
    */
    using RegionalPressure = struct RegionalPressure
    {
        // The key for the RPS
        std::string key;

        // The display name of the RPS
        std::string name;

        // The MSL
        unsigned int pressure;

        // When it was last updated
        std::chrono::system_clock::time_point updatedAt = std::chrono::system_clock::now();

        // The time at which it was acknowledged
        std::chrono::system_clock::time_point acknowledgedAt = (std::chrono::system_clock::time_point::min)();

        [[nodiscard]] auto IsAcknowledged() const -> bool;
        void Acknowledge();
        [[nodiscard]] auto operator==(const RegionalPressure& compare) const -> bool;
    };
} // namespace UKControllerPlugin::Regional
