#pragma once
#include "euroscope/EuroScopeCRadarTargetInterface.h"

namespace UKControllerPlugin::Euroscope {

    class EuroScopeCRadarTargetWrapper : public UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface
    {
        public:
        explicit EuroScopeCRadarTargetWrapper(EuroScopePlugIn::CRadarTarget originalData);
        [[nodiscard]] auto GetCallsign() const -> const std::string override;
        [[nodiscard]] auto GetFlightLevel() const -> int override;
        [[nodiscard]] auto GetAltitude() const -> int override;
        [[nodiscard]] auto GetPosition() const -> const EuroScopePlugIn::CPosition override;
        [[nodiscard]] auto GetGroundSpeed() const -> int override;
        [[nodiscard]] auto GetVerticalSpeed() const -> int override;
        [[nodiscard]] auto GetHeading() const -> double override;
        [[nodiscard]] auto GetEuroScopeObject() const -> EuroScopePlugIn::CRadarTarget& override;

        private:
        // The original object from EuroScope
        mutable EuroScopePlugIn::CRadarTarget originalData;
    };
} // namespace UKControllerPlugin::Euroscope
