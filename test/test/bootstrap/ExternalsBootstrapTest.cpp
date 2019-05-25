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
                    TCHAR myDocumentsPath[MAX_PATH];
                    HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, myDocumentsPath);

                    std::wstring widePath(myDocumentsPath);
                    std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
                    return widePath;
                }

                /*
                    Get the My Documents path
                */
                std::string GetMyDocumentsPathNarrow(void)
                {
                    TCHAR myDocumentsPath[MAX_PATH];
                    HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, myDocumentsPath);

                    std::wstring widePath(myDocumentsPath);
                    std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
                    return HelperFunctions::ConvertToRegularString(widePath);
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
                container.windows->GetFullPathToLocalFile("testfile.json"),
                ExternalsBootstrap::GetPluginFileRoot() + "/testfile.json"
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

        TEST_F(ExternalsBootstrapTest, GetPluginFileRootWideReturnsMyDocumentsEuroscopeFolder)
        {
            std::wstring expected = this->GetMyDocumentsPath() + std::wstring(L"/EuroScope/ukcp");
            EXPECT_EQ(expected, ExternalsBootstrap::GetPluginFileRootWide());
        }

        TEST_F(ExternalsBootstrapTest, GetPluginFileRootReturnsMyDocumentsEuroscopeFolder)
        {
            std::string expected = this->GetMyDocumentsPathNarrow() + "/EuroScope/ukcp";
            EXPECT_EQ(expected, ExternalsBootstrap::GetPluginFileRoot());
        }

        TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootThrowsExceptionOnFailedCreatingEuroscopeFolder)
        {
            std::string expected = this->GetMyDocumentsPathNarrow() + "/EuroScope";

            ON_CALL(this->winApiMock, CreateFolder(expected))
                .WillByDefault(Return(false));


            EXPECT_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock),std::runtime_error);
        }

        TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootThrowsExceptionOnFailedCreatingUkcpFolder)
        {
            std::string expected = this->GetMyDocumentsPathNarrow() + "/EuroScope/ukcp";

            ON_CALL(this->winApiMock, CreateFolder(_))
                .WillByDefault(Return(true));

            ON_CALL(this->winApiMock, CreateFolder(expected))
                .WillByDefault(Return(false));

            EXPECT_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock), std::runtime_error);
        }

        TEST_F(ExternalsBootstrapTest, SetupUkcpFolderRootCreatesFolders)
        {
            std::string expected1 = this->GetMyDocumentsPathNarrow() + "/EuroScope";
            std::string expected2 = this->GetMyDocumentsPathNarrow() + "/EuroScope/ukcp";

            EXPECT_CALL(this->winApiMock, CreateFolder(expected1))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(this->winApiMock, CreateFolder(expected2))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_NO_THROW(ExternalsBootstrap::SetupUkcpFolderRoot(this->winApiMock));
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPluginTest
