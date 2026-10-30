#include "TestEnvironment.h"
#include "log/LoggerFunctions.h"
#include "mock/MockTaskRunnerInterface.h"
#include "task/RunAsyncTask.h"

using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

void TestEnvironment::SetUp()
{
    // Put in a null logger
    SetLoggerInstance(std::make_shared<spdlog::logger>("null_logger", std::make_shared<spdlog::sinks::null_sink_mt>()));

    // Put a task runner that just runs everything non-async
    SetTaskRunner(std::make_shared<MockTaskRunnerInterface>());

    // Start up GDI so we can use its functions, even though not drawing to screen.
    Gdiplus::GdiplusStartupInput gdiStartup;
    GdiplusStartup(&this->gdiPlusToken, &gdiStartup, NULL);
}

void TestEnvironment::TearDown()
{
    Gdiplus::GdiplusShutdown(this->gdiPlusToken);
}
