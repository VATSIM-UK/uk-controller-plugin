#include "login/Login.h"
#include "controller/ControllerStatusEventHandlerCollection.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::Test;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Controller::Login;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::EventHandler::MockControllerStatusEventHandlerInterface;

namespace UKControllerPluginTest {
    namespace Controller {

        class LoginTest : public Test
        {
            public:
            LoginTest() : login(this->mockLoopback, this->collection)
            {
            }

            const NiceMock<MockEuroscopePluginLoopbackInterface> mockLoopback;
            ControllerStatusEventHandlerCollection collection;
            Login login;
        };

        TEST_F(LoginTest, DefaultsToLoggedOutStatus)
        {
            EXPECT_EQ(this->login.loggedOut, this->login.GetLoginStatus());
        }

        TEST_F(LoginTest, DefaultsLoginTime)
        {
            EXPECT_EQ(this->login.GetDefaultLoginTime(), this->login.GetLoginTime());
        }

        TEST_F(LoginTest, SetsLoginTimeIfDirectLogin)
        {
            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_DIRECT));

            this->login.TimedEventTrigger();
            EXPECT_NE(this->login.GetLoginTime(), this->login.GetDefaultLoginTime());
        }

        TEST_F(LoginTest, SetsLoginTimeIfSweatboxLogin)
        {
            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_SWEATBOX));

            this->login.TimedEventTrigger();
            EXPECT_NE(this->login.GetLoginTime(), this->login.GetDefaultLoginTime());
        }

        TEST_F(LoginTest, SetsLoginTimeIfSimulatorServerLogin)
        {
            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_SIMULATOR_SERVER));

            this->login.TimedEventTrigger();
            EXPECT_NE(this->login.GetLoginTime(), this->login.GetDefaultLoginTime());
        }

        TEST_F(LoginTest, SetsLoginTimeIfSimulatorConnection)
        {
            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_SIMULATOR_CLIENT));

            this->login.TimedEventTrigger();
            EXPECT_NE(this->login.GetLoginTime(), this->login.GetDefaultLoginTime());
        }

        TEST_F(LoginTest, DoesNotSetLoginTimeIfViaProxy)
        {
            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_VIA_PROXY));

            this->login.TimedEventTrigger();
            EXPECT_EQ(this->login.GetLoginTime(), this->login.GetDefaultLoginTime());
        }

        TEST_F(LoginTest, DoesNotSetLoginTimeIfPlayback)
        {
            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_PLAYBACK));

            this->login.TimedEventTrigger();
            EXPECT_EQ(this->login.GetLoginTime(), this->login.GetDefaultLoginTime());
        }

        TEST_F(LoginTest, ControllerStatusEventDoesNothingIfControllerNotMe)
        {
            NiceMock<MockEuroScopeCControllerInterface> controller;
            ON_CALL(controller, IsCurrentUser()).WillByDefault(Return(false));

            this->login.ControllerUpdateEvent(controller);
            EXPECT_EQ(this->login.GetLoginTime(), this->login.GetDefaultLoginTime());
        }

        TEST_F(LoginTest, ControllerStatusEventDoesLoginIfMe)
        {
            NiceMock<MockEuroScopeCControllerInterface> controller;
            ON_CALL(controller, IsCurrentUser()).WillByDefault(Return(true));

            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_DIRECT));

            this->login.ControllerUpdateEvent(controller);
            EXPECT_NE(this->login.GetLoginTime(), this->login.GetDefaultLoginTime());
        }

        TEST_F(LoginTest, IsLoggedInDirect)
        {
            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_DIRECT));

            this->login.TimedEventTrigger();
            EXPECT_EQ(this->login.loggedIn, this->login.GetLoginStatus());
        }

        TEST_F(LoginTest, IsLoggedInSimulatorServer)
        {
            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_SIMULATOR_SERVER));

            this->login.TimedEventTrigger();
            EXPECT_EQ(this->login.loggedIn, this->login.GetLoginStatus());
        }

        TEST_F(LoginTest, IsLoggedInSimulatorClient)
        {
            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_SIMULATOR_CLIENT));

            this->login.TimedEventTrigger();
            EXPECT_EQ(this->login.loggedIn, this->login.GetLoginStatus());
        }

        TEST_F(LoginTest, IsLoggedInSweatbox)
        {
            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_SWEATBOX));

            this->login.TimedEventTrigger();
            EXPECT_EQ(this->login.loggedIn, this->login.GetLoginStatus());
        }

        TEST_F(LoginTest, IsLoggedInProxy)
        {
            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_VIA_PROXY));

            Login login(mockLoopback, collection);
            this->login.TimedEventTrigger();
            EXPECT_EQ(this->login.proxied, this->login.GetLoginStatus());
        }

        TEST_F(LoginTest, IsLoggedInNoConnection)
        {
            ON_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .WillByDefault(Return(EuroScopePlugIn::CONNECTION_TYPE_NO));

            this->login.TimedEventTrigger();
            EXPECT_EQ(this->login.loggedOut, this->login.GetLoginStatus());
        }

        TEST_F(LoginTest, LoggingOutTriggersSelfDisconnectEvent)
        {
            EXPECT_CALL(this->mockLoopback, GetEuroscopeConnectionStatus())
                .Times(2)
                .WillOnce(Return(EuroScopePlugIn::CONNECTION_TYPE_DIRECT))
                .WillOnce(Return(EuroScopePlugIn::CONNECTION_TYPE_NO));

            std::shared_ptr<MockControllerStatusEventHandlerInterface> mockHandler(
                new NiceMock<MockControllerStatusEventHandlerInterface>);

            EXPECT_CALL(*mockHandler, SelfDisconnectEvent()).Times(1);

            this->collection.RegisterHandler(mockHandler);

            this->login.TimedEventTrigger();
            this->login.TimedEventTrigger();
        }

        TEST_F(LoginTest, SetLoginTimeSetsTime)
        {
            std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
            this->login.SetLoginTime(time);
            EXPECT_EQ(this->login.GetLoginTime(), time);
        }

        TEST_F(LoginTest, SetLoginStatusSetsStatus)
        {
            this->login.SetLoginStatus(1);
            EXPECT_EQ(1, this->login.GetLoginStatus());
        }
    } // namespace Controller
} // namespace UKControllerPluginTest
