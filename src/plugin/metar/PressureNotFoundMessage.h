#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin::Metar {
    class PressureComponent;

    /*
        A message to be used when the pressure at an airfield changes.
    */
    class PressureNotFoundMessage : public UKControllerPlugin::Message::MessageSerializableInterface
    {
        public:
        PressureNotFoundMessage(std::string airfield);

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
        std::string airfield;
    };
} // namespace UKControllerPlugin::Metar
