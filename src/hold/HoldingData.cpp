#include "pch/stdafx.h"
#include "hold/HoldingData.h"

namespace UKControllerPlugin {
    namespace Hold {

        const std::string HoldingData::TURN_DIRECTION_LEFT = "left";
        const std::string HoldingData::TURN_DIRECTION_RIGHT = "right";

        HoldingData::HoldingData(HoldingData && original)
            : identifier(original.identifier), fix(original.fix), description(original.description),
            minimum(original.minimum), maximum(original.maximum), turnDirection(original.turnDirection),
            inbound(original.inbound), restrictions(std::move(original.restrictions))
        {
        }

        /*
            Move assignement
        */
        HoldingData & HoldingData::operator=(HoldingData && original)
        {
            if (this != &original)
            {
                this->identifier = original.identifier;
                this->fix = original.fix;
                this->description = original.description;
                this->minimum = original.minimum;
                this->maximum = original.maximum;
                this->inbound = original.inbound;
                this->turnDirection = original.turnDirection;
                this->restrictions = std::move(original.restrictions);
            }

            return *this;
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
