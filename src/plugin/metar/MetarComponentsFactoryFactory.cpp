#include "MetarComponentsFactory.h"
#include "MetarComponentsFactoryFactory.h"
#include "MetarComponentFactoryInterface.h"
#include "PressureComponentFactory.h"

namespace UKControllerPlugin::Metar {
    auto BuildComponentsFactory() -> std::shared_ptr<MetarComponentsFactory>
    {
        return std::make_shared<MetarComponentsFactory>(
            std::set<std::shared_ptr<MetarComponentFactoryInterface>>{std::make_shared<PressureComponentFactory>()});
    }
} // namespace UKControllerPlugin::Metar
