#pragma once
#include "MeasurementUnitType.h"

namespace UKControllerPlugin::Geometry {
    struct MeasurementUnit;

    [[nodiscard]] auto UnitFromString(const std::string& unitString) -> std::unique_ptr<MeasurementUnit>;
    [[nodiscard]] auto UnitStringValid(const std::string& unitString) -> bool;
} // namespace UKControllerPlugin::Geometry
