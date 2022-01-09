#include "Runway.h"
#include "RunwayCollection.h"

namespace UKControllerPlugin::Runway {

    void RunwayCollection::Add(std::shared_ptr<class Runway> runway)
    {
        if (this->runways.count(runway->Id()) > 0) {
            LogError("Duplicate runway added: " + std::to_string(runway->Id()));
            return;
        }

        runways[runway->Id()] = runway;
    }

    auto RunwayCollection::Count() const -> size_t
    {
        return runways.size();
    }

    auto RunwayCollection::GetById(int id) const -> std::shared_ptr<class Runway>
    {
        return runways.count(id) == 1 ? runways.at(id) : nullptr;
    }

    auto RunwayCollection::GetByAirfieldAndIdentifier(int airfieldId, const std::string& identifier) const
        -> std::shared_ptr<class Runway>
    {
        auto runway = std::find_if(
            runways.begin(),
            runways.end(),
            [&airfieldId, &identifier](std::pair<int, const std::shared_ptr<class Runway>&> runway) -> bool {
                return runway.second->AirfieldId() == airfieldId && runway.second->Identifier() == identifier;
            });
        return runway != runways.cend() ? runway->second : nullptr;
    }
} // namespace UKControllerPlugin::Runway
