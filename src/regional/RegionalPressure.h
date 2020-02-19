#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Regional {

        /*
            Represents a regional pressure
        */
        typedef struct RegionalPressure
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

            bool IsAcknowledged(void) const
            {
                return this->acknowledgedAt > this->updatedAt;
            }

            void Acknowledge(void)
            {
                this->acknowledgedAt = std::chrono::system_clock::now();
            }

            bool operator==(const RegionalPressure & compare) const
            {
                return this->name == compare.name &&
                    this->pressure == compare.pressure &&
                    this->acknowledgedAt == compare.acknowledgedAt;
            }
        } RegionalPressure;
    }  // namespace Regional
}  // namespace UKControllerPlugin
