#include "metar/MetarComponents.h"
#include "metar/ParsedMetar.h"
#include "metar/PressureComponent.h"
#include "metar/PressureMonitor.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "controller/ControllerPosition.h"

using testing::_;
using testing::NiceMock;
using testing::Return;
using testing::Test;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Metar::MetarComponents;
using UKControllerPlugin::Metar::ParsedMetar;
using UKControllerPlugin::Metar::PressureComponent;
using UKControllerPlugin::Metar::PressureMonitor;
using UKControllerPlugin::Metar::PressureUnit;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest::Metar {

    class PressureMonitorTest : public Test
    {
        public:
        PressureMonitorTest()
            : gatwickTower(1, "EGKK_TWR", 124.22, {"EGKK"}, true, false), messager(mockPlugin),
              userSetting(mockUserSettingProvider), monitor(messager, activeCallsigns)
        {
            this->activeCallsigns.AddUserCallsign(ActiveCallsign("EGKK_TWR", "Testy", this->gatwickTower, true));
        }

        [[nodiscard]] static auto GetParsedMetar(int qnh, int qfe, std::string station = "EGKK") -> ParsedMetar
        {
            auto components = std::make_unique<MetarComponents>();
            components->pressure =
                std::make_shared<PressureComponent>(qnh, qfe, 29.92, 29.93, PressureUnit::Hectopascals);
            return ParsedMetar(station, "foo", std::move(components));
        }

        NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
        NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
        ControllerPosition gatwickTower;
        ActiveCallsignCollection activeCallsigns;
        UserMessager messager;
        UserSetting userSetting;
        PressureMonitor monitor;
    };

    TEST_F(PressureMonitorTest, NotificationsDefaultToOff)
    {
        EXPECT_FALSE(this->monitor.NotificationsEnabled());
    }

    TEST_F(PressureMonitorTest, NotificationsCanBeTurnedOnAndOff)
    {
        this->monitor.SetNotficationsEnabled(true);
        EXPECT_TRUE(this->monitor.NotificationsEnabled());
        this->monitor.SetNotficationsEnabled(false);
        EXPECT_FALSE(this->monitor.NotificationsEnabled());
    }

    TEST_F(PressureMonitorTest, ItDoesNothingIfNoPressureComponents)
    {
        this->monitor.MetarUpdated(ParsedMetar("EGKK", "foo", std::make_unique<MetarComponents>()));
        EXPECT_EQ(nullptr, this->monitor.GetStoredPressure("EGKK"));
    }

    TEST_F(PressureMonitorTest, ItSetsQnhOnFirstTimeMetar)
    {
        this->monitor.MetarUpdated(GetParsedMetar(1011, 1012));
        EXPECT_EQ(1011, monitor.GetStoredPressure("EGKK")->QnhHectopascals());
        EXPECT_EQ(1012, monitor.GetStoredPressure("EGKK")->QfeHectopascals());
    }

    TEST_F(PressureMonitorTest, ItDoesntSendUpdateMessageOnFirstTimeQnh)
    {
        this->monitor.SetNotficationsEnabled(true);

        EXPECT_CALL(this->mockPlugin, ChatAreaMessage(_, _, _, _, _, _, _, _)).Times(0);

        this->monitor.MetarUpdated(GetParsedMetar(1011, 1012));
    }

    TEST_F(PressureMonitorTest, ItHandlesSameQnh)
    {
        this->monitor.MetarUpdated(GetParsedMetar(1011, 1012));
        this->monitor.MetarUpdated(GetParsedMetar(1011, 1012));
        EXPECT_EQ(1011, monitor.GetStoredPressure("EGKK")->QnhHectopascals());
        EXPECT_EQ(1012, monitor.GetStoredPressure("EGKK")->QfeHectopascals());
    }

    TEST_F(PressureMonitorTest, ItDoesntSendUpdateMessageOnSameQnh)
    {
        EXPECT_CALL(this->mockPlugin, ChatAreaMessage(_, _, _, _, _, _, _, _)).Times(0);

        this->monitor.SetNotficationsEnabled(true);

        this->monitor.MetarUpdated(GetParsedMetar(1011, 1012));
        this->monitor.MetarUpdated(GetParsedMetar(1011, 1012));
    }

    TEST_F(PressureMonitorTest, ItSendsUpdateMessageOnNewQnh)
    {
        EXPECT_CALL(
            this->mockPlugin,
            ChatAreaMessage(
                _, _, "Pressure change at EGKK. QNH was 1011, now 1012. QFE was 1012, now 1013.", _, _, _, _, _))
            .Times(1);

        this->monitor.SetNotficationsEnabled(true);

        this->monitor.MetarUpdated(GetParsedMetar(1011, 1012));
        this->monitor.MetarUpdated(GetParsedMetar(1012, 1013));
    }

    TEST_F(PressureMonitorTest, ItDoesntSendMessageIfNonConcernedStation)
    {
        EXPECT_CALL(this->mockPlugin, ChatAreaMessage(_, _, _, _, _, _, _, _)).Times(0);

        this->monitor.SetNotficationsEnabled(true);

        this->monitor.MetarUpdated(GetParsedMetar(1011, 1012, "EGLL"));
        this->monitor.MetarUpdated(GetParsedMetar(1012, 1013, "EGLL"));
    }

    TEST_F(PressureMonitorTest, ItDoesntSendUpdateMessageIfTurnedOff)
    {
        EXPECT_CALL(this->mockPlugin, ChatAreaMessage(_, _, _, _, _, _, _, _)).Times(0);

        this->monitor.MetarUpdated(GetParsedMetar(1011, 1012));
        this->monitor.MetarUpdated(GetParsedMetar(1012, 1013));
    }

    TEST_F(PressureMonitorTest, ItStoresUpdatedQnh)
    {
        ON_CALL(this->mockUserSettingProvider, GetKey(GeneralSettingsEntries::pressureMonitorSendMessageKey))
            .WillByDefault(Return("0"));

        this->monitor.MetarUpdated(GetParsedMetar(1011, 1012));
        this->monitor.MetarUpdated(GetParsedMetar(1012, 1013));
        EXPECT_EQ(1012, this->monitor.GetStoredPressure("EGKK")->QnhHectopascals());
    }

    TEST_F(PressureMonitorTest, ItUpdatesFromUserSettings)
    {
        ON_CALL(this->mockUserSettingProvider, GetKey(GeneralSettingsEntries::pressureMonitorSendMessageKey))
            .WillByDefault(Return("1"));

        this->monitor.UserSettingsUpdated(this->userSetting);
        EXPECT_TRUE(this->monitor.NotificationsEnabled());
    }

    TEST_F(PressureMonitorTest, ItDefaultsToOffIfNoUserSetting)
    {

        ON_CALL(this->mockUserSettingProvider, GetKey(GeneralSettingsEntries::pressureMonitorSendMessageKey))
            .WillByDefault(Return(""));

        this->monitor.SetNotficationsEnabled(true);
        this->monitor.UserSettingsUpdated(this->userSetting);
        EXPECT_FALSE(this->monitor.NotificationsEnabled());
    }
} // namespace UKControllerPluginTest::Metar
