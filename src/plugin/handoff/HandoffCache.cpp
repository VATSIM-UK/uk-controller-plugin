#include "HandoffCache.h"
#include "ResolvedHandoff.h"

namespace UKControllerPlugin::Handoff {

    void HandoffCache::Add(const std::shared_ptr<ResolvedHandoff>& handoff)
    {
        if (this->handoffs.count(handoff->callsign) != 0) {
            LogWarning("Duplicate cached handoff detected for " + handoff->callsign);
        }

        this->handoffs[handoff->callsign] = handoff;
    }

    auto HandoffCache::Count() const -> size_t
    {
        return this->handoffs.size();
    }

    auto HandoffCache::Get(const std::string& callsign) const -> std::shared_ptr<ResolvedHandoff>
    {
        return this->handoffs.count(callsign) == 0 ? nullptr : this->handoffs.at(callsign);
    }

    void HandoffCache::Delete(const std::string& callsign)
    {
        this->handoffs.erase(callsign);
    }
    
    void HandoffCache::Clear()
    {
        this->handoffs.clear();
    }
    
    void HandoffCache::DeleteWhere(const std::function<bool(const std::shared_ptr<ResolvedHandoff>)>& predicate)
    {
        for (auto handoff = this->handoffs.cbegin(); handoff != this->handoffs.cend();) {
            if (predicate(handoff->second)) {
                handoff = this->handoffs.erase(handoff);
            } else {
                ++handoff;
            }
        }
    }
} // namespace UKControllerPlugin::Handoff
