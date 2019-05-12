#include "pch/pch.h"
#include "bootstrap/ExternalsBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "curl/CurlRequest.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Bootstrap::ExternalsBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPluginTest {
    namespace Bootstrap {

        class ExternalsBootstrapTest : public ::testing::Test {

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
            TCHAR myDocumentsPath[MAX_PATH];
            HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, myDocumentsPath);
            std::wstring expected = myDocumentsPath + std::wstring(L"/EuroScope/ukcp");
            std::replace(expected.begin(), expected.end(), L'\\', L'/');

            EXPECT_EQ(expected, ExternalsBootstrap::GetPluginFileRootWide());
        }

        TEST_F(ExternalsBootstrapTest, GetPluginFileRootReturnsMyDocumentsEuroscopeFolder)
        {
            TCHAR myDocumentsPath[MAX_PATH];
            HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, myDocumentsPath);
            std::wstring widePath(myDocumentsPath);
            std::replace(widePath.begin(), widePath.end(), L'\\', L'/');

            std::string expected = HelperFunctions::ConvertToRegularString(widePath) + "/EuroScope/ukcp";

            EXPECT_EQ(expected, ExternalsBootstrap::GetPluginFileRoot());
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPluginTest
