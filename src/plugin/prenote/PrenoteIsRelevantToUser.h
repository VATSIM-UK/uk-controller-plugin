#pragma once
#include "PrenoteUserRelevanceChecker.h"

namespace UKControllerPlugin::Controller {
    class ActiveCallsignCollection;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Prenote {
    class PrenoteMessage;

    class PrenoteIsRelevantToUser : public PrenoteUserRelevanceChecker
    {
        public:
        PrenoteIsRelevantToUser(const Controller::ActiveCallsignCollection& activeCallsigns);
        [[nodiscard]] auto IsRelevant(const PrenoteMessage& message) const -> bool;

        private:
        // The active controllers
        const Controller::ActiveCallsignCollection& activeCallsigns;
    };
} // namespace UKControllerPlugin::Prenote
