#include "controller/HandoffEventHandlerCollection.h"
#include "mock/MockEuroScopeCControllerInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"

using UKControllerPlugin::Controller::HandoffEventHandlerCollection;
using UKControllerPlugin::Controller::HandoffEventHandlerInterface;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;

using ::testing::NiceMock;
using ::testing::Ref;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Controller {

        class MockHandler : public HandoffEventHandlerInterface
        {
            public:
            // Inherited via HandoffEventHandler
            void HandoffInitiated(
                EuroScopeCFlightPlanInterface& flightplan,
                EuroScopeCControllerInterface& transferringController,
                EuroScopeCControllerInterface& targetController) override
            {
                EXPECT_EQ("BAW123", flightplan.GetCallsign());
                EXPECT_EQ("LON_S_CTR", transferringController.GetCallsign());
                EXPECT_EQ("LON_C_CTR", targetController.GetCallsign());
                this->hasBeenCalled = true;
            }

            bool hasBeenCalled = false;
        };

        class HandoffEventHandlerCollectionTest : public Test
        {
            public:
            HandoffEventHandlerCollectionTest()
            {
                handler1.reset(new MockHandler);
                handler2.reset(new MockHandler);
            }

            HandoffEventHandlerCollection collection;
            std::shared_ptr<MockHandler> handler1;
            std::shared_ptr<MockHandler> handler2;
        };

        TEST_F(HandoffEventHandlerCollectionTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->collection.CountHandlers());
        }

        TEST_F(HandoffEventHandlerCollectionTest, ItAddsHandlers)
        {
            this->collection.RegisterHandler(this->handler1);
            this->collection.RegisterHandler(this->handler2);
            EXPECT_EQ(2, this->collection.CountHandlers());
        }

        TEST_F(HandoffEventHandlerCollectionTest, ItDoesntDuplicateHandlers)
        {
            this->collection.RegisterHandler(this->handler1);
            this->collection.RegisterHandler(this->handler1);
            this->collection.RegisterHandler(this->handler2);
            this->collection.RegisterHandler(this->handler2);
            EXPECT_EQ(2, this->collection.CountHandlers());
        }

        TEST_F(HandoffEventHandlerCollectionTest, ItCallsEventsOnHandlers)
        {
            NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
            ON_CALL(mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            NiceMock<MockEuroScopeCControllerInterface> mockSender;
            ON_CALL(mockSender, GetCallsign()).WillByDefault(Return("LON_S_CTR"));

            NiceMock<MockEuroScopeCControllerInterface> mockTarget;
            ON_CALL(mockTarget, GetCallsign()).WillByDefault(Return("LON_C_CTR"));

            this->collection.RegisterHandler(this->handler1);
            this->collection.RegisterHandler(this->handler2);

            this->collection.HandoffInitiated(mockFlightplan, mockSender, mockTarget);
            EXPECT_TRUE(this->handler1->hasBeenCalled);
            EXPECT_TRUE(this->handler2->hasBeenCalled);
        }
    } // namespace Controller
} // namespace UKControllerPluginTest
