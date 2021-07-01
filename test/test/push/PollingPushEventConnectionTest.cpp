#include "pch/pch.h"
#include "push/PollingPushEventConnection.h"
#include "push/PushEventProcessorCollection.h"
#include "push/PushEventSubscription.h"
#include "mock/MockTaskRunnerInterface.h"
#include "mock/MockPushEventProcessor.h"
#include "mock/MockApiInterface.h"
#include "api/ApiException.h"

using ::testing::Test;
using testing::NiceMock;
using testing::Return;
using testing::Throw;
using UKControllerPlugin::Push::PollingPushEventConnection;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPluginTest::Push::MockPushEventProcessor;
using UKControllerPlugin::Api::ApiException;

namespace UKControllerPluginTest {
    namespace Push {

        class PollingPushEventConnectionTest : public Test
        {
            public:
                PollingPushEventConnectionTest()
                    : connection(mockApi, mockTaskRunner, collection)
                {
                    this->eventProcessor.reset(new NiceMock<MockPushEventProcessor>);
                }

                std::shared_ptr<NiceMock<MockPushEventProcessor>> eventProcessor;
                PushEventProcessorCollection collection;
                MockTaskRunnerInterface mockTaskRunner;
                NiceMock<MockApiInterface> mockApi;
                PollingPushEventConnection connection;
        };

        TEST_F(PollingPushEventConnectionTest, GetNextMessageReturnsEmptyIfNoMessages)
        {
            EXPECT_EQ("", connection.GetNextMessage());
        }

        TEST_F(PollingPushEventConnectionTest, SyncResponseIsValidIfAllPresent)
        {
            nlohmann::json response = {
                {"event_id", 55}
            };
            EXPECT_TRUE(connection.SyncResponseValid(response));
        }

        TEST_F(PollingPushEventConnectionTest, SyncResponseIsValidIfEventIdNotInteger)
        {
            nlohmann::json response = {
                {"event_id", "55"}
            };
            EXPECT_FALSE(connection.SyncResponseValid(response));
        }

        TEST_F(PollingPushEventConnectionTest, SyncResponseIsValidIfEventIdMissing)
        {
            nlohmann::json response = {
                {"not_event_id", 55}
            };
            EXPECT_FALSE(connection.SyncResponseValid(response));
        }

        TEST_F(PollingPushEventConnectionTest, LatestPluginEventsResponseIsValidIfArray)
        {
            EXPECT_TRUE(connection.LatestPluginEventsResponseValid(nlohmann::json::array()));
        }

        TEST_F(PollingPushEventConnectionTest, LatestPluginEventsResponseIsNotValidIfNotArray)
        {
            EXPECT_FALSE(connection.LatestPluginEventsResponseValid(nlohmann::json::object()));
        }

        TEST_F(PollingPushEventConnectionTest, PluginEventIsValidIfAllPresent)
        {
            nlohmann::json pluginEvent = {
                {"id", 55},
                {"event", nlohmann::json{{"data", nlohmann::json{{"foo", "bar"}}}}}
            };

            EXPECT_TRUE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingPushEventConnectionTest, PluginEventIsNotValidIfEventDataNotObject)
        {
            nlohmann::json pluginEvent = {
                {"id", 55},
                {"event", nlohmann::json{{"data", nlohmann::json::array()}}}
            };

            EXPECT_FALSE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingPushEventConnectionTest, PluginEventIsNotValidIfEventHasNoData)
        {
            nlohmann::json pluginEvent = {
                {"id", 55},
                {"event", nlohmann::json::object()}
            };

            EXPECT_FALSE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingPushEventConnectionTest, PluginEventIsNotValidIfEventNotObject)
        {
            nlohmann::json pluginEvent = {
                {"id", 55},
                {"event", nlohmann::json::array()}
            };

            EXPECT_FALSE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingPushEventConnectionTest, PluginEventIsNotValidIfNoEvent)
        {
            nlohmann::json pluginEvent = {
                {"id", 55},
            };

            EXPECT_FALSE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingPushEventConnectionTest, PluginEventIsNotValidIfIdNotAnInteger)
        {
            nlohmann::json pluginEvent = {
                {"id", "55"},
                {"event", nlohmann::json{{"data", nlohmann::json{{"foo", "bar"}}}}}
            };

            EXPECT_FALSE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingPushEventConnectionTest, PluginEventIsNotValidIfNoId)
        {
            nlohmann::json pluginEvent = {
                {"event", nlohmann::json{{"data", nlohmann::json{{"foo", "bar"}}}}}
            };

            EXPECT_FALSE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingPushEventConnectionTest, ItHandlesApiExceptionsWhenSyncing)
        {
            EXPECT_CALL(mockApi, SyncPluginEvents())
                .Times(1)
                .WillOnce(Throw(ApiException("foo")));

            connection.TimedEventTrigger();
            EXPECT_TRUE(std::chrono::system_clock::now() - connection.LastPollTime() < std::chrono::seconds(2));
        }

        TEST_F(PollingPushEventConnectionTest, ItHandlesInvalidResponseWhenSyncing)
        {
            EXPECT_CALL(mockApi, SyncPluginEvents())
                .Times(1)
                .WillOnce(Return(nlohmann::json::array()));

            connection.TimedEventTrigger();
            EXPECT_TRUE(std::chrono::system_clock::now() - connection.LastPollTime() < std::chrono::seconds(2));
        }

        TEST_F(PollingPushEventConnectionTest, ItSyncsTheLastEventId)
        {
            nlohmann::json syncResponse{
                {"event_id", 55}
            };

            EXPECT_CALL(mockApi, SyncPluginEvents())
                .Times(1)
                .WillOnce(Return(syncResponse));

            connection.TimedEventTrigger();
            EXPECT_TRUE(connection.LastPollTime() < std::chrono::system_clock::now() - connection.pollInterval);
        }

        TEST_F(PollingPushEventConnectionTest, ItTriggersASyncedEventWhenSyncComplete)
        {
            std::set<PushEventSubscription> subs = {{PushEventSubscription::SUB_TYPE_CHANNEL, "channel1"}};

            ON_CALL(*this->eventProcessor, GetPushEventSubscriptions)
                .WillByDefault(Return(subs));

            EXPECT_CALL(*this->eventProcessor, PluginEventsSynced)
                .Times(1);

            this->collection.AddProcessor(this->eventProcessor);

            nlohmann::json syncResponse{
                {"event_id", 55}
            };

            EXPECT_CALL(mockApi, SyncPluginEvents())
                .Times(1)
                .WillOnce(Return(syncResponse));

            connection.TimedEventTrigger();
        }

        TEST_F(PollingPushEventConnectionTest, ItDoesntSyncIfSyncInProgress)
        {
            connection.SetSyncInProgress();

            EXPECT_CALL(mockApi, SyncPluginEvents())
                .Times(0);

            connection.TimedEventTrigger();
        }

        TEST_F(PollingPushEventConnectionTest, TestItDoesntSyncIfRecentlyPolled)
        {
            connection.SetLastPollTime(std::chrono::system_clock::now());

            EXPECT_CALL(mockApi, SyncPluginEvents())
                .Times(0);

            connection.TimedEventTrigger();
        }

        TEST_F(PollingPushEventConnectionTest, ItHandlesApiExceptionWhenUpdating)
        {
            connection.SetSynced();

            EXPECT_CALL(mockApi, GetLatestPluginEvents(0))
                .Times(1)
                .WillOnce(Throw(ApiException("foo")));

            connection.TimedEventTrigger();
            EXPECT_TRUE(std::chrono::system_clock::now() - connection.LastPollTime() < std::chrono::seconds(2));
        }

        TEST_F(PollingPushEventConnectionTest, ItHandlesInvalidApiResponseWhenUpdating)
        {
            connection.SetSynced();

            EXPECT_CALL(mockApi, GetLatestPluginEvents(0))
                .Times(1)
                .WillOnce(Return(nlohmann::json::object()));

            connection.TimedEventTrigger();
            EXPECT_TRUE(std::chrono::system_clock::now() - connection.LastPollTime() < std::chrono::seconds(2));
        }

        TEST_F(PollingPushEventConnectionTest, ItUpdatesLastEventIdWhenUpdating)
        {
            nlohmann::json response = nlohmann::json::array({
                {
                    {"id", 3},
                    {
                        "event",
                        nlohmann::json{
                            {"channel", "foo"},
                            {"data", nlohmann::json{{"bar", "baz"}}}
                        }
                    }
                },
                {
                    {"id", 7},
                    {
                        "event",
                        nlohmann::json{
                            {"channel", "bish"},
                            {"data", nlohmann::json{{"bash", "bosh"}}}
                        }
                    }
                }
            });

            connection.SetSynced();

            EXPECT_CALL(mockApi, GetLatestPluginEvents(0))
                .Times(1)
                .WillOnce(Return(response));

            connection.TimedEventTrigger();

            EXPECT_EQ(7, connection.LastEventId());
        }

        TEST_F(PollingPushEventConnectionTest, ItSetsLastPollTimeFollowingUpdate)
        {
            nlohmann::json response = nlohmann::json::array({
                {
                    {"id", 3},
                    {
                        "event",
                        nlohmann::json{
                            {"channel", "foo"},
                            {"data", nlohmann::json{{"bar", "baz"}}}
                        }
                    }
                },
                {
                    {"id", 7},
                    {
                        "event",
                        nlohmann::json{
                            {"channel", "bish"},
                            {"data", nlohmann::json{{"bash", "bosh"}}}
                        }
                    }
                }
            });

            connection.SetSynced();

            EXPECT_CALL(mockApi, GetLatestPluginEvents(0))
                .Times(1)
                .WillOnce(Return(response));

            connection.TimedEventTrigger();

            EXPECT_TRUE(std::chrono::system_clock::now() - connection.LastPollTime() < std::chrono::seconds(2));
        }

        TEST_F(PollingPushEventConnectionTest, TestItProcessesPushEventMessages)
        {
            nlohmann::json response = nlohmann::json::array({
                {
                    {"id", 3},
                    {
                        "event",
                        nlohmann::json{
                            {"channel", "foo"},
                            {"data", {{"bar", "baz"}}}
                        }
                    }
                },
                {
                    {"id", 7},
                    {
                        "event",
                        nlohmann::json{
                            {"channel", "bish"},
                            {"data", {{"bash", "bosh"}}}
                        }
                    }
                }
            });

            connection.SetSynced();

            EXPECT_CALL(mockApi, GetLatestPluginEvents(0))
                .Times(1)
                .WillOnce(Return(response));

            connection.TimedEventTrigger();


            std::string firstExpectedMessage = nlohmann::json{
                {"channel", "foo"},
                {"data", {{"bar", "baz"}}}
            }.dump();
            EXPECT_EQ(firstExpectedMessage, connection.GetNextMessage());

            std::string secondExpectedMessage = nlohmann::json{
                {"channel", "bish"},
                {"data", {{"bash", "bosh"}}}
            }.dump();
            EXPECT_EQ(secondExpectedMessage, connection.GetNextMessage());

            EXPECT_EQ("", connection.GetNextMessage());
        }

        TEST_F(PollingPushEventConnectionTest, TestItDoesntUpdateIfUpdateInProgress)
        {
            connection.SetSynced();
            connection.SetUpdateInProgress();

            EXPECT_CALL(mockApi, GetLatestPluginEvents(0))
                .Times(0);

            connection.TimedEventTrigger();
        }

        TEST_F(PollingPushEventConnectionTest, TestItDoesntUpdateIfRecentlyPolled)
        {
            connection.SetSynced();
            connection.SetLastPollTime(std::chrono::system_clock::now());

            EXPECT_CALL(mockApi, GetLatestPluginEvents(0))
                .Times(0);

            connection.TimedEventTrigger();
        }
    } // namespace Push
}  // namespace UKControllerPluginTest
