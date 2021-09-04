#include "PrenoteMessage.h"
#include "PrenoteMessageCollection.h"
#include "PrenoteMessageTimeout.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPlugin::Prenote {

    PrenoteMessageTimeout::PrenoteMessageTimeout(std::shared_ptr<PrenoteMessageCollection> messages)
        : messages(std::move(messages))
    {
    }

    void PrenoteMessageTimeout::TimedEventTrigger()
    {
        this->messages->RemoveWhere([](const std::shared_ptr<PrenoteMessage>& message) -> bool {
            return message->IsAcknowledged()
                       ? message->GetAcknowledgedAt() < TimeNow() - std::chrono::minutes(ACKNOWLEDGED_PRENOTE_TIMEOUT)
                       : message->GetExpiresAt() < TimeNow();
        });
    }
} // namespace UKControllerPlugin::Prenote
