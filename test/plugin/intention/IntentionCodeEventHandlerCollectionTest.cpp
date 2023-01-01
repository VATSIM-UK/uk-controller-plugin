#include "intention/AircraftIntentionCode.h"
#include "intention/IntentionCodeEventHandlerCollection.h"
#include "intention/IntentionCodeEventHandlerInterface.h"

using UKControllerPlugin::IntentionCode::AircraftIntentionCode;
using UKControllerPlugin::IntentionCode::IntentionCodeEventHandlerCollection;
using UKControllerPlugin::IntentionCode::IntentionCodeEventHandlerInterface;

namespace UKControllerPluginTest::IntentionCode {

    // Mock for the test
    class IntentionCodeEventHandlerCollectionTestInterfaceMock : public IntentionCodeEventHandlerInterface
    {
        public:
        void IntentionCodeUpdated(const AircraftIntentionCode& intentionCode) override
        {
            passedCode = &intentionCode;
            updatedCalled = true;
        }

        const AircraftIntentionCode* passedCode = nullptr;
        bool updatedCalled = false;
    };

    class IntentionCodeEventHandlerCollectionTest : public testing::Test
    {
        public:
        IntentionCodeEventHandlerCollectionTest()
            : handler1(std::make_shared<IntentionCodeEventHandlerCollectionTestInterfaceMock>()),
              handler2(std::make_shared<IntentionCodeEventHandlerCollectionTestInterfaceMock>())
        {
        }

        std::shared_ptr<IntentionCodeEventHandlerCollectionTestInterfaceMock> handler1;
        std::shared_ptr<IntentionCodeEventHandlerCollectionTestInterfaceMock> handler2;
        IntentionCodeEventHandlerCollection collection;
    };

    TEST_F(IntentionCodeEventHandlerCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.CountHandlers());
    }

    TEST_F(IntentionCodeEventHandlerCollectionTest, ItAddsHandlers)
    {
        collection.AddHandler(handler1);
        collection.AddHandler(handler2);
        EXPECT_EQ(2, collection.CountHandlers());
    }

    TEST_F(IntentionCodeEventHandlerCollectionTest, ItDoesntAddDuplicateHandlers)
    {
        collection.AddHandler(handler1);
        collection.AddHandler(handler1);
        collection.AddHandler(handler1);
        collection.AddHandler(handler2);
        collection.AddHandler(handler2);
        collection.AddHandler(handler2);
        EXPECT_EQ(2, collection.CountHandlers());
    }

    TEST_F(IntentionCodeEventHandlerCollectionTest, ItCallsHandlers)
    {
        collection.AddHandler(handler1);
        collection.AddHandler(handler2);

        AircraftIntentionCode code;
        collection.IntentionCodeUpdated(code);

        EXPECT_TRUE(handler1->updatedCalled);
        EXPECT_EQ(&code, handler1->passedCode);

        EXPECT_TRUE(handler2->updatedCalled);
        EXPECT_EQ(&code, handler2->passedCode);
    }
} // namespace UKControllerPluginTest::IntentionCode
