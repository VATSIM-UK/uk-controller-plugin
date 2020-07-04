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

        TEST_F(ExternalsBootstrapTest, GetPluginFileRootReturnsMyDocumentsEuroscopeFolder)
        {
            std::wstring expected = this->GetMyDocumentsPath() + L"/EuroScope/ukcp";
            EXPECT_EQ(expected, ExternalsBootstrap::GetPluginFileRoot());
        }

        TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootThrowsExceptionOnFailedCreatingEuroscopeFolder)
        {
            std::wstring expected = this->GetMyDocumentsPath() + L"/EuroScope";

            ON_CALL(this->winApiMock, CreateFolderRecursive(expected))
                .WillByDefault(Return(false));


            EXPECT_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock), std::runtime_error);
        }

        TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootThrowsExceptionOnFailedCreatingUkcpFolder)
        {
            std::wstring expected = this->GetMyDocumentsPath() + L"/EuroScope/ukcp";

            ON_CALL(this->winApiMock, CreateFolderRecursive(_))
                .WillByDefault(Return(true));

            ON_CALL(this->winApiMock, CreateFolderRecursive(expected))
                .WillByDefault(Return(false));

            EXPECT_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock), std::runtime_error);
        }

        TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootCreatesFolders)
        {
            std::wstring expected = this->GetMyDocumentsPath() + L"/EuroScope/ukcp";

            ON_CALL(this->winApiMock, SetPermissions(expected, std::filesystem::perms::all))
                .WillByDefault(Return(true));

            EXPECT_CALL(this->winApiMock, CreateFolderRecursive(expected))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_NO_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock));
        }

        TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootSetsPermissions)
        {
            std::wstring expected = this->GetMyDocumentsPath() + L"/EuroScope/ukcp";
            EXPECT_CALL(this->winApiMock, CreateFolderRecursive(expected))
                .WillRepeatedly(Return(true));

            EXPECT_CALL(
                this->winApiMock,
                SetPermissions(expected, std::filesystem::perms::all)
            )
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_NO_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock));
        }

        TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootThrowsExceptionOnFailedSettingFolderPermissions)
        {
            std::wstring expected = this->GetMyDocumentsPath() + L"/EuroScope/ukcp";

            ON_CALL(this->winApiMock, CreateFolderRecursive(_))
                .WillByDefault(Return(true));

            ON_CALL(this->winApiMock, SetPermissions(expected, std::filesystem::perms::all))
                .WillByDefault(Return(false));

            EXPECT_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock), std::runtime_error);
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPluginTest
