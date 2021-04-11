#include "pch/pch.h"
#include "websocket/PollingWebsocketConnection.h"
#include "mock/MockTaskRunnerInterface.h"
#include "mock/MockApiInterface.h"
#include "api/ApiException.h"

using ::testing::Test;
using testing::NiceMock;
using testing::Return;
using testing::Throw;
using UKControllerPlugin::Websocket::PollingWebsocketConnection;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPlugin::Api::ApiException;

namespace UKControllerPluginTest {
    namespace Websocket {

        class PollingWebsocketConnectionTest : public Test
        {
            public:
                PollingWebsocketConnectionTest()
                    : connection(mockApi, mockTaskRunner)
                { }

                MockTaskRunnerInterface mockTaskRunner;
                NiceMock<MockApiInterface> mockApi;
                PollingWebsocketConnection connection;
        };

        TEST_F(PollingWebsocketConnectionTest, GetNextMessageReturnsEmptyIfNoMessages)
        {
            EXPECT_EQ("", connection.GetNextMessage());
        }

        TEST_F(PollingWebsocketConnectionTest, SyncResponseIsValidIfAllPresent)
        {
            nlohmann::json response = {
                {"event_id", 55}
            };
            EXPECT_TRUE(connection.SyncResponseValid(response));
        }

        TEST_F(PollingWebsocketConnectionTest, SyncResponseIsValidIfEventIdNotInteger)
        {
            nlohmann::json response = {
                {"event_id", "55"}
            };
            EXPECT_FALSE(connection.SyncResponseValid(response));
        }

        TEST_F(PollingWebsocketConnectionTest, SyncResponseIsValidIfEventIdMissing)
        {
            nlohmann::json response = {
                {"not_event_id", 55}
            };
            EXPECT_FALSE(connection.SyncResponseValid(response));
        }

        TEST_F(PollingWebsocketConnectionTest, LatestPluginEventsResponseIsValidIfArray)
        {
            EXPECT_TRUE(connection.LatestPluginEventsResponseValid(nlohmann::json::array()));
        }

        TEST_F(PollingWebsocketConnectionTest, LatestPluginEventsResponseIsNotValidIfNotArray)
        {
            EXPECT_FALSE(connection.LatestPluginEventsResponseValid(nlohmann::json::object()));
        }

        TEST_F(PollingWebsocketConnectionTest, PluginEventIsValidIfAllPresent)
        {
            nlohmann::json pluginEvent = {
                {"id", 55},
                {"event", nlohmann::json{{"data", nlohmann::json{{"foo", "bar"}}}}}
            };

            EXPECT_TRUE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingWebsocketConnectionTest, PluginEventIsNotValidIfEventDataNotObject)
        {
            nlohmann::json pluginEvent = {
                {"id", 55},
                {"event", nlohmann::json{{"data", nlohmann::json::array()}}}
            };

            EXPECT_FALSE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingWebsocketConnectionTest, PluginEventIsNotValidIfEventHasNoData)
        {
            nlohmann::json pluginEvent = {
                {"id", 55},
                {"event", nlohmann::json::object()}
            };

            EXPECT_FALSE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingWebsocketConnectionTest, PluginEventIsNotValidIfEventNotObject)
        {
            nlohmann::json pluginEvent = {
                {"id", 55},
                {"event", nlohmann::json::array()}
            };

            EXPECT_FALSE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingWebsocketConnectionTest, PluginEventIsNotValidIfNoEvent)
        {
            nlohmann::json pluginEvent = {
                {"id", 55},
            };

            EXPECT_FALSE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingWebsocketConnectionTest, PluginEventIsNotValidIfIdNotAnInteger)
        {
            nlohmann::json pluginEvent = {
                {"id", "55"},
                {"event", nlohmann::json{{"data", nlohmann::json{{"foo", "bar"}}}}}
            };

            EXPECT_FALSE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingWebsocketConnectionTest, PluginEventIsNotValidIfNoId)
        {
            nlohmann::json pluginEvent = {
                {"event", nlohmann::json{{"data", nlohmann::json{{"foo", "bar"}}}}}
            };

            EXPECT_FALSE(connection.PluginEventValid(pluginEvent));
        }

        TEST_F(PollingWebsocketConnectionTest, ItHandlesApiExceptionsWhenSyncing)
        {
            EXPECT_CALL(mockApi, SyncPluginEvents())
                .Times(1)
                .WillOnce(Throw(ApiException("foo")));

            connection.TimedEventTrigger();
            EXPECT_TRUE(std::chrono::system_clock::now() - connection.LastPollTime() < std::chrono::seconds(2));
        }

        TEST_F(PollingWebsocketConnectionTest, ItHandlesInvalidResponseWhenSyncing)
        {
            EXPECT_CALL(mockApi, SyncPluginEvents())
                .Times(1)
                .WillOnce(Return(nlohmann::json::array()));

            connection.TimedEventTrigger();
            EXPECT_TRUE(std::chrono::system_clock::now() - connection.LastPollTime() < std::chrono::seconds(2));
        }

        TEST_F(PollingWebsocketConnectionTest, ItSyncsTheLastEventId)
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

        TEST_F(PollingWebsocketConnectionTest, ItPushesAConnectionEstablishedMessageWhenSyncComplete)
        {
            nlohmann::json syncResponse{
                {"event_id", 55}
            };

            EXPECT_CALL(mockApi, SyncPluginEvents())
                .Times(1)
                .WillOnce(Return(syncResponse));

            connection.TimedEventTrigger();

            std::string expectedMessage = nlohmann::json({
                {"event", "pusher:connection_established"},
                {"data", nlohmann::json({{"socket_id", "abc.def"}, {"activity_timeout", 30}}).dump()}
            }).dump();
            EXPECT_EQ(expectedMessage, connection.GetNextMessage());
            EXPECT_EQ("", connection.GetNextMessage());
        }

        TEST_F(PollingWebsocketConnectionTest, ItDoesntSyncIfSyncInProgress)
        {
            connection.SetSyncInProgress();

            EXPECT_CALL(mockApi, SyncPluginEvents())
                .Times(0);

            connection.TimedEventTrigger();
        }

        TEST_F(PollingWebsocketConnectionTest, TestItDoesntSyncIfRecentlyPolled)
        {
            connection.SetLastPollTime(std::chrono::system_clock::now());

            EXPECT_CALL(mockApi, SyncPluginEvents())
                .Times(0);

            connection.TimedEventTrigger();
        }

        TEST_F(PollingWebsocketConnectionTest, ItHandlesApiExceptionWhenUpdating)
        {
            connection.SetSynced();

            EXPECT_CALL(mockApi, GetLatestPluginEvents(0))
                .Times(1)
                .WillOnce(Throw(ApiException("foo")));

            connection.TimedEventTrigger();
            EXPECT_TRUE(std::chrono::system_clock::now() - connection.LastPollTime() < std::chrono::seconds(2));
        }

        TEST_F(PollingWebsocketConnectionTest, ItHandlesInvalidApiResponseWhenUpdating)
        {
            connection.SetSynced();

            EXPECT_CALL(mockApi, GetLatestPluginEvents(0))
                .Times(1)
                .WillOnce(Return(nlohmann::json::object()));

            connection.TimedEventTrigger();
            EXPECT_TRUE(std::chrono::system_clock::now() - connection.LastPollTime() < std::chrono::seconds(2));
        }

        TEST_F(PollingWebsocketConnectionTest, ItUpdatesLastEventIdWhenUpdating)
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

        TEST_F(PollingWebsocketConnectionTest, ItSetsLastPollTimeFollowingUpdate)
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

        TEST_F(PollingWebsocketConnectionTest, TestItProcessesWebsocketMessages)
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


            std::string firstExpectedMessage = nlohmann::json{
                {"channel", "foo"},
                {"data", nlohmann::json{{"bar", "baz"}}.dump()}
            }.dump();
            EXPECT_EQ(firstExpectedMessage, connection.GetNextMessage());

            std::string secondExpectedMessage = nlohmann::json{
                {"channel", "bish"},
                {"data", nlohmann::json{{"bash", "bosh"}}.dump()}
            }.dump();
            EXPECT_EQ(secondExpectedMessage, connection.GetNextMessage());

            EXPECT_EQ("", connection.GetNextMessage());
        }

        TEST_F(PollingWebsocketConnectionTest, TestItDoesntUpdateIfUpdateInProgress)
        {
            connection.SetSynced();
            connection.SetUpdateInProgress();

            EXPECT_CALL(mockApi, GetLatestPluginEvents(0))
                .Times(0);

            connection.TimedEventTrigger();
        }

        TEST_F(PollingWebsocketConnectionTest, TestItDoesntUpdateIfRecentlyPolled)
        {
            connection.SetSynced();
            connection.SetLastPollTime(std::chrono::system_clock::now());

            EXPECT_CALL(mockApi, GetLatestPluginEvents(0))
                .Times(0);

            connection.TimedEventTrigger();
        }
    }  // namespace Websocket
}  // namespace UKControllerPluginTest
