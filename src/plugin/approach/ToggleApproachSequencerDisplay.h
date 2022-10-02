#pragma once
#include "radarscreen/MenuToggleableDisplayInterface.h"

namespace UKControllerPlugin::Approach {
    class ApproachSequencerDisplayOptions;

    class ToggleApproachSequencerDisplay : public RadarScreen::MenuToggleableDisplayInterface
    {
        public:
        ToggleApproachSequencerDisplay(std::shared_ptr<ApproachSequencerDisplayOptions> options);
        auto MenuItem() const -> std::string override;
        auto IsVisible() const -> bool override;
        void Toggle() override;

        private:
        std::shared_ptr<ApproachSequencerDisplayOptions> options;
    };
} // namespace UKControllerPlugin::Approach
