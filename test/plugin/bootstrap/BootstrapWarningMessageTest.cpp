#include "bootstrap/BootstrapWarningMessage.h"

using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;

namespace UKControllerPluginTest {
    namespace Bootstrap {

        TEST(BootstrapWarningMessage, ItWritesToThePrenoteHandler)
        {
            BootstrapWarningMessage message("This is a test");
            ASSERT_TRUE(message.MessageHandler() == message.handler);
        }

        TEST(BootstrapWarningMessage, ItSendsFromThePlugin)
        {
            BootstrapWarningMessage message("This is a test");
            ASSERT_TRUE(message.MessageSender() == message.sender);
        }

        TEST(BootstrapWarningMessage, ItCompilesAMessageToSend)
        {
            BootstrapWarningMessage message("This is a test");
            ASSERT_TRUE(message.MessageString() == "This is a test");
        }

        TEST(BootstrapWarningMessage, ItShowsTheMessageHandler)
        {
            BootstrapWarningMessage message("This is a test");
            ASSERT_TRUE(message.MessageShowHandler());
        }

        TEST(BootstrapWarningMessage, ItMarksTheHandlerAsUnread)
        {
            BootstrapWarningMessage message("This is a test");
            ASSERT_TRUE(message.MessageMarkUnread());
        }

        TEST(BootstrapWarningMessage, ItOverridesBusyStatus)
        {
            BootstrapWarningMessage message("This is a test");
            ASSERT_TRUE(message.MessageOverrideBusy());
        }

        TEST(BootstrapWarningMessage, ItFlashesTheHandler)
        {
            BootstrapWarningMessage message("This is a test");
            ASSERT_TRUE(message.MessageFlashHandler());
        }

        TEST(BootstrapWarningMessage, ItRequiresAConfirmation)
        {
            BootstrapWarningMessage message("This is a test");
            ASSERT_TRUE(message.MessageRequiresConfirm());
        }
    } // namespace Bootstrap
} // namespace UKControllerPluginTest
