#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
    namespace Message {
        class MessageSerializableInterface;
    }  // namespace Message
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Message {

        /*
            Handles the sending of messages to the user of the plugin.
        */
        class UserMessager
        {
            public:
                explicit UserMessager(UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin);
                void SendMessageToUser(const UKControllerPlugin::Message::MessageSerializableInterface & message);

            private:
                // The plugin - through which messages are sent
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;
        };
    }  // namespace Message
}  // namespace UKControllerPlugin
