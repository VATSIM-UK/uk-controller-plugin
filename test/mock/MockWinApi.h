#pragma once
#include "windows/WinApiInterface.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPluginTest {
    namespace Windows {
        class MockWinApi : public UKControllerPlugin::Windows::WinApiInterface
        {
        public:
            MockWinApi() : WinApiInterface(NULL) {}
            MOCK_CONST_METHOD3(
                FileOpenDialog,
                std::wstring(
                    std::wstring title,
                    UINT numFileTypes,
                    const COMDLG_FILTERSPEC * fileTypes
                )
            );
            MOCK_CONST_METHOD3(OpenDialog, bool (int, DLGPROC, LPARAM));
            MOCK_METHOD0(OpenGeneralSettingsDialog, void(void));
            MOCK_METHOD3(OpenMessageBox, int(LPCWSTR, LPCWSTR, int));
            MOCK_METHOD1(PlayWave, void(LPCTSTR));
            MOCK_METHOD3(WriteToFile, void(std::string, std::string, bool));
            MOCK_METHOD2(ReadFromFileMock, std::string(std::string, bool));
            MOCK_METHOD2(ReadFromFileMock, std::string(std::wstring, bool));
            MOCK_METHOD1(FileExists, bool(std::string ));
            MOCK_METHOD1(CreateFolder, bool(std::string folder));
            MOCK_METHOD1(DeleteGivenFile, bool(std::string filename));
            MOCK_CONST_METHOD1(GetFullPathToLocalFile, std::string(std::string));


            // Helper methods because optional parameters
            std::string ReadFromFile(std::string path, bool relative = true)
            {
                return ReadFromFileMock(path, relative);
            }

            std::string ReadFromFile(std::wstring path, bool relative = true)
            {
                return ReadFromFileMock(path, relative);
            }
        };
    }  // namespace Windows
}  // namespace UKControllerPluginTest
