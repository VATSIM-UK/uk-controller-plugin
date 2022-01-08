#pragma once

namespace UKControllerPlugin::Metar {
    struct MetarComponents;

    /**
     * Adds a component of a METAR to the MetarComponents
     */
    class MetarComponentFactoryInterface
    {
        public:
        virtual ~MetarComponentFactoryInterface() = default;
        virtual void FromJson(const nlohmann::json& json, MetarComponents& components) const = 0;
    };
} // namespace UKControllerPlugin::Metar
