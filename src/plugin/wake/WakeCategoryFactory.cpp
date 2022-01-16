#include "DepartureWakeIntervalFactory.h"
#include "WakeCategory.h"
#include "WakeCategoryFactory.h"

namespace UKControllerPlugin::Wake {

    auto WakeCategoryFromJson(const nlohmann::json& json) -> std::shared_ptr<WakeCategory>
    {
        if (!CategoryValid(json)) {
            LogWarning("Invalid wake category " + json.dump());
            return nullptr;
        }

        std::list<std::shared_ptr<DepartureWakeInterval>> wakeIntervals;
        for (const auto& interval : json.at("subsequent_departure_intervals")) {
            auto wakeInterval = IntervalFromJson(interval);
            if (!wakeInterval) {
                continue;
            }

            wakeIntervals.push_back(wakeInterval);
        }

        return std::make_shared<WakeCategory>(
            json.at("id").get<int>(),
            json.at("code").get<std::string>(),
            json.at("description").get<std::string>(),
            json.at("relative_weighting").get<int>(),
            std::move(wakeIntervals));
    }

    auto CategoryValid(const nlohmann::json& json) -> bool
    {
        return json.is_object() && json.contains("id") && json.at("id").is_number_integer() && json.contains("code") &&
               json.at("code").is_string() && json.contains("description") && json.at("description").is_string() &&
               json.contains("relative_weighting") && json.at("relative_weighting").is_number_integer() &&
               json.contains("subsequent_departure_intervals") && json.at("subsequent_departure_intervals").is_array();
    }
} // namespace UKControllerPlugin::Wake
