#include "MetarComponentFactoryInterface.h"
#include "MetarComponents.h"
#include "MetarComponentsFactory.h"

namespace UKControllerPlugin::Metar {

    MetarComponentsFactory::MetarComponentsFactory(std::set<std::shared_ptr<MetarComponentFactoryInterface>> factories)
        : factories(std::move(factories))
    {
    }

    auto MetarComponentsFactory::FromJson(const nlohmann::json& json) -> std::unique_ptr<MetarComponents>
    {
        auto components = std::make_unique<MetarComponents>();
        if (!json.is_object()) {
            LogError("Parsed metar json is not object");
            return components;
        }

        for (const auto& factory : factories) {
            factory->FromJson(json, *components);
        }

        return components;
    }
} // namespace UKControllerPlugin::Metar
