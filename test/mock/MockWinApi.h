#pragma once
#include "pch/testingutilspch.h"
#include "windows/WinApiInterface.h"

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
            MOCK_METHOD1(OpenWebBrowser, void(std::wstring));
            MOCK_METHOD1(PlayWave, void(LPCTSTR));
            MOCK_METHOD4(WriteToFile, void(std::wstring, std::string, bool, bool));
            MOCK_METHOD4(WriteToFileMock, void(std::wstring, std::string, bool, bool));
            MOCK_METHOD2(ReadFromFileMock, std::string(std::wstring, bool));
            MOCK_METHOD1(FileExists, bool(std::wstring));
            MOCK_METHOD1(CreateFolder, bool(std::wstring folder));
            MOCK_METHOD1(CreateFolderRecursive, bool(std::wstring folder));
            MOCK_METHOD1(CreateLocalFolderRecursive, bool(std::wstring folder));
            MOCK_METHOD1(DeleteGivenFile, bool(std::wstring filename));
            MOCK_CONST_METHOD1(GetFullPathToLocalFile, std::wstring(std::wstring));
            MOCK_METHOD2(SetPermissions, bool(std::wstring, std::filesystem::perms));
            MOCK_CONST_METHOD1(ListAllFilenamesInDirectory, std::set<std::wstring>(std::wstring));
            MOCK_CONST_METHOD1(LoadLibraryRelative, HINSTANCE(std::wstring relativePath));
            MOCK_CONST_METHOD1(UnloadLibrary, void(HINSTANCE handle));
            MOCK_METHOD2(MoveFileToNewLocation, bool(std::wstring, std::wstring));


            std::string ReadFromFile(std::wstring path, bool relative) override
            {
                return ReadFromFileMock(path, relative);
            }
        };
    }  // namespace Windows
}  // namespace UKControllerPluginTest
