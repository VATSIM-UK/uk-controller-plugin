#include "MutableEventBus.h"
#include "StandardEventBusFactory.h"

namespace UKControllerPlugin::EventHandler {

    auto StandardEventBusFactory::CreateBus() -> std::unique_ptr<EventBus>
    {
        return std::make_unique<MutableEventBus>();
    }
} // namespace UKControllerPlugin::EventHandler
