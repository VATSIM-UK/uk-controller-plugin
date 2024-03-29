#include "message/UserMessager.h"
#include "message/MessageSerializableInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Message::MessageSerializableInterface;

namespace UKControllerPlugin {
    namespace Message {

        UserMessager::UserMessager(EuroscopePluginLoopbackInterface& plugin) : plugin(plugin)
        {
        }

        /*
            Takes something that can be converted into a message and sends it to the user.
        */
        void UserMessager::SendMessageToUser(const MessageSerializableInterface& message)
        {
            this->plugin.ChatAreaMessage(
                message.MessageHandler(),
                message.MessageSender(),
                message.MessageString(),
                message.MessageShowHandler(),
                message.MessageMarkUnread(),
                message.MessageOverrideBusy(),
                message.MessageFlashHandler(),
                message.MessageRequiresConfirm());
        }
    } // namespace Message
} // namespace UKControllerPlugin
