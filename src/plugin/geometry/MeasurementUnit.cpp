#include "MeasurementUnit.h"

namespace UKControllerPlugin::Geometry {
    auto MeasurementUnit::operator==(const MeasurementUnitType& type) const -> bool
    {
        return this->type == type;
    }
} // namespace UKControllerPlugin::Geometry
