#include "pch/pch.h"
#include "metar/PressureMonitor.h"
#include "euroscope/UserSetting.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "message/UserMessager.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "euroscope/GeneralSettingsEntries.h"

using UKControllerPlugin::Metar::PressureMonitor;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using testing::Test;
using testing::NiceMock;
using testing::Return;
using testing::_;

namespace UKControllerPluginTest {
    namespace Metar {

        class PressureMonitorTest : public Test
        {
            public:
                PressureMonitorTest()
                    : userSetting(mockUserSettingProvider), messager(mockPlugin), monitor(userSetting, messager)
                {

                }
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
                UserMessager messager;
                UserSetting userSetting;
                PressureMonitor monitor;
        };

        TEST_F(PressureMonitorTest, ItDoesNothingIfNoQnhInMetar)
        {
            this->monitor.NewMetar("EGKK", "EGKK 02012KT SCT002");
            EXPECT_EQ(this->monitor.qnhNotStored, this->monitor.GetStoredQnh("EGKK"));
        }

        TEST_F(PressureMonitorTest, ItSetsQnhOnFirstTimeMetar)
        {
            this->monitor.NewMetar("EGKK", "EGKK 02012KT Q1011 SCT002");
            EXPECT_EQ("1011", this->monitor.GetStoredQnh("EGKK"));
        }

        TEST_F(PressureMonitorTest, ItDoesntSendUpdateMessageOnFirstTimeQnh)
        {
            EXPECT_CALL(this->mockPlugin, ChatAreaMessage(_, _, _, _, _, _, _, _))
                .Times(0);

            this->monitor.NewMetar("EGKK", "EGKK 02012KT Q1011 SCT002");
        }

        TEST_F(PressureMonitorTest, ItHandlesSameQnh)
        {
            this->monitor.NewMetar("EGKK", "EGKK 02012KT Q1011 SCT002");
            this->monitor.NewMetar("EGKK", "EGKK 02012KT Q1011 SCT002");
            EXPECT_EQ("1011", this->monitor.GetStoredQnh("EGKK"));
        }

        TEST_F(PressureMonitorTest, ItDoesntSendUpdateMessageOnSameQnh)
        {
            EXPECT_CALL(this->mockPlugin, ChatAreaMessage(_, _, _, _, _, _, _, _))
                .Times(0);

            ON_CALL(this->mockUserSettingProvider, GetKey(GeneralSettingsEntries::pressureMonitorSendMessageKey))
                .WillByDefault(Return("1"));

            this->monitor.NewMetar("EGKK", "EGKK 02012KT Q1011 SCT002");
            this->monitor.NewMetar("EGKK", "EGKK 02012KT Q1011 SCT002");
        }

        TEST_F(PressureMonitorTest, ItSendsUpdateMessageOnNewQnh)
        {
            EXPECT_CALL(
                this->mockPlugin, 
                ChatAreaMessage(_, _, "New QNH at EGKK, Was: 1011, Now: 1012" , _, _, _, _, _)
            )
                .Times(1);

            ON_CALL(this->mockUserSettingProvider, GetKey(GeneralSettingsEntries::pressureMonitorSendMessageKey))
                .WillByDefault(Return("1"));

            this->monitor.NewMetar("EGKK", "EGKK 02012KT Q1011 SCT002");
            this->monitor.NewMetar("EGKK", "EGKK 02012KT Q1012 SCT002");
        }

        TEST_F(PressureMonitorTest, ItDoesntSendUpdateMessageIfTurnedOff)
        {
            EXPECT_CALL(this->mockPlugin, ChatAreaMessage(_, _, _, _, _, _, _, _))
                .Times(0);

            ON_CALL(this->mockUserSettingProvider, GetKey(GeneralSettingsEntries::pressureMonitorSendMessageKey))
                .WillByDefault(Return("0"));

            this->monitor.NewMetar("EGKK", "EGKK 02012KT Q1011 SCT002");
            this->monitor.NewMetar("EGKK", "EGKK 02012KT Q1012 SCT002");
        }

        TEST_F(PressureMonitorTest, ItStoresUpdatedQnh)
        {
            ON_CALL(this->mockUserSettingProvider, GetKey(GeneralSettingsEntries::pressureMonitorSendMessageKey))
                .WillByDefault(Return("0"));

            this->monitor.NewMetar("EGKK", "EGKK 02012KT Q1011 SCT002");
            this->monitor.NewMetar("EGKK", "EGKK 02012KT Q1012 SCT002");
            EXPECT_EQ("1012", this->monitor.GetStoredQnh("EGKK"));
        }
    }  // namespace Metar
}  // namespace UKControllerPluginTest
