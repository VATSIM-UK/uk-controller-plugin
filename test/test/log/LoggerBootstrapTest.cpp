#include "pch/pch.h"
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

namespace UKControllerPluginTest {
    namespace Log {

        class LoggerBootstrapTest : public Test
        {
            public:
                PersistenceContainer container;
        };

        TEST_F(LoggerBootstrapTest, ItNotifiesTheUserIfLogsFolderCannotBeCreated)
        {
            std::unique_ptr<NiceMock<MockWinApi>> mockWindows(new NiceMock<MockWinApi>);

            std::wstring expected = L"Unable to create logs folder, please contact the VATSIM UK Web Department.\n\n";
            expected += L"Plugin events will not be logged.";

            ON_CALL(*mockWindows, CreateLocalFolderRecursive)
                .WillByDefault(Return(false));
            
            EXPECT_CALL(*mockWindows, OpenMessageBox(StrEq(expected.c_str()), _, _))
                .Times(1);

            container.windows = std::move(mockWindows);

            LoggerBootstrap::Bootstrap(container, false);
        }

    }  // namespace Log
}  // namespace UKControllerPluginTest
