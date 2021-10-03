#include "ServiceProvision.h"

namespace UKControllerPlugin::Ownership {
    ServiceProvision::ServiceProvision(
        const ServiceType serviceProvided, const std::shared_ptr<Controller::ActiveCallsign> controller)
        : serviceProvided(serviceProvided), controller(controller)
    {
    }
} // namespace UKControllerPlugin::Ownership
