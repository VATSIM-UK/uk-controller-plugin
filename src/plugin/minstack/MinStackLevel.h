#pragma once

namespace UKControllerPlugin::MinStack {

    /*
        Represents a minimum stack level
    */
    using MinStackLevel = struct MinStackLevel
    {
        // The type of MSL - airfield or tma
        std::string type;

        // The display name of the MSL
        std::string name;

        // The MSL
        unsigned int msl;

        // When it was last updated
        std::chrono::system_clock::time_point updatedAt = std::chrono::system_clock::now();

        // The time at which it was acknowledged
        std::chrono::system_clock::time_point acknowledgedAt = (std::chrono::system_clock::time_point::min)();

        [[nodiscard]] auto IsAcknowledged() const -> bool;
        void Acknowledge();
        [[nodiscard]] auto operator==(const MinStackLevel& compare) const -> bool;
    };
} // namespace UKControllerPlugin::MinStack
