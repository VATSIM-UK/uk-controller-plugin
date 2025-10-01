#pragma once
#include "MetarComponentFactoryInterface.h"
#include <nlohmann/json.hpp>

namespace UKControllerPlugin::Metar {
    class PressureComponentFactory : public MetarComponentFactoryInterface
    {
        public:
        void FromJson(const nlohmann::json& json, MetarComponents& components) const override;

        private:
        [[nodiscard]] auto JsonValid(const nlohmann::json& json) const -> bool;
        [[nodiscard]] static auto HectopascalPressuresValid(const nlohmann::json& json) -> bool;
        [[nodiscard]] static auto InHgPressuresValid(const nlohmann::json& json) -> bool;
        [[nodiscard]] auto PressureFormatValid(const nlohmann::json& json) const -> bool;
        [[nodiscard]] static auto PressureFormatValidUnit(const std::string& pressureFormat) -> bool;
    };
} // namespace UKControllerPlugin::Metar
