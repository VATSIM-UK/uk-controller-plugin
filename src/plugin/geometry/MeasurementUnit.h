#pragma once
#include "MeasurementUnitType.h"

namespace UKControllerPlugin::Geometry {
    struct MeasurementUnit
    {
        MeasurementUnit(MeasurementUnitType type, std::string description)
            : type(type), description(std::move(description)){};

        // An enum representing the unit type
        MeasurementUnitType type;

        // A description of the unit
        std::string description;

        [[nodiscard]] auto operator==(const MeasurementUnitType& type) const -> bool;
    };
} // namespace UKControllerPlugin::Geometry
