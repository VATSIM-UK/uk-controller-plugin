#pragma once
#include "euroscope/EuroscopeSectorFileElementInterface.h"

namespace UKControllerPlugin::Euroscope {

    /*
        An implementation for a class that wraps EuroScopes CSectorElement class
    */
    class EuroscopeSectorFileElementWrapper final
        : public UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface
    {
        public:
        explicit EuroscopeSectorFileElementWrapper(EuroScopePlugIn::CSectorElement originalData);

        // Inherited via EuroscopeSectorFileElementInterface
        [[nodiscard]] auto Airport() const -> std::string override;
        [[nodiscard]] auto Name() const -> std::string override;
        [[nodiscard]] auto Runway1ActiveForDepartures() -> bool override;
        [[nodiscard]] auto Runway1ActiveForArrivals() -> bool override;
        [[nodiscard]] auto Runway2ActiveForDepartures() -> bool override;
        [[nodiscard]] auto Runway2ActiveForArrivals() -> bool override;
        [[nodiscard]] auto Runway1Heading() -> int override;
        [[nodiscard]] auto Runway2Heading() -> int override;
        [[nodiscard]] auto Runway1Identifier() const -> std::string override;
        [[nodiscard]] auto Runway2Identifier() const -> std::string override;

        private:
        // The original ES class
        EuroScopePlugIn::CSectorElement originalData;
    };
} // namespace UKControllerPlugin::Euroscope
