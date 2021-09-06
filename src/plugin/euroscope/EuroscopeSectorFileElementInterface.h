#pragma once

namespace UKControllerPlugin::Euroscope {
    /*
        An interface for a class that wraps EuroScopes CSectorElement class
    */
    class EuroscopeSectorFileElementInterface
    {
        public:
        [[nodiscard]] virtual auto Airport() const -> std::string = 0;
        [[nodiscard]] virtual auto Name() const -> std::string = 0;
        [[nodiscard]] virtual auto Runway1ActiveForDepartures() -> bool = 0;
        [[nodiscard]] virtual auto Runway1ActiveForArrivals() -> bool = 0;
        [[nodiscard]] virtual auto Runway2ActiveForDepartures() -> bool = 0;
        [[nodiscard]] virtual auto Runway2ActiveForArrivals() -> bool = 0;
        [[nodiscard]] virtual auto Runway1Heading() -> int = 0;
        [[nodiscard]] virtual auto Runway2Heading() -> int = 0;
        [[nodiscard]] virtual auto Runway1Identifier() const -> std::string = 0;
        [[nodiscard]] virtual auto Runway2Identifier() const -> std::string = 0;
    };
} // namespace UKControllerPlugin::Euroscope
