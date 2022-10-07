#pragma once
#include "prenote/PrenoteMessageEventHandlerInterface.h"

using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageEventHandlerInterface;

namespace UKControllerPluginTest::Prenote {

    class MockPrenoteMessageEventHandlerInterface : public PrenoteMessageEventHandlerInterface
    {
        public:
        MOCK_METHOD(void, NewMessage, (const PrenoteMessage&), (override));
        MOCK_METHOD(void, MessageCancelled, (const PrenoteMessage&), (override));
        MOCK_METHOD(void, MessageAcknowledged, (const PrenoteMessage&), (override));
        MOCK_METHOD(void, MessageTimeout, (const PrenoteMessage&), (override));
    };
} // namespace UKControllerPluginTest::Prenote
