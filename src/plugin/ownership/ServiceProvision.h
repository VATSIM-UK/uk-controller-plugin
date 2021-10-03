#pragma once
#include "ServiceType.h"

namespace UKControllerPlugin::Controller {
    class ActiveCallsign;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Ownership {
    /**
     * Represents the provision of a service at a given airfield
     */
    using ServiceProvision = struct ServiceProvision
    {
        ServiceProvision(
            const ServiceType serviceProvided, const std::shared_ptr<Controller::ActiveCallsign> controller);

        // The service being provided
        const ServiceType serviceProvided{};

        // Who's providing the service
        const std::shared_ptr<Controller::ActiveCallsign> controller{};
    };
} // namespace UKControllerPlugin::Ownership
