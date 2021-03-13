#include "pch/utilstestpch.h"
#include "log/LoggerBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockWinApi.h"

using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Log::LoggerBootstrap;
using testing::Test;
using testing::NiceMock;
using testing::StrEq;
using testing::Return;
using testing::_;

namespace UKControllerPluginUtilsTest {
    namespace Log {

        class LoggerBootstrapTest : public Test
        {
            public:
                NiceMock<MockWinApi> windows;
        };

        TEST_F(LoggerBootstrapTest, ItNotifiesTheUserIfLogsFolderCannotBeCreated)
        {
            std::wstring expected = L"Unable to create logs folder, please contact the VATSIM UK Web Department.\n\n";
            expected += L"Plugin events will not be logged.";

            ON_CALL(windows, CreateLocalFolderRecursive)
                .WillByDefault(Return(false));

            EXPECT_CALL(windows, OpenMessageBox(StrEq(expected.c_str()), _, _))
                .Times(1);

            LoggerBootstrap::Bootstrap(windows, L"foo");
        }

    }  // namespace Log
}  // namespace UKControllerPluginUtilsTest
