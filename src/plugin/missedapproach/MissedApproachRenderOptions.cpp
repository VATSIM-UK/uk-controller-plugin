#include "MissedApproachRenderOptions.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Ownership::ServiceType;

namespace UKControllerPlugin::MissedApproach {

    MissedApproachRenderOptions::MissedApproachRenderOptions()
        : mode(MissedApproachRenderMode::None), serviceProvisions(Ownership::ServiceType::Invalid), duration(0)
    {
    }

    void MissedApproachRenderOptions::AsrLoadedEvent(Euroscope::UserSetting& userSetting)
    {
        // What to render
        auto asrRenderMode = userSetting.GetUnsignedIntegerEntry(
            this->RENDER_MODE_ASR_KEY, static_cast<unsigned int>(MissedApproachRenderMode::None));

        this->mode = asrRenderMode >= static_cast<unsigned int>(MissedApproachRenderMode::Invalid)
                         ? MissedApproachRenderMode::None
                         : static_cast<MissedApproachRenderMode>(asrRenderMode);

        // When to render
        auto asrServiceProvision = userSetting.GetUnsignedIntegerEntry(
            this->SERVICE_PROVISION_ASR_KEY, static_cast<unsigned int>(ServiceType::Invalid));

        this->serviceProvisions = asrServiceProvision >= static_cast<unsigned int>(ServiceType::Invalid)
                                      ? ServiceType::Invalid
                                      : static_cast<ServiceType>(asrServiceProvision);

        // How long to render
        duration = std::chrono::seconds(
            userSetting.GetUnsignedIntegerEntry(this->SERVICE_PROVISION_ASR_KEY, DEFAULT_RENDER_DURATION));

        // Which airfields to render for
        airfields = userSetting.GetStringListEntry(this->RENDER_FOR_ASR_KEY);
    }

    void MissedApproachRenderOptions::AsrClosingEvent(Euroscope::UserSetting& userSetting)
    {
        userSetting.Save(this->RENDER_MODE_ASR_KEY, this->RENDER_MODE_ASR_DESC, static_cast<unsigned int>(this->mode));

        userSetting.Save(
            this->SERVICE_PROVISION_ASR_KEY,
            this->SERVICE_PROVISION_ASR_DESC,
            static_cast<unsigned int>(this->serviceProvisions));

        userSetting.Save(
            this->RENDER_DURATION_ASR_KEY, this->RENDER_DURATION_ASR_DESC, static_cast<unsigned int>(duration.count()));

        userSetting.Save(this->RENDER_FOR_ASR_KEY, this->RENDER_FOR_ASR_DESC, airfields);
    }

    auto MissedApproachRenderOptions::Mode() const -> MissedApproachRenderMode
    {
        return mode;
    }

    void MissedApproachRenderOptions::SetMode(MissedApproachRenderMode mode)
    {
        this->mode = mode;
    }

    auto MissedApproachRenderOptions::ServiceProvisions() const -> ServiceType
    {
        return serviceProvisions;
    }

    void MissedApproachRenderOptions::SetServiceProvisions(Ownership::ServiceType serviceProvisions)
    {
        this->serviceProvisions = serviceProvisions;
    }

    auto MissedApproachRenderOptions::Duration() const -> const std::chrono::seconds&
    {
        return duration;
    }

    auto MissedApproachRenderOptions::Airfields() const -> const std::vector<std::string>&
    {
        return airfields;
    }

} // namespace UKControllerPlugin::MissedApproach
