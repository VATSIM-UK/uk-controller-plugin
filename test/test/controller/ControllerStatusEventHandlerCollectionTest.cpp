#include "pch/pch.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "mock/MockControllerStatusEventHandlerInterface.h"
#include "euroscope/EuroScopeCControllerWrapper.h"

using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPluginTest::EventHandler::MockControllerStatusEventHandlerInterface;
using UKControllerPlugin::Euroscope::EuroScopeCControllerWrapper;

using ::testing::StrictMock;
using ::testing::Ref;

namespace UKControllerPluginTest {
    namespace EventHandler {

        TEST(ControllerStatusEventHandlerCollection, ControllerDisconnectEventCallsCorrectMethodOnHandler)
        {
            ControllerStatusEventHandlerCollection collection;
            std::shared_ptr<StrictMock<MockControllerStatusEventHandlerInterface>> mockHandler(
                new StrictMock<MockControllerStatusEventHandlerInterface>
            );
            EuroScopeCControllerWrapper controller(EuroScopePlugIn::CController(), false);

            EXPECT_CALL(*mockHandler, ControllerDisconnectEvent(Ref(controller)))
                .Times(1);

            collection.RegisterHandler(mockHandler);
            collection.ControllerDisconnectEvent(controller);
        }

        TEST(ControllerStatusEventHandlerCollection, ControllerUpdateEventCallsCorrectMethodOnHandler)
        {
            ControllerStatusEventHandlerCollection collection;
            std::shared_ptr<StrictMock<MockControllerStatusEventHandlerInterface>> mockHandler(
                new StrictMock<MockControllerStatusEventHandlerInterface>
            );
            EuroScopeCControllerWrapper controller(EuroScopePlugIn::CController(), false);

            EXPECT_CALL(*mockHandler, ControllerUpdateEvent(Ref(controller)))
                .Times(1);

            collection.RegisterHandler(mockHandler);
            collection.ControllerUpdateEvent(controller);
        }

        TEST(ControllerStatusEventHandlerCollection, SelfDisconnectEventCallsCorrectMethodOnHandler)
        {
            ControllerStatusEventHandlerCollection collection;
            std::shared_ptr<StrictMock<MockControllerStatusEventHandlerInterface>> mockHandler(
                new StrictMock<MockControllerStatusEventHandlerInterface>
            );

            EXPECT_CALL(*mockHandler, SelfDisconnectEvent())
                .Times(1);

            collection.RegisterHandler(mockHandler);
            collection.SelfDisconnectEvent();
        }
    }  // namespace EventHandler
}  // namespace UKControllerPluginTest
