#pragma once

namespace UKControllerPlugin {
    namespace HistoryTrail {

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
            HistoryTrailRepository(void);
            ~HistoryTrailRepository(void);
            std::shared_ptr<AircraftHistoryTrail> GetAircraft(std::string callsign);
            bool HasAircraft(std::string callsign) const;
            void UnregisterAircraft(std::string callsign);
            void RegisterAircraft(std::shared_ptr<AircraftHistoryTrail>);

            // Public type definitions for a custom iterator over the class.
            typedef std::map<std::string, std::shared_ptr<AircraftHistoryTrail>> HistoryTrails;
            typedef HistoryTrails::const_iterator const_iterator;
            const_iterator cbegin(void) const
            {
                return trailData.cbegin();
            }
            const_iterator cend(void) const
            {
                return trailData.cend();
            }

            // Map of callsign to history trail
            HistoryTrails trailData;
        };
    } // namespace HistoryTrail
} // namespace UKControllerPlugin
