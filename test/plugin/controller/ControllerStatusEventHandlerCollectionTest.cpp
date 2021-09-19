#include "controller/ControllerStatusEventHandlerCollection.h"

using testing::NiceMock;
using testing::Ref;
using testing::Test;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPluginTest::EventHandler::MockControllerStatusEventHandlerInterface;

namespace UKControllerPluginTest {
    namespace EventHandler {

        class ControllerStatusEventHandlerCollectionTest : public Test
        {
            public:
            ControllerStatusEventHandlerCollectionTest()
            {
                mockHandler = std::make_shared<NiceMock<MockControllerStatusEventHandlerInterface>>();
            }

            ControllerStatusEventHandlerCollection collection;
            NiceMock<MockEuroScopeCControllerInterface> mockController;
            std::shared_ptr<NiceMock<MockControllerStatusEventHandlerInterface>> mockHandler;
        };

        TEST_F(ControllerStatusEventHandlerCollectionTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, collection.CountHandlers());
        }

        TEST_F(ControllerStatusEventHandlerCollectionTest, ControllerDisconnectEventCallsCorrectMethodOnHandler)
        {
            EXPECT_CALL(*mockHandler, ControllerDisconnectEvent(Ref(mockController))).Times(1);

            collection.RegisterHandler(mockHandler);
            collection.ControllerDisconnectEvent(mockController);
        }

        TEST_F(ControllerStatusEventHandlerCollectionTest, ControllerUpdateEventCallsCorrectMethodOnHandler)
        {
            EXPECT_CALL(*mockHandler, ControllerUpdateEvent(Ref(mockController))).Times(1);

            collection.RegisterHandler(mockHandler);
            collection.ControllerUpdateEvent(mockController);
        }

        TEST_F(ControllerStatusEventHandlerCollectionTest, SelfDisconnectEventCallsCorrectMethodOnHandler)
        {
            EXPECT_CALL(*mockHandler, SelfDisconnectEvent()).Times(1);

            collection.RegisterHandler(mockHandler);
            collection.SelfDisconnectEvent();
        }
    } // namespace EventHandler
} // namespace UKControllerPluginTest
