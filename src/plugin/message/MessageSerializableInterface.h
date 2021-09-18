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
        virtual std::string MessageHandler(void) const = 0;

        /*
            Returns who the message comes from.
        */
        virtual std::string MessageSender(void) const = 0;

        /*
            Returns the string of the message.
        */
        virtual std::string MessageString(void) const = 0;

        /*
            Returns whether or not the message handler should be shown when
            this message is sent.
        */
        virtual bool MessageShowHandler(void) const = 0;

        /*
            Returns whether or not the message handler should be marked
            as unread when this message is sent.
        */
        virtual bool MessageMarkUnread(void) const = 0;

        /*
            Returns whether or not the message should override
            the busy indicator of the controller.
        */
        virtual bool MessageOverrideBusy(void) const = 0;

        /*
            Returns whether or not the sending of this message
            should flash the chat handler.
        */
        virtual bool MessageFlashHandler(void) const = 0;

        /*
            Returns whether or not this message requires confirmation
            from the user that they have read it.
        */
        virtual bool MessageRequiresConfirm(void) const = 0;
    };
} // namespace UKControllerPlugin::Message
