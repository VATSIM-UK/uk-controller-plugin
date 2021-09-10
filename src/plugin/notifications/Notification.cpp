#include "Notification.h"

namespace UKControllerPlugin::Notifications {
    Notification::Notification(
        int id,
        std::string title,
        std::string body,
        std::chrono::system_clock::time_point validFrom,
        std::chrono::system_clock::time_point validTo,
        std::unique_ptr<Controller::ControllerPositionHierarchy> controllers,
        std::string link)
        : id(id), title(std::move(title)), body(std::move(body)), link(std::move(link)),
          validFrom(std::move(validFrom)), validTo(std::move(validTo)), controllers(std::move(controllers))
    {
    }

    auto Notification::Id() const -> int
    {
        return this->id;
    }

    auto Notification::Title() const -> std::string
    {
        return this->title;
    }

    auto Notification::Body() const -> std::string
    {
        return this->body;
    }

    auto Notification::Link() const -> std::string
    {
        return this->link;
    }

    auto Notification::Active() const -> bool
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        return now >= this->validFrom && now < this->validTo;
    }

    auto Notification::IsRelevant(const Controller::ControllerPosition& position) const -> bool
    {
        return this->controllers->PositionInHierarchy(position);
    }

    auto Notification::IsRead() const -> bool
    {
        return this->read;
    }

    void Notification::Read()
    {
        this->read = true;
    }
} // namespace UKControllerPlugin::Notifications
