#include "bootstrap/ExternalsBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"
#include "dialog/DialogManager.h"
#include "graphics/GdiplusBrushes.h"
#include "graphics/GdiGraphicsWrapper.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Bootstrap::ExternalsBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPluginTest::Windows::MockWinApi;

using testing::_;
using testing::NiceMock;
using testing::Return;

namespace UKControllerPluginTest::Bootstrap {

    class ExternalsBootstrapTest : public ::testing::Test
    {
        public:
        /*
            Get the local app data path
        */
        static auto GetLocalAppDataPath(void) -> std::wstring
        {
            TCHAR* localApPDataPath = nullptr;
            SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_SIMPLE_IDLIST, nullptr, &localApPDataPath);

            std::wstring widePath(localApPDataPath);
            std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
            CoTaskMemFree(localApPDataPath);
            return widePath;
        }

        auto GetExpectedUkcpFolder() -> std::wstring
        {
            return this->GetLocalAppDataPath() + L"/UKControllerPlugin";
        }

        NiceMock<MockWinApi> winApiMock;
    };

    TEST_F(ExternalsBootstrapTest, BootstrapCreatesCurlApi)
    {
        PersistenceContainer container;
        HINSTANCE dll = 0;
        ExternalsBootstrap::Bootstrap(container, dll);

        EXPECT_NO_THROW(container.curl->MakeCurlRequest(CurlRequest("", CurlRequest::METHOD_GET)));
    }

    TEST_F(ExternalsBootstrapTest, BootstrapCreatesWindowsApi)
    {
        PersistenceContainer container;
        HINSTANCE dll = 0;
        ExternalsBootstrap::Bootstrap(container, dll);

        EXPECT_EQ(
            container.windows->GetFullPathToLocalFile(L"testfile.json"),
            ExternalsBootstrap::GetPluginFileRoot() + L"/testfile.json");
    }

    TEST_F(ExternalsBootstrapTest, BootstrapCreatesDialogManager)
    {
        PersistenceContainer container;
        HINSTANCE dll = 0;
        ExternalsBootstrap::Bootstrap(container, dll);

        EXPECT_EQ(0, container.dialogManager->CountDialogs());
    }

    TEST_F(ExternalsBootstrapTest, BootstrapCreatesBrushes)
    {
        PersistenceContainer container;
        HINSTANCE dll = 0;
        ExternalsBootstrap::Bootstrap(container, dll);

        EXPECT_NO_THROW(container.brushes->euroscopeBackgroundBrush->GetType());
    }

    TEST_F(ExternalsBootstrapTest, BootstrapCreatesGraphicsWrapper)
    {
        PersistenceContainer container;
        HINSTANCE dll = 0;
        ExternalsBootstrap::Bootstrap(container, dll);

        HDC handle;
        EXPECT_NO_THROW(container.graphics->SetDeviceHandle(handle));
    }

    TEST_F(ExternalsBootstrapTest, GetPluginFileRootReturnsLocalAppDataFolder)
    {
        std::wstring expected = this->GetLocalAppDataPath() + L"/UKControllerPlugin";
        EXPECT_EQ(this->GetExpectedUkcpFolder(), ExternalsBootstrap::GetPluginFileRoot());
    }

    TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootThrowsExceptionOnFailedCreatingUkcpFolder)
    {

        ON_CALL(this->winApiMock, CreateFolderRecursive(this->GetExpectedUkcpFolder())).WillByDefault(Return(false));

        EXPECT_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock), std::runtime_error);
    }

    TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootCreatesFolders)
    {
        ON_CALL(this->winApiMock, SetPermissions(this->GetExpectedUkcpFolder(), std::filesystem::perms::all))
            .WillByDefault(Return(true));

        EXPECT_CALL(this->winApiMock, CreateFolderRecursive(this->GetExpectedUkcpFolder()))
            .Times(1)
            .WillOnce(Return(true));

        EXPECT_NO_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock));
    }

    TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootSetsPermissions)
    {
        EXPECT_CALL(this->winApiMock, CreateFolderRecursive(this->GetExpectedUkcpFolder()))
            .WillRepeatedly(Return(true));

        EXPECT_CALL(this->winApiMock, SetPermissions(this->GetExpectedUkcpFolder(), std::filesystem::perms::all))
            .Times(1)
            .WillOnce(Return(true));

        EXPECT_NO_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock));
    }

    TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootThrowsExceptionOnFailedSettingFolderPermissions)
    {
        ON_CALL(this->winApiMock, CreateFolderRecursive(_)).WillByDefault(Return(true));

        ON_CALL(this->winApiMock, SetPermissions(this->GetExpectedUkcpFolder(), std::filesystem::perms::all))
            .WillByDefault(Return(false));

        EXPECT_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock), std::runtime_error);
    }
} // namespace UKControllerPluginTest::Bootstrap
