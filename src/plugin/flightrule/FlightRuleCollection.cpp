#include "FlightRule.h"
#include "FlightRuleCollection.h"

namespace UKControllerPlugin::FlightRules {
    void FlightRuleCollection::Add(const std::shared_ptr<FlightRule>& rule)
    {
        if (this->flightRules.count(rule->id) != 0) {
            LogWarning("Duplicate flight rule " + std::to_string(rule->id));
            return;
        }

        this->flightRules[rule->id] = rule;
    }

    auto FlightRuleCollection::Count() const -> size_t
    {
        return this->flightRules.size();
    }

    auto FlightRuleCollection::Get(int id) const -> std::shared_ptr<FlightRule>
    {
        auto rule = this->flightRules.find(id);
        return rule == this->flightRules.cend() ? nullptr : rule->second;
    }

    auto FlightRuleCollection::GetByEuroscopeKey(const std::string& key) const -> std::shared_ptr<FlightRule>
    {
        auto rule = std::find_if(
            this->flightRules.begin(),
            this->flightRules.end(),
            [&key](const std::pair<int, std::shared_ptr<FlightRule>>& rule) -> bool {
                return rule.second->euroscopeKey == key;
            });
        return rule == this->flightRules.cend() ? nullptr : rule->second;
    }
} // namespace UKControllerPlugin::FlightRules
