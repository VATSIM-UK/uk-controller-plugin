#include "pch/pch.h"
#include "regional/RegionalPressureManager.h"
#include "regional/RegionalPressure.h"
#include "push/PushEventSubscription.h"
#include "push/PushEvent.h"

using UKControllerPlugin::Regional::RegionalPressureManager;
using UKControllerPlugin::Regional::RegionalPressure;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::Push::PushEvent;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace MinStack {

        class RegionalPressureManagerTest : public Test
        {
            public:
                RegionalPressureManagerTest()
                    : regional(asrKeys)
                {

                }

                std::map<std::string, std::string> asrKeys = {
                    {"ASR_LONDON", "London"},
                    {"ASR_SCOTTISH", "Scottish"},
                };
                RegionalPressureManager regional;
        };

        TEST_F(RegionalPressureManagerTest, ItCanStoreUserAcknowledgementsOfRegionalPressures)
        {
            this->regional.AddRegionalPressure("ASR_LONDON", "London" , 1013);
            EXPECT_FALSE(this->regional.GetRegionalPressure("ASR_LONDON").IsAcknowledged());
            this->regional.AcknowledgePressure("ASR_LONDON");
            EXPECT_TRUE(this->regional.GetRegionalPressure("ASR_LONDON").IsAcknowledged());
        }

        TEST_F(RegionalPressureManagerTest, ItReturnsAllThePossibleKeysForMinStacks)
        {
            this->regional.AddRegionalPressure("ASR_LONDON", "London", 1013);
            this->regional.AddRegionalPressure("ASR_SCOTTISH", "Scottish", 1012);


            std::set<std::string> expectedKeys = { "ASR_LONDON", "ASR_SCOTTISH" };
            EXPECT_EQ(expectedKeys, this->regional.GetAllRegionalPressureKeys());
        }

        TEST_F(RegionalPressureManagerTest, ItConvertsKeysToNames)
        {
            this->regional.AddRegionalPressure("ASR_LONDON", "London", 1013);
            EXPECT_EQ("London", this->regional.GetNameFromKey("ASR_LONDON"));
        }

        TEST_F(RegionalPressureManagerTest, ItReturnsInvalidMslIfKeyNotFound)
        {
            EXPECT_EQ(this->regional.invalidPressure, this->regional.GetRegionalPressure("nope"));
        }

        TEST_F(RegionalPressureManagerTest, ItReturnsAMsl)
        {
            this->regional.AddRegionalPressure("ASR_LONDON", "London", 1013);
            RegionalPressure pressure = this->regional.GetRegionalPressure("ASR_LONDON");

            EXPECT_EQ(1013, pressure.pressure);
            EXPECT_EQ("London", pressure.name);
            EXPECT_FALSE(pressure.IsAcknowledged());
        }

        TEST_F(RegionalPressureManagerTest, ItHasPushEventSubscriptions)
        {
            std::set<PushEventSubscription> expected = {
                {
                    PushEventSubscription::SUB_TYPE_CHANNEL,
                    "private-rps-updates"
                }
            };
            EXPECT_EQ(expected, this->regional.GetSubscriptions());
        }

        TEST_F(RegionalPressureManagerTest, ItAllowsManualMinStackUpdates)
        {
            this->regional.AddRegionalPressure("ASR_LONDON", "London", 1013);
            this->regional.SetPressure("ASR_LONDON", 1014);

            EXPECT_EQ(1014, this->regional.GetRegionalPressure("ASR_LONDON").pressure);
        }

        TEST_F(RegionalPressureManagerTest, ItUpdatesRegionalPressuresFromJson)
        {
            nlohmann::json pressureData;
            pressureData["pressures"] = {
                {"ASR_LONDON", 1013},
                {"ASR_SCOTTISH", 1014}
            };

            PushEvent message{
                "App\\Events\\RegionalPressuresUpdatedEvent",
                "private-rps-updates",
                pressureData,
                ""
            };

            this->regional.ProcessWebsocketMessage(message);
            EXPECT_EQ(1013, this->regional.GetRegionalPressure("ASR_LONDON").pressure);
            EXPECT_EQ("ASR_LONDON", this->regional.GetRegionalPressure("ASR_LONDON").key);
            EXPECT_EQ("London", this->regional.GetRegionalPressure("ASR_LONDON").name);
            EXPECT_EQ(1014, this->regional.GetRegionalPressure("ASR_SCOTTISH").pressure);
            EXPECT_EQ("ASR_SCOTTISH", this->regional.GetRegionalPressure("ASR_SCOTTISH").key);
            EXPECT_EQ("Scottish", this->regional.GetRegionalPressure("ASR_SCOTTISH").name);
        }

        TEST_F(RegionalPressureManagerTest, ItHandlesNullRegionalPressures)
        {
            nlohmann::json pressureData;
            pressureData["pressures"] = {
                {"ASR_LONDON", nullptr},
                {"ASR_SCOTTISH", nullptr}
            };

            PushEvent message{
                "App\\Events\\RegionalPressuresUpdatedEvent",
                "private-rps-updates",
                pressureData,
                ""
            };

            this->regional.ProcessWebsocketMessage(message);
            EXPECT_EQ(
                this->regional.invalidPressure,
                this->regional.GetRegionalPressure("ASR_LONDON")
            );
            EXPECT_EQ(
                this->regional.invalidPressure,
                this->regional.GetRegionalPressure("ASR_SCOTTISH")
            );
        }

        TEST_F(RegionalPressureManagerTest, ItDoesntDoManualMinStackUpdatesIfItDoesntExist)
        {
            EXPECT_NO_THROW(this->regional.SetPressure("nope", 8000));
        }
    }  // namespace MinStack
}  // namespace UKControllerPluginTest
