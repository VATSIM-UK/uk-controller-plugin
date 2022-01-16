#pragma once

#include "AircraftTypeCollectionFactory.h"

namespace UKControllerPlugin::Aircraft {
    class AircraftType;

    /**
     * A collection of all the aircraft types
     */
    class AircraftTypeCollection
    {
        public:
        void Add(std::shared_ptr<AircraftType> type);
        [[nodiscard]] auto Count() const -> size_t;
        [[nodiscard]] auto GetByIcaoCode(const std::string& code) const -> std::shared_ptr<AircraftType>;

        private:
        std::map<int, std::shared_ptr<AircraftType>> types;
    };
} // namespace UKControllerPlugin::Aircraft
