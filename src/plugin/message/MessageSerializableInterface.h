#pragma once

namespace UKControllerPlugin::Message {

    /*
        Represents an object that can be serialized into a message
        that can be displayed to a user.
    */
    class MessageSerializableInterface
    {
        public:
        MessageSerializableInterface();
        virtual ~MessageSerializableInterface();
        MessageSerializableInterface(const MessageSerializableInterface&);
        MessageSerializableInterface(MessageSerializableInterface&&) = delete;
        [[nodiscard]] auto operator=(const MessageSerializableInterface&) -> MessageSerializableInterface&;
        [[nodiscard]] auto operator=(MessageSerializableInterface&&) -> MessageSerializableInterface& = delete;

        /*
            Returns the handler (or, tab) in the chat area that the message
            will be displayed in.
        */
        [[nodiscard]] virtual auto MessageHandler() const -> std::string = 0;

        /*
            Returns who the message comes from.
        */
        [[nodiscard]] virtual auto MessageSender() const -> std::string = 0;

        /*
            Returns the string of the message.
        */
        [[nodiscard]] virtual auto MessageString() const -> std::string = 0;

        /*
            Returns whether or not the message handler should be shown when
            this message is sent.
        */
        [[nodiscard]] virtual auto MessageShowHandler() const -> bool = 0;

        /*
            Returns whether or not the message handler should be marked
            as unread when this message is sent.
        */
        [[nodiscard]] virtual auto MessageMarkUnread() const -> bool = 0;

        /*
            Returns whether or not the message should override
            the busy indicator of the controller.
        */
        [[nodiscard]] virtual auto MessageOverrideBusy() const -> bool = 0;

        /*
            Returns whether or not the sending of this message
            should flash the chat handler.
        */
        [[nodiscard]] virtual auto MessageFlashHandler() const -> bool = 0;

        /*
            Returns whether or not this message requires confirmation
            from the user that they have read it.
        */
        [[nodiscard]] virtual auto MessageRequiresConfirm() const -> bool = 0;
    };
} // namespace UKControllerPlugin::Message
