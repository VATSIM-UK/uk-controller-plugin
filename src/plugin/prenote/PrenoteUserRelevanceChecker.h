#pragma once

namespace UKControllerPlugin::Prenote {
    class PrenoteMessage;

    class PrenoteUserRelevanceChecker
    {
        public:
        virtual ~PrenoteUserRelevanceChecker() = default;
        [[nodiscard]] virtual auto IsRelevant(const PrenoteMessage& message) const -> bool = 0;
    };
} // namespace UKControllerPlugin::Prenote
