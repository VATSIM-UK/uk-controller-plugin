#include "WakeCategory.h"
#include "WakeCategoryFactory.h"
#include "WakeScheme.h"
#include "WakeSchemeFactory.h"

namespace UKControllerPlugin::Wake {

    auto WakeSchemeFromJson(const nlohmann::json& json) -> std::shared_ptr<WakeScheme>
    {
        if (!SchemeValid(json)) {
            LogWarning("Invalid wake scheme " + json.dump());
            return nullptr;
        }

        std::list<std::shared_ptr<WakeCategory>> categories;
        for (const auto& category : json.at("categories")) {
            auto wakeCategory = WakeCategoryFromJson(category);
            if (!wakeCategory) {
                continue;
            }

            categories.push_back(std::move(wakeCategory));
        }

        return std::make_shared<WakeScheme>(
            json.at("id").get<int>(),
            json.at("key").get<std::string>(),
            json.at("name").get<std::string>(),
            std::move(categories));
    }

    auto SchemeValid(const nlohmann::json& json) -> bool
    {
        return json.is_object() && json.contains("id") && json.at("id").is_number_integer() && json.contains("key") &&
               json.at("key").is_string() && json.contains("name") && json.at("name").is_string() &&
               json.contains("categories") && json.at("categories").is_array();
    }
} // namespace UKControllerPlugin::Wake
