#include "HoldingData.h"

namespace UKControllerPlugin::Hold {

    const std::string HoldingData::TURN_DIRECTION_LEFT = "left";
    const std::string HoldingData::TURN_DIRECTION_RIGHT = "right";

    HoldingData::HoldingData(HoldingData&& original)
        : identifier(original.identifier), fix(original.fix), description(original.description),
          minimum(original.minimum), maximum(original.maximum), inbound(original.inbound),
          turnDirection(original.turnDirection), restrictions(std::move(original.restrictions)),
          deemedSeparatedHolds(std::move(original.deemedSeparatedHolds))
    {
    }

    /*
        Move assignment
    */
    HoldingData& HoldingData::operator=(HoldingData&& original) noexcept
    {
        if (this != &original) {
            this->identifier = original.identifier;
            this->fix = original.fix;
            this->description = original.description;
            this->minimum = original.minimum;
            this->maximum = original.maximum;
            this->inbound = original.inbound;
            this->turnDirection = original.turnDirection;
            this->restrictions = std::move(original.restrictions);
            this->deemedSeparatedHolds = std::move(original.deemedSeparatedHolds);
        }

        return *this;
    }

    auto HoldingData::LevelWithinHold(unsigned int level) const -> bool
    {
        return level >= this->minimum && level <= this->maximum;
    }
} // namespace UKControllerPlugin::Hold
