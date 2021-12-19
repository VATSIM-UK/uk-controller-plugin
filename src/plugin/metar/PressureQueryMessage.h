#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin::Metar {
    class PressureComponent;

    /*
        A message to be used when the pressure at an airfield changes.
    */
    class PressureQueryMessage : public UKControllerPlugin::Message::MessageSerializableInterface
    {
        public:
        PressureQueryMessage(
            std::string airfield, const PressureComponent& pressure);

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
        [[nodiscard]] auto Qnh() const -> std::string;
        [[nodiscard]] auto Qfe() const -> std::string;
        [[nodiscard]] static auto FormatInHg(float pressure) -> std::string;

        std::string airfield;
        const PressureComponent& pressure;

        static inline const int INHG_BUFFER_SIZE = 6;
    };
} // namespace UKControllerPlugin::Metar
