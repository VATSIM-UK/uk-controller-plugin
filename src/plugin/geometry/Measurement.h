#pragma once
#include "MeasurementUnit.h"

namespace UKControllerPlugin::Geometry {

    struct MeasurementUnit;

    /**
     * A measurement, comprising a unit and a value
     */
    struct Measurement
    {
        public:
        Measurement(std::unique_ptr<MeasurementUnit> unit, double value);
        ~Measurement();

        // The unit for the measurement
        std::unique_ptr<MeasurementUnit> unit;

        // The value of the measurement
        double value;
    };
} // namespace UKControllerPlugin::Geometry
