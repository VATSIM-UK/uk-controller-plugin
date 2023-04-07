#include "Measurement.h"
#include "MeasurementUnit.h"

namespace UKControllerPlugin::Geometry {

    Measurement::Measurement(std::unique_ptr<MeasurementUnit> unit, double value) : unit(std::move(unit)), value(value)
    {
        assert(this->unit && "Invalid unit in measurement");
    }

    Measurement::~Measurement() = default;
} // namespace UKControllerPlugin::Geometry
