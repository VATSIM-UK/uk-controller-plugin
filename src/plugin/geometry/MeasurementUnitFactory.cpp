#include "MeasurementUnit.h"
#include "MeasurementUnitFactory.h"

namespace UKControllerPlugin::Geometry {

    auto UnitTypeFromString(const std::string& unitString) -> MeasurementUnitType
    {
        if (unitString == "s") {
            return MeasurementUnitType::Seconds;
        }

        if (unitString == "min") {
            return MeasurementUnitType::Minutes;
        }

        if (unitString == "nm") {
            return MeasurementUnitType::NauticalMiles;
        }

        return MeasurementUnitType::None;
    }

    auto UnitToString(MeasurementUnitType unit) -> std::string
    {
        if (unit == MeasurementUnitType::Seconds) {
            return "Seconds";
        }

        if (unit == MeasurementUnitType::Minutes) {
            return "Minutes";
        }

        if (unit == MeasurementUnitType::NauticalMiles) {
            return "NM";
        }

        return "??";
    }

    auto UnitFromString(const std::string& unitString) -> std::unique_ptr<MeasurementUnit>
    {
        const auto unitType = UnitTypeFromString(unitString);
        return std::make_unique<MeasurementUnit>(unitType, UnitToString(unitType));
    }

    auto UnitStringValid(const std::string& unitString) -> bool
    {
        return UnitTypeFromString(unitString) != MeasurementUnitType::None;
    }
} // namespace UKControllerPlugin::Geometry
