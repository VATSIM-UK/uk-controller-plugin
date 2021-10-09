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
        [[nodiscard]] auto ServiceProvisions() const -> Ownership::ServiceType;
        void SetServiceProvisions(Ownership::ServiceType serviceProvisions);
        [[nodiscard]] auto Duration() const -> const std::chrono::seconds&;
        [[nodiscard]] auto Airfields() const -> const std::vector<std::string>&;

        private:
        // The default render duration
        inline static const unsigned int DEFAULT_RENDER_DURATION = 55;

        // The render mode for missed approaches
        MissedApproachRenderMode mode;

        // We should render when providing which services
        Ownership::ServiceType serviceProvisions;

        // How long to render missed approaches for
        std::chrono::seconds duration;

        // Which airfields we care about
        std::vector<std::string> airfields;

        const std::string RENDER_MODE_ASR_KEY = "missedApproachRenderMode";
        const std::string RENDER_MODE_ASR_DESC = "Missed Approach Render Mode";
        const std::string RENDER_FOR_ASR_KEY = "missedApproachRenderAirfields";
        const std::string RENDER_FOR_ASR_DESC = "Missed Approach Render Airfields";
        const std::string RENDER_DURATION_ASR_KEY = "missedApproachRenderDuration";
        const std::string RENDER_DURATION_ASR_DESC = "Missed Approach Render Duration";
        const std::string SERVICE_PROVISION_ASR_KEY = "missedApproachRenderServiceProvisions";
        const std::string SERVICE_PROVISION_ASR_DESC = "Missed Approach Render For Service Provision";
    };
} // namespace UKControllerPlugin::MissedApproach
