#include "api/ApiException.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "prenote/AcknowledgePrenoteMessage.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageCollection.h"
#include "time/SystemClock.h"

using testing::NiceMock;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::AcknowledgePrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Time::TimeNow;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest::Prenote {
    class AcknowledgePrenoteMessageTest : public ::testing::Test
    {
        public:
        AcknowledgePrenoteMessageTest()
            : collection(std::make_shared<PrenoteMessageCollection>()),
              handler(collection, callsigns, mockTaskRunner, mockApi)
        {
            ON_CALL(mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            collection->Add(std::make_shared<PrenoteMessage>(1, "BAW123", "EGGD", "BADIM1X", "EGLL", 1, 2, TimeNow()));
            collection->Add(std::make_shared<PrenoteMessage>(2, "BAW123", "EGGD", "BADIM1X", "EGLL", 1, 3, TimeNow()));
            collection->Add(std::make_shared<PrenoteMessage>(4, "BAW456", "EGGD", "BADIM1X", "EGLL", 1, 2, TimeNow()));

            // Default the user to active
            userPosition = std::make_shared<ControllerPosition>(
                2, "LON_S_CTR", 129.420, std::vector<std::string>{"EGKK"}, true, false, true);
            callsigns.AddUserCallsign(ActiveCallsign("LON_S_CTR", "Test", *userPosition));
        }

        ActiveCallsignCollection callsigns;
        NiceMock<MockTaskRunnerInterface> mockTaskRunner;
        NiceMock<MockApiInterface> mockApi;
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        std::shared_ptr<ControllerPosition> userPosition;
        std::shared_ptr<PrenoteMessageCollection> collection;
        AcknowledgePrenoteMessage handler;
    };

    TEST_F(AcknowledgePrenoteMessageTest, ItAcknowledgesThePrenote)
    {
        EXPECT_CALL(mockApi, AcknowledgePrenoteMessage(1, 2)).Times(1);

        handler.Acknowledge(mockFlightplan);

        EXPECT_TRUE(collection->GetById(1)->IsAcknowledged());
        EXPECT_FALSE(collection->GetById(2)->IsAcknowledged());
        EXPECT_FALSE(collection->GetById(4)->IsAcknowledged());
    }

    TEST_F(AcknowledgePrenoteMessageTest, ItHandlesApiExceptionWhenAcknowledging)
    {
        EXPECT_CALL(mockApi, AcknowledgePrenoteMessage(1, 2)).Times(1).WillOnce(testing::Throw(ApiException("Foo")));

        handler.Acknowledge(mockFlightplan);

        EXPECT_FALSE(collection->GetById(1)->IsAcknowledged());
        EXPECT_FALSE(collection->GetById(2)->IsAcknowledged());
        EXPECT_FALSE(collection->GetById(4)->IsAcknowledged());
    }

    TEST_F(AcknowledgePrenoteMessageTest, ItDoesntAcknowledgeIfTheUserIsNotActive)
    {
        callsigns.Flush();
        EXPECT_CALL(mockApi, AcknowledgePrenoteMessage(testing::_, testing::_)).Times(0);

        handler.Acknowledge(mockFlightplan);

        EXPECT_FALSE(collection->GetById(1)->IsAcknowledged());
        EXPECT_FALSE(collection->GetById(2)->IsAcknowledged());
        EXPECT_FALSE(collection->GetById(4)->IsAcknowledged());
    }

    TEST_F(AcknowledgePrenoteMessageTest, ItDoesntAcknowledgeTheWrongCallsign)
    {
        collection->Remove(1);
        collection->Remove(2);
        EXPECT_CALL(mockApi, AcknowledgePrenoteMessage(testing::_, testing::_)).Times(0);

        handler.Acknowledge(mockFlightplan);

        EXPECT_FALSE(collection->GetById(4)->IsAcknowledged());
    }

    TEST_F(AcknowledgePrenoteMessageTest, ItDoesntAcknowledgeTheWrongTargetController)
    {
        collection->Remove(1);
        collection->Remove(4);

        EXPECT_CALL(mockApi, AcknowledgePrenoteMessage(testing::_, testing::_)).Times(0);

        handler.Acknowledge(mockFlightplan);

        EXPECT_FALSE(collection->GetById(2)->IsAcknowledged());
    }
} // namespace UKControllerPluginTest::Prenote
