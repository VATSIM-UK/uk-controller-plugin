#include "pch/pch.h"
#include "bootstrap/ExternalsBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlRequest.h"
#include "helper/HelperFunctions.h"
#include "mock/MockWinApi.h"

using UKControllerPlugin::Bootstrap::ExternalsBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::HelperFunctions;
using UKControllerPluginTest::Windows::MockWinApi;

using testing::NiceMock;
using testing::Return;
using testing::_;

namespace UKControllerPluginTest {
    namespace Bootstrap {

        class ExternalsBootstrapTest : public ::testing::Test {
            public:
                /*
                    Get the My Documents path
                */
                std::wstring GetMyDocumentsPath(void)
                {
                    TCHAR * myDocumentsPath = 0;
                    HRESULT result = SHGetKnownFolderPath(
                        FOLDERID_Documents,
                        KF_FLAG_SIMPLE_IDLIST,
                        NULL,
                        &myDocumentsPath
                    );

                    std::wstring widePath(myDocumentsPath);
                    std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
                    CoTaskMemFree(myDocumentsPath);
                    return widePath;
                }

                /*
                    Get the local app data path
                */
                std::wstring GetLocalAppDataPath(void)
                {
                    TCHAR * localApPDataPath = 0;
                    HRESULT result = SHGetKnownFolderPath(
                        FOLDERID_LocalAppData,
                        KF_FLAG_SIMPLE_IDLIST,
                        NULL,
                        &localApPDataPath
                    );

                    std::wstring widePath(localApPDataPath);
                    std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
                    CoTaskMemFree(localApPDataPath);
                    return widePath;
                }

                std::wstring GetExpectedUkcpFolder(void)
                {
                    return this->GetLocalAppDataPath() + L"/UKControllerPlugin";
                }

                std::wstring GetExpectedLegacyUkcpFolder(void)
                {
                    return this->GetMyDocumentsPath() + L"/EuroScope/ukcp";
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
                ExternalsBootstrap::GetPluginFileRoot() + L"/testfile.json"
            );
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

        TEST_F(ExternalsBootstrapTest, GetLegacyPluginFileRootReturnsMyDocumentsEuroscopeFolder)
        {
            EXPECT_EQ(this->GetExpectedLegacyUkcpFolder(), ExternalsBootstrap::GetLegacyPluginFileRoot());
        }

        TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootThrowsExceptionOnFailedCreatingUkcpFolder)
        {

            ON_CALL(this->winApiMock, CreateFolderRecursive(this->GetExpectedUkcpFolder()))
                .WillByDefault(Return(false));


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

            EXPECT_CALL(
                this->winApiMock,
                SetPermissions(this->GetExpectedUkcpFolder(), std::filesystem::perms::all)
            )
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_NO_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock));
        }

        TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootThrowsExceptionOnFailedSettingFolderPermissions)
        {
            ON_CALL(this->winApiMock, CreateFolderRecursive(_))
                .WillByDefault(Return(true));

            ON_CALL(this->winApiMock, SetPermissions(this->GetExpectedUkcpFolder(), std::filesystem::perms::all))
                .WillByDefault(Return(false));

            EXPECT_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock), std::runtime_error);
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPluginTest
