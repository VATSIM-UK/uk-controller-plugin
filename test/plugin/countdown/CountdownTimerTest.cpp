#include "pch/pch.h"
#include "mock/MockWinApi.h"
#include "helper/Matchers.h"
#include "countdown/CountdownTimer.h"

using ::testing::StrictMock;
using ::testing::_;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest {
    namespace Countdown {

        // Avoiding name clashes
        using UKControllerPlugin::Countdown::CountdownTimer;

        TEST(Countdown, GetCurrentTimeStringReturnsCorrectZeroValue)
        {
            MockWinApi winApiMock;
            CountdownTimer timer(winApiMock);
            EXPECT_EQ(0, lstrcmp(timer.GetCurrentTimeString(0).str().c_str(), L"0:00"));
        }

        TEST(Countdown, GetCurrentTimeStringReturnsCorrectValueNoMinutesSecondsLessThanTen)
        {
            MockWinApi winApiMock;
            CountdownTimer timer(winApiMock);
            EXPECT_EQ(0, lstrcmp(timer.GetCurrentTimeString(5).str().c_str(), L"0:05"));
        }

        TEST(Countdown, GetCurrentTimeStringReturnsCorrectValueNoMinutesSecondsGreaterThanTen)
        {
            MockWinApi winApiMock;
            CountdownTimer timer(winApiMock);
            EXPECT_EQ(0, lstrcmp(timer.GetCurrentTimeString(25).str().c_str(), L"0:25"));
        }

        TEST(Countdown, GetCurrentTimeStringReturnsCorrectValueWithMinutesSecondsLessThanTen)
        {
            MockWinApi winApiMock;
            CountdownTimer timer(winApiMock);
            EXPECT_EQ(0, lstrcmp(timer.GetCurrentTimeString(67).str().c_str(), L"1:07"));
        }

        TEST(Countdown, GetCurrentTimeStringReturnsCorrectValueWithMinutesSecondsGreaterThanTen)
        {
            MockWinApi winApiMock;
            CountdownTimer timer(winApiMock);
            EXPECT_EQ(0, lstrcmp(timer.GetCurrentTimeString(93).str().c_str(), L"1:33"));
        }

        TEST(Countdown, ResetTimerSetsEndTimeToNull)
        {
            MockWinApi winApiMock;
            CountdownTimer timer(winApiMock);
            timer.StartTimer(120);
            EXPECT_NE(NULL, timer.GetSecondsRemaining());
            timer.ResetTimer(false);
            EXPECT_EQ(NULL, timer.GetSecondsRemaining());
        }

        TEST(Countdown, ResetTimerPlaysSoundIfRequested)
        {
            StrictMock<MockWinApi> winApiMock;

            EXPECT_CALL(winApiMock, PlayWave(_))
                .Times(1);

            CountdownTimer timer(winApiMock);
            timer.StartTimer(60);
            timer.ResetTimer(true);
        }
    }  // namespace Countdown
}  // namespace UKControllerPluginTest
