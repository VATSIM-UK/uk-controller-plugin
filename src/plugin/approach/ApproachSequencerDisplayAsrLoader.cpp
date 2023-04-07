#include "ApproachSequencerDisplayAsrLoader.h"
#include "ApproachSequencerDisplayOptions.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPlugin::Approach {

    ApproachSequencerDisplayAsrLoader::ApproachSequencerDisplayAsrLoader(
        std::shared_ptr<ApproachSequencerDisplayOptions> options)
        : options(options)
    {
    }

    void ApproachSequencerDisplayAsrLoader::AsrLoadedEvent(Euroscope::UserSetting& userSetting)
    {
        options->Position(
            {userSetting.GetIntegerEntry(this->X_POSITION_ASR_KEY, 200),
             userSetting.GetIntegerEntry(this->Y_POSITION_ASR_KEY, 200)});

        options->SetVisible(userSetting.GetBooleanEntry(this->VISIBLE_ASR_KEY, false));

        options->SetCollapsed(userSetting.GetBooleanEntry(this->COLLAPSED_ASR_KEY, false));
        options->Airfield(userSetting.GetStringEntry(this->AIRFIELD_ASR_KEY));
    }

    void ApproachSequencerDisplayAsrLoader::AsrClosingEvent(Euroscope::UserSetting& userSetting)
    {
        userSetting.Save(this->X_POSITION_ASR_KEY, this->X_POSITION_ASR_DESC, options->Position().x);
        userSetting.Save(this->Y_POSITION_ASR_KEY, this->Y_POSITION_ASR_DESC, options->Position().y);
        userSetting.Save(this->VISIBLE_ASR_KEY, this->VISIBLE_ASR_DESC, options->IsVisible());
        userSetting.Save(this->COLLAPSED_ASR_KEY, this->COLLAPSED_ASR_DESC, options->ContentCollapsed());
        userSetting.Save(this->AIRFIELD_ASR_KEY, this->AIRFIELD_ASR_DESC, options->Airfield());
    }
} // namespace UKControllerPlugin::Approach
