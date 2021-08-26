#include "ComparePrenoteMessages.h"
#include "PrenoteMessage.h"

namespace UKControllerPlugin::Prenote {
    auto ComparePrenoteMessages::operator()(const std::shared_ptr<PrenoteMessage>& hold, int id) const -> bool
    {
        return hold->GetId() < id;
    }

    auto ComparePrenoteMessages::operator()(int id, const std::shared_ptr<PrenoteMessage>& hold) const -> bool
    {
        return id < hold->GetId();
    }

    auto ComparePrenoteMessages::operator()(
        const std::shared_ptr<PrenoteMessage>& a, const std::shared_ptr<PrenoteMessage>& b) const -> bool
    {
        return a->GetId() < b->GetId();
    }
} // namespace UKControllerPlugin::Prenote
