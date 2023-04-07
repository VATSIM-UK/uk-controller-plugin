#pragma once
#include "controller/ControllerStatusEventHandlerInterface.h"
#include "euroscope/EuroScopeCControllerInterface.h"

namespace UKControllerPluginTest {
    namespace EventHandler {
        class MockControllerStatusEventHandlerInterface
            : public UKControllerPlugin::Controller::ControllerStatusEventHandlerInterface
        {
            public:
            MockControllerStatusEventHandlerInterface();
            virtual ~MockControllerStatusEventHandlerInterface();
            MOCK_METHOD1(ControllerUpdateEvent, void(UKControllerPlugin::Euroscope::EuroScopeCControllerInterface&));
            MOCK_METHOD1(
                ControllerDisconnectEvent, void(UKControllerPlugin::Euroscope::EuroScopeCControllerInterface&));
            MOCK_METHOD0(SelfDisconnectEvent, void(void));
        };
    } // namespace EventHandler
} // namespace UKControllerPluginTest
