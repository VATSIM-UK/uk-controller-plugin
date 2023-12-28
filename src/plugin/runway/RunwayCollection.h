#pragma once
#include "RunwayCollectionFactory.h"

namespace UKControllerPlugin::Runway {
    class Runway;

    /*
     * A collection of all the runways stored in the plugin API.
     */
    class RunwayCollection
    {
        public:
        void Add(std::shared_ptr<class Runway> runway);
        [[nodiscard]] auto Count() const -> size_t;
        [[nodiscard]] auto GetById(int id) const -> std::shared_ptr<class Runway>;
        [[nodiscard]] auto GetByAirfieldAndIdentifier(int airfieldId, const std::string& identifier) const
            -> std::shared_ptr<class Runway>;
        [[nodiscard]] auto GetByAirfieldAndIdentifier(const std::string& airfield, const std::string& identifier) const
            -> std::shared_ptr<class Runway>;

        private:
        // All the runways
        std::map<int, std::shared_ptr<class Runway>> runways;
        std::unordered_map<int, std::unordered_map<std::string, std::shared_ptr<class Runway>>>
            runwaysByAirfieldIdAndIdentifier;
        std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<class Runway>>>
            runwaysByAirfieldAndIdentifier;
    };
} // namespace UKControllerPlugin::Runway
