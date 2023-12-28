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
        runwaysByAirfieldIdAndIdentifier[runway->AirfieldId()][runway->Identifier()] = runway;
        runwaysByAirfieldAndIdentifier[runway->AirfieldIdentifier()][runway->Identifier()] = runway;
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
        if (runwaysByAirfieldIdAndIdentifier.count(airfieldId) == 0) {
            return nullptr;
        }

        if (runwaysByAirfieldIdAndIdentifier.at(airfieldId).count(identifier) == 0) {
            return nullptr;
        }

        return runwaysByAirfieldIdAndIdentifier.at(airfieldId).at(identifier);
    }

    auto RunwayCollection::GetByAirfieldAndIdentifier(const std::string& airfield, const std::string& identifier) const
        -> std::shared_ptr<class Runway>
    {
        if (runwaysByAirfieldAndIdentifier.count(airfield) == 0) {
            return nullptr;
        }

        if (runwaysByAirfieldAndIdentifier.at(airfield).count(identifier) == 0) {
            return nullptr;
        }

        return runwaysByAirfieldAndIdentifier.at(airfield).at(identifier);
    }
} // namespace UKControllerPlugin::Runway
