#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachAcknowledgedPushEventProcessor.h"
#include "missedapproach/MissedApproachCollection.h"
#include "message/UserMessager.h"
#include "push/PushEventSubscription.h"

using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::MissedApproach::MissedApproachAcknowledgedPushEventProcessor;
using UKControllerPlugin::MissedApproach::MissedApproachCollection;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachAcknowledgedPushEventProcessorTest : public testing::Test
    {
        public:
        MissedApproachAcknowledgedPushEventProcessorTest() : messager(mockPlugin), processor(collection, messager)
        {
        }

        /*
         * Make an event based on the merge of some base data and overriding data so we dont
         * have to repeat ourselves
         */
        [[nodiscard]] static auto MakePushEvent(
            const nlohmann::json& overridingData = nlohmann::json::object(), const std::string& keyToRemove = "")
            -> PushEvent
        {
            nlohmann::json eventData{{"id", 1}, {"acknowledged_by", "EGLL_N_APP"}, {"remarks", "Hi."}};
            if (overridingData.is_object()) {
                eventData.update(overridingData);
            } else {
                eventData = overridingData;
            }

            if (!keyToRemove.empty()) {
                eventData.erase(eventData.find(keyToRemove));
            }

            return {"missed-approach.acknowledged", "test", eventData, eventData.dump()};
        };

        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        UserMessager messager;
        MissedApproachCollection collection;
        MissedApproachAcknowledgedPushEventProcessor processor;
    };

    TEST_F(MissedApproachAcknowledgedPushEventProcessorTest, ItHasInterestedEvents)
    {
        std::set<PushEventSubscription> expected{
            {PushEventSubscription::SUB_TYPE_EVENT,
             "missed-approach"
             ".acknowledged"}};
        EXPECT_EQ(expected, processor.GetPushEventSubscriptions());
    }

    TEST_F(MissedApproachAcknowledgedPushEventProcessorTest, ItAcknowledgesAMissedApproach)
    {
        auto approach = std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), true);
        collection.Add(approach);

        processor.ProcessPushEvent(MakePushEvent());

        EXPECT_EQ("EGLL_N_APP", approach->AcknowledgedBy());
        EXPECT_EQ("Hi.", approach->Remarks());
    }

    TEST_F(MissedApproachAcknowledgedPushEventProcessorTest, ItTriggersUserMessageIfMissedApproachCreatedByUser)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                "MISSED_APPROACH",
                "UKCP",
                "Missed approach for BAW123 acknowledged by EGLL_N_APP. Remarks: Hi.",
                true,
                true,
                true,
                true,
                true))
            .Times(1);

        auto approach = std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), true);
        collection.Add(approach);

        processor.ProcessPushEvent(MakePushEvent());
    }

    TEST_F(MissedApproachAcknowledgedPushEventProcessorTest, ItDoesntTriggerUserMessageIfMissedApproachNotCreatedByUser)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        auto approach = std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), false);
        collection.Add(approach);

        processor.ProcessPushEvent(MakePushEvent());
    }

    TEST_F(MissedApproachAcknowledgedPushEventProcessorTest, ItDoesntAcknowledgeApproachIfApproachNotFound)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        collection.Add(std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), true));
        processor.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"id", 2}})));

        EXPECT_FALSE(collection.Get(1)->IsAcknowledged());
    }

    TEST_F(MissedApproachAcknowledgedPushEventProcessorTest, ItDoesntAcknowledgeApproachIfIdMissing)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        collection.Add(std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), true));
        processor.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "id"));

        EXPECT_FALSE(collection.Get(1)->IsAcknowledged());
    }

    TEST_F(MissedApproachAcknowledgedPushEventProcessorTest, ItDoesntAcknowledgeApproachIfIdNotInteger)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        collection.Add(std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), true));
        processor.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"id", "abc"}})));

        EXPECT_FALSE(collection.Get(1)->IsAcknowledged());
    }

    TEST_F(MissedApproachAcknowledgedPushEventProcessorTest, ItDoesntAcknowledgeApproachIfAcknowledgedByMissing)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        collection.Add(std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), true));
        processor.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "acknowledged_by"));

        EXPECT_FALSE(collection.Get(1)->IsAcknowledged());
    }

    TEST_F(MissedApproachAcknowledgedPushEventProcessorTest, ItDoesntAcknowledgeApproachIfAcknowledgedByNotString)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        collection.Add(std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), true));
        processor.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"acknowledged_by", 2}})));

        EXPECT_FALSE(collection.Get(1)->IsAcknowledged());
    }

    TEST_F(MissedApproachAcknowledgedPushEventProcessorTest, ItDoesntAcknowledgeApproachIfRemarksMissing)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        collection.Add(std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), true));
        processor.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "remarks"));

        EXPECT_FALSE(collection.Get(1)->IsAcknowledged());
    }

    TEST_F(MissedApproachAcknowledgedPushEventProcessorTest, ItDoesntAcknowledgeApproachIfRemarksNotString)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        collection.Add(std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), true));
        processor.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"remarks", 2}})));

        EXPECT_FALSE(collection.Get(1)->IsAcknowledged());
    }

    TEST_F(MissedApproachAcknowledgedPushEventProcessorTest, ItDoesntAcknowledgeApproachIfMessageNotObject)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        collection.Add(std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), true));
        processor.ProcessPushEvent(MakePushEvent(nlohmann::json::array()));

        EXPECT_FALSE(collection.Get(1)->IsAcknowledged());
    }
} // namespace UKControllerPluginTest::MissedApproach
