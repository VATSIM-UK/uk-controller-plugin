#pragma once

namespace UKControllerPlugin::HistoryTrail {

    // Pre-declarations in the namespace
    class AircraftHistoryTrail;
    // END

    /*
        This class stores all the history trails currently in use by the plugin.
        It provides a public interface that allows other classes to register and unregister
        aircraft, update aircraft positions and retrieve the trail.
    */
    class HistoryTrailRepository
    {
        public:
        [[nodiscard]] auto GetAircraft(const std::string& callsign) -> std::shared_ptr<AircraftHistoryTrail>;
        [[nodiscard]] auto HasAircraft(const std::string& callsign) const -> bool;
        void UnregisterAircraft(const std::string& callsign);
        void RegisterAircraft(std::shared_ptr<AircraftHistoryTrail>);

        // Public type definitions for a custom iterator over the class.
        using HistoryTrails = std::vector<std::shared_ptr<AircraftHistoryTrail>>;
        using const_iterator = HistoryTrails::const_iterator;

        [[nodiscard]] auto cbegin() const -> const_iterator
        {
            return trailData.cbegin();
        }

        [[nodiscard]] auto cend() const -> const_iterator
        {
            return trailData.cend();
        }

        // Map of callsign to history trail
        HistoryTrails trailData;

        private:
        // Unordered map of callsign to trail, for ease of lookup and update.
        std::unordered_map<std::string, std::shared_ptr<AircraftHistoryTrail>> trailMap;
    };
} // namespace UKControllerPlugin::HistoryTrail
