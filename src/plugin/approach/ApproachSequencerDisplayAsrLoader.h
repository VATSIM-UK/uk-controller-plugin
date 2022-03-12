#pragma once
#include "euroscope/AsrEventHandlerInterface.h"

namespace UKControllerPlugin::Approach {
    class ApproachSequencerDisplayOptions;

    class ApproachSequencerDisplayAsrLoader : public Euroscope::AsrEventHandlerInterface
    {
        public:
        ApproachSequencerDisplayAsrLoader(std::shared_ptr<ApproachSequencerDisplayOptions> options);
        void AsrLoadedEvent(Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(Euroscope::UserSetting& userSetting) override;

        private:
        std::shared_ptr<ApproachSequencerDisplayOptions> options;

        const std::string X_POSITION_ASR_KEY = "approachSequencerXPosition";
        const std::string X_POSITION_ASR_DESC = "Approach Sequencer X Position";

        const std::string Y_POSITION_ASR_KEY = "approachSequencerYPosition";
        const std::string Y_POSITION_ASR_DESC = "Approach Sequencer Y Position";

        const std::string VISIBLE_ASR_KEY = "approachSequencerVisible";
        const std::string VISIBLE_ASR_DESC = "Approach Sequencer Visibility";

        const std::string COLLAPSED_ASR_KEY = "approachSequencerContentCollapsed";
        const std::string COLLAPSED_ASR_DESC = "Approach Sequencer Content Collapsed";
    };
} // namespace UKControllerPlugin::Approach
