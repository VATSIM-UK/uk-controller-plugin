#include "PushEventConnectionInterface.h"

namespace UKControllerPlugin::Push {
    PushEventConnectionInterface::~PushEventConnectionInterface() = default;
    PushEventConnectionInterface::PushEventConnectionInterface(const PushEventConnectionInterface&) = default;
    PushEventConnectionInterface::PushEventConnectionInterface(PushEventConnectionInterface&&) noexcept = default;
} // namespace UKControllerPlugin::Push
