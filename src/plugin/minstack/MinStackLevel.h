#pragma once
#include "pch/pch.h"

namespace UKControllerPlugin {
    namespace MinStack {

        /*
            Represents a minimum stack level
        */
        typedef struct MinStackLevel
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

            bool IsAcknowledged(void) const
            {
                return this->acknowledgedAt > this->updatedAt;
            }

            void Acknowledge(void)
            {
                this->acknowledgedAt = std::chrono::system_clock::now();
            }

            bool operator==(const MinStackLevel & compare) const
            {
                return this->type == compare.type &&
                    this->name == compare.name &&
                    this->msl == compare.msl &&
                    this->acknowledgedAt == compare.acknowledgedAt;
            }
        } MinStackLevel;
    }  // namespace MinStack
}  // namespace UKControllerPlugin
