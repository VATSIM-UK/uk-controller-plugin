#pragma once
#include "metar/PressureUnit.h"

namespace UKControllerPlugin::Metar {
    /**
     * The pressure component of a METAR, includes
     * QNH, QFE in both hPa and inhg.
     */
    class PressureComponent
    {
        public:
        PressureComponent(
            int qnhHectopascals, int qfeHectopascals, float qnhInHg, float qfeInHg, PressureUnit reportedIn);
        [[nodiscard]] auto QnhHectopascals() const -> int;
        [[nodiscard]] auto QfeHectopascals() const -> int;
        [[nodiscard]] auto QnhInHg() const -> float;
        [[nodiscard]] auto QfeInHg() const -> float;
        [[nodiscard]] auto ReportedIn() const -> PressureUnit;
        [[nodiscard]] auto ReportedAsHectopascals() const -> bool;
        [[nodiscard]] auto ReportedAsInHg() const -> bool;

        private:
        const int qnhHectopascals;
        const int qfeHectopascals;
        const float qnhInHg;
        const float qfeInHg;
        const PressureUnit reportedIn;
    };
} // namespace UKControllerPlugin::Metar
