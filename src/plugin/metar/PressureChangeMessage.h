#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin::Metar {
    class PressureComponent;

    /*
        A message to be used when the pressure at an airfield changes.
    */
    class PressureChangeMessage : public UKControllerPlugin::Message::MessageSerializableInterface
    {
        public:
        PressureChangeMessage(
            std::string airfield, const PressureComponent& pressureBefore, const PressureComponent& pressureNow);

        // Inherited via MessageSerializableInterface
        [[nodiscard]] auto MessageHandler() const -> std::string override;
        [[nodiscard]] auto MessageSender() const -> std::string override;
        [[nodiscard]] auto MessageString() const -> std::string override;
        [[nodiscard]] auto MessageShowHandler() const -> bool override;
        [[nodiscard]] auto MessageMarkUnread() const -> bool override;
        [[nodiscard]] auto MessageOverrideBusy() const -> bool override;
        [[nodiscard]] auto MessageFlashHandler() const -> bool override;
        [[nodiscard]] auto MessageRequiresConfirm() const -> bool override;

        private:
        [[nodiscard]] auto QnhBefore() const -> std::string;
        [[nodiscard]] auto QnhAfter() const -> std::string;
        [[nodiscard]] auto QfeBefore() const -> std::string;
        [[nodiscard]] auto QfeAfter() const -> std::string;
        [[nodiscard]] static auto FormatInHg(float pressure) -> std::string;

        std::string airfield;
        const PressureComponent& pressureBefore;
        const PressureComponent& pressureNow;

        static inline const int INHG_BUFFER_SIZE = 6;
    };
} // namespace UKControllerPlugin::Metar
