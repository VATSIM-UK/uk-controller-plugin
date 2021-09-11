#include "PrenoteMessage.h"
#include "PrenoteMessageCollection.h"
#include "PrenoteStatusIndicatorTagItem.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "tag/TagData.h"

namespace UKControllerPlugin::Prenote {
    PrenoteStatusIndicatorTagItem::PrenoteStatusIndicatorTagItem(std::shared_ptr<PrenoteMessageCollection> messages)
        : messages(std::move(messages))
    {
    }

    auto PrenoteStatusIndicatorTagItem::GetTagItemDescription([[maybe_unused]] int tagItemId) const -> std::string
    {
        return "Prenote Messages Status Indicator";
    }

    /**
     * Set an indicator in the format acknowledged/total of prenote messages
     * for a givev callsign.
     *
     * Set the colour if all are acknowledged.
     */
    void PrenoteStatusIndicatorTagItem::SetTagItemData(Tag::TagData& tagData)
    {
        int totalForCallsign = 0;
        int totalAcknowledged = 0;
        this->messages->Iterate(
            [&tagData, &totalForCallsign, &totalAcknowledged](const std::shared_ptr<PrenoteMessage>& message) {
                if (message->GetCallsign() == tagData.GetFlightplan().GetCallsign()) {
                    totalForCallsign++;

                    if (message->IsAcknowledged()) {
                        totalAcknowledged++;
                    }
                }
            });

        if (totalForCallsign == 0) {
            return;
        }

        tagData.SetItemString(std::to_string(totalAcknowledged) + "/" + std::to_string(totalForCallsign));
        if (totalAcknowledged == totalForCallsign) {
            tagData.SetTagColour(RGB(36, 138, 73));
        }
    }
} // namespace UKControllerPlugin::Prenote
