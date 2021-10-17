#pragma once
#include "MissedApproachRenderMode.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "ownership/ServiceType.h"

namespace UKControllerPlugin::MissedApproach {
    /**
     * Stores the options for a single missed approach renderer
     * instance.
     */
    class MissedApproachRenderOptions : public Euroscope::AsrEventHandlerInterface
    {
        public:
        MissedApproachRenderOptions();
        void AsrLoadedEvent(Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(Euroscope::UserSetting& userSetting) override;
        [[nodiscard]] auto Mode() const -> MissedApproachRenderMode;
        void SetMode(MissedApproachRenderMode mode);
        [[nodiscard]] auto Duration() const -> const std::chrono::seconds&;
        void SetDuration(std::chrono::seconds duration);

        private:
        // The default render duration
        inline static const unsigned int DEFAULT_RENDER_DURATION = 5;

        // The render mode for missed approaches
        MissedApproachRenderMode mode;

        // How long to render missed approaches for
        std::chrono::seconds duration;

        const std::string RENDER_MODE_ASR_KEY = "missedApproachRenderMode";
        const std::string RENDER_MODE_ASR_DESC = "Missed Approach Render Mode";
        const std::string RENDER_DURATION_ASR_KEY = "missedApproachRenderDuration";
        const std::string RENDER_DURATION_ASR_DESC = "Missed Approach Render Duration";
    };
} // namespace UKControllerPlugin::MissedApproach
