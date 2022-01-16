#pragma once

namespace UKControllerPlugin::Aircraft {

    /**
     * Represents a type of aircraft
     */
    class AircraftType
    {
        public:
        AircraftType(int id, std::string icaoCode, std::set<int> wakeCategories);
        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto IcaoCode() const -> const std::string&;
        [[nodiscard]] auto WakeCategories() const -> const std::set<int>&;

        private:
        // The id of the aircraft in the API
        int id;

        // The aircrafts ICAO type code, e.g. B738
        std::string icaoCode;

        // The wake categories that the aircraft is in
        std::set<int> wakeCategories;
    };
} // namespace UKControllerPlugin::Aircraft
