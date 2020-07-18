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
            MOCK_METHOD3(OpenMessageBox, int(LPCWSTR, LPCWSTR, int));
            MOCK_METHOD1(PlayWave, void(LPCTSTR));
            MOCK_METHOD3(WriteToFile, void(std::wstring, std::string, bool));
            MOCK_METHOD2(ReadFromFileMock, std::string(std::wstring, bool));
            MOCK_METHOD1(FileExists, bool(std::wstring));
            MOCK_METHOD1(CreateFolder, bool(std::wstring folder));
            MOCK_METHOD1(CreateFolderRecursive, bool(std::wstring folder));
            MOCK_METHOD1(CreateLocalFolderRecursive, bool(std::wstring folder));
            MOCK_METHOD1(DeleteGivenFile, bool(std::wstring filename));
            MOCK_CONST_METHOD1(GetFullPathToLocalFile, std::wstring(std::wstring));
            MOCK_METHOD2(SetPermissions, bool(std::wstring, std::filesystem::perms));


            std::string ReadFromFile(std::wstring path, bool relative = true)
            {
                return ReadFromFileMock(path, relative);
            }
        };
    }  // namespace Windows
}  // namespace UKControllerPluginTest
