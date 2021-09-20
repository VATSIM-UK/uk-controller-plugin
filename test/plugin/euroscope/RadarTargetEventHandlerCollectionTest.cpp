#include "euroscope/RadarTargetEventHandlerCollection.h"

using UKControllerPlugin::Euroscope::RadarTargetEventHandlerCollection;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::EventHandler::MockRadarTargetEventHandlerInterface;

using ::testing::_;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace Euroscope {
        TEST(RadarTargetEventHandlerCollection, CallsCorrectMethodRadarTargetEventSingleHandler)
        {
            RadarTargetEventHandlerCollection collection;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockHandler;
            std::shared_ptr<StrictMock<MockRadarTargetEventHandlerInterface>> mockInterface(
                new StrictMock<MockRadarTargetEventHandlerInterface>);

            EXPECT_CALL(*mockInterface, RadarTargetPositionUpdateEvent(_)).Times(1);

            collection.RegisterHandler(mockInterface);
            collection.RadarTargetEvent(mockHandler);
        }

        TEST(RadarTargetEventHandlerCollection, CallsCorrectMethodRadarTargetEventMultipleHandlers)
        {
            RadarTargetEventHandlerCollection collection;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockHandler;
            std::shared_ptr<StrictMock<MockRadarTargetEventHandlerInterface>> mockInterface1(
                new StrictMock<MockRadarTargetEventHandlerInterface>);
            std::shared_ptr<StrictMock<MockRadarTargetEventHandlerInterface>> mockInterface2(
                new StrictMock<MockRadarTargetEventHandlerInterface>);

            EXPECT_CALL(*mockInterface1, RadarTargetPositionUpdateEvent(_)).Times(1);

            EXPECT_CALL(*mockInterface2, RadarTargetPositionUpdateEvent(_)).Times(1);

            collection.RegisterHandler(mockInterface1);
            collection.RegisterHandler(mockInterface2);
            collection.RadarTargetEvent(mockHandler);
        }

        TEST(RadarTargetEventHandlerCollection, StartsEmpty)
        {
            RadarTargetEventHandlerCollection collection;
            EXPECT_EQ(0, collection.CountHandlers());
        }

        TEST(RadarTargetEventHandlerCollection, CountHandlersReturnsNumberOfHandlers)
        {
            RadarTargetEventHandlerCollection collection;
            collection.RegisterHandler(std::shared_ptr<StrictMock<MockRadarTargetEventHandlerInterface>>(
                new StrictMock<MockRadarTargetEventHandlerInterface>));
            EXPECT_EQ(1, collection.CountHandlers());
            collection.RegisterHandler(std::shared_ptr<StrictMock<MockRadarTargetEventHandlerInterface>>(
                new StrictMock<MockRadarTargetEventHandlerInterface>));
            EXPECT_EQ(2, collection.CountHandlers());
            collection.RegisterHandler(std::shared_ptr<StrictMock<MockRadarTargetEventHandlerInterface>>(
                new StrictMock<MockRadarTargetEventHandlerInterface>));
            EXPECT_EQ(3, collection.CountHandlers());
        }

        TEST(RadarTargetEventHandlerCollection, RegisterHandlerDoesNotAddDuplicates)
        {
            RadarTargetEventHandlerCollection collection;
            std::shared_ptr<StrictMock<MockRadarTargetEventHandlerInterface>> mockInterface(
                new StrictMock<MockRadarTargetEventHandlerInterface>);
            collection.RegisterHandler(mockInterface);
            EXPECT_EQ(1, collection.CountHandlers());
            collection.RegisterHandler(mockInterface);
            EXPECT_EQ(1, collection.CountHandlers());
        }
    } // namespace Euroscope
} // namespace UKControllerPluginTest
