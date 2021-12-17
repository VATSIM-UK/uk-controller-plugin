#pragma once

namespace UKControllerPlugin::Metar {
    struct MetarComponents;
    class MetarComponentFactoryInterface;
    
    /**
     * Builds the components of a METAR from JSON
     */
    class MetarComponentsFactory
    {
        public:
            MetarComponentsFactory(std::set<std::shared_ptr<MetarComponentFactoryInterface>> factories);
            [[nodiscard]] auto FromJson(const nlohmann::json& json) -> std::unique_ptr<MetarComponents>;
            
            private:
            
            // The factories
            std::set<std::shared_ptr<MetarComponentFactoryInterface>> factories;
    };
} // namespace UKControllerPlugin::Metar
