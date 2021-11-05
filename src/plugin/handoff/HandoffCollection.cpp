#include "HandoffCollection.h"
#include "HandoffOrder.h"

using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPlugin::Handoff {
    void HandoffCollection::Add(std::shared_ptr<HandoffOrder> order)
    {
        if (this->orders.count(order->id) != 0) {
            LogWarning("Duplicate handoff order added " + std::to_string(order->id));
            return;
        }

        this->orders[order->id] = std::move(order);
    }

    auto HandoffCollection::Get(int id) const -> const std::shared_ptr<HandoffOrder>
    {
        auto handoff = this->orders.find(id);
        return handoff == this->orders.cend() ? handoff->second : this->noOrder;
    }

    auto HandoffCollection::Count() const -> size_t
    {
        return this->orders.size();
    }
} // namespace UKControllerPlugin::Handoff
