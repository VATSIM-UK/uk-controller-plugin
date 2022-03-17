#pragma once
#include "ApproachSequencingMode.h"

namespace UKControllerPlugin::Approach {
    /**
     * All the options for the approach sequencer that aren't specific
     * to the display itself
     */
    class ApproachSequencerOptions
    {
        public:
        void DefaultMode(const std::string& airfield, ApproachSequencingMode mode);
        [[nodiscard]] auto DefaultMode(const std::string& airfield) const -> ApproachSequencingMode;
        void MinimumSeparation(const std::string& airfield, double separation);
        [[nodiscard]] auto MinimumSeparation(const std::string& airfield) const -> double;
        void TargetDistance(const std::string& airfield, double target);
        [[nodiscard]] auto TargetDistance(const std::string& airfield) const -> double;
        
        private:
        
        // The default separation to use
        inline static const double DEFAULT_SEPARATION = 3.0;
        
        // The default mode to use
        inline static const ApproachSequencingMode DEFAULT_MODE = ApproachSequencingMode::WakeTurbulence;
        
        // The default target distance to use
        inline static const double DEFAULT_TARGET_DISTANCE = 6.0;
        
        // The default mode to use when adding a new aircraft to the sequencer, per airfield
        std::map<std::string, ApproachSequencingMode> defaultModes;
        
        // The default target distance if in minimum distance mode
        std::map<std::string, double> targetDistances;
        
        // The minimum allowable separation per airfield
        std::map<std::string, double> minimumSeparations;
    };
} // namespace UKControllerPlugin::Approach
