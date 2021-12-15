#include "PublishedPrenote.h"
#include "controller/ControllerPositionHierarchy.h"

namespace UKControllerPlugin::Prenote {
    PublishedPrenote::PublishedPrenote(
        int id, std::string key, std::unique_ptr<Controller::ControllerPositionHierarchy> controllers)
        : id(id), key(std::move(key)), controllers(std::move(controllers))
    {
    }

    PublishedPrenote::~PublishedPrenote() = default;
    PublishedPrenote::PublishedPrenote(PublishedPrenote&&) noexcept = default;
    auto PublishedPrenote::operator=(PublishedPrenote&&) noexcept -> PublishedPrenote& = default;
} // namespace UKControllerPlugin::Prenote
