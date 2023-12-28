#include "Runway.h"
#include "RunwayCollection.h"
#include "RunwayCollectionFactory.h"

namespace UKControllerPlugin::Runway {
    auto BuildRunwayCollection(const nlohmann::json& dependency) -> std::shared_ptr<RunwayCollection>
    {
        auto collection = std::make_shared<RunwayCollection>();
        if (!dependency.is_array()) {
            LogWarning("Runway dependency is not an array");
            return collection;
        }

        for (const auto& runway : dependency) {
            if (!RunwayValid(runway)) {
                LogWarning("Invalid runway detected");
                continue;
            }

            EuroScopePlugIn::CPosition threshold;
            threshold.m_Latitude = runway.at("threshold_latitude").get<double>();
            threshold.m_Longitude = runway.at("threshold_longitude").get<double>();

            collection->Add(std::make_shared<class Runway>(
                runway.at("id").get<int>(),
                runway.at("airfield_id").get<int>(),
                runway.at("identifier").get<std::string>(),
                runway.at("heading").get<int>(),
                std::move(threshold)));
        }

        LogInfo("Loaded " + std::to_string(collection->Count()) + " runways");
        return collection;
    }

    auto RunwayValid(const nlohmann::json& runway) -> bool
    {
        return runway.is_object() && runway.contains("id") && runway.at("id").is_number_integer() &&
               runway.contains("airfield_id") && runway.at("airfield_id").is_number_integer() &&
               runway.contains("identifier") && runway.at("identifier").is_string() && runway.contains("heading") &&
               runway.at("heading").is_number_integer() && runway.contains("threshold_latitude") &&
               runway.at("threshold_latitude").is_number() && runway.contains("threshold_longitude") &&
               runway.at("threshold_longitude").is_number();
    }
} // namespace UKControllerPlugin::Runway
