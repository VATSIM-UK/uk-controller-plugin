#pragma once
#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/DeemedSeparatedHold.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Stores information about a hold.
        */
        typedef struct HoldingData
        {
            HoldingData(
                unsigned int identifier = 0,
                std::string fix = "INVALID",
                std::string description = "INVALID",
                unsigned int minimum = 0,
                unsigned int maximum = 0,
                unsigned int inbound = 361,
                std::string turnDirection = "up",
                std::set<std::unique_ptr<AbstractHoldLevelRestriction>> restrictions = {},
                std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparatedHolds = {}
            ) : identifier(identifier), fix(fix), description(description), minimum(minimum), maximum(maximum),
                inbound(inbound), turnDirection(turnDirection), restrictions(std::move(restrictions)),
                deemedSeparatedHolds(std::move(deemedSeparatedHolds))
            {};
            HoldingData(HoldingData const&) = delete;
            HoldingData& operator=(HoldingData const&) = delete;
            HoldingData(HoldingData&& original);
            HoldingData &operator=(HoldingData && original) noexcept;
            bool LevelWithinHold(unsigned int level) const;

            // The id for the hold
            unsigned int identifier;

            // The fix the hold is based upon
            std::string fix;

            // The description of the hold
            std::string description;

            // The minimum altitude
            unsigned int minimum;

            // The maximum altitude
            unsigned int maximum;

            // The inbound heading
            unsigned int inbound;

            // The direction of turn
            std::string turnDirection;

            // The restrictions in place regarding levels
            std::set<std::unique_ptr<UKControllerPlugin::Hold::AbstractHoldLevelRestriction>> restrictions;

            // Holds against which this hold is deemed separated
            std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparatedHolds;

            /*
                Compare two holds
            */
            bool operator== (const HoldingData & compare) const
            {
                return this->identifier == compare.identifier;
            }

            static const std::string TURN_DIRECTION_LEFT;
            static const std::string TURN_DIRECTION_RIGHT;
        } HoldingData;
    }  // namespace Hold
}  // namespace UKControllerPlugin
