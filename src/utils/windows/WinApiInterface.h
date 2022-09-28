#pragma once

namespace UKControllerPlugin::Windows {
    /*
        Interface between the plugin and the Windows API.
    */
    class WinApiInterface
    {
        public:
        explicit WinApiInterface(HINSTANCE dllInstance);
        virtual ~WinApiInterface();
        WinApiInterface(const WinApiInterface&);
        WinApiInterface(const WinApiInterface&&) = delete;
        [[nodiscard]] auto operator=(const WinApiInterface&) -> WinApiInterface& = delete;
        [[nodiscard]] auto operator=(const WinApiInterface&&) -> WinApiInterface& = delete;
        virtual auto CreateFolder(std::wstring folder) -> bool = 0;
        virtual auto CreateFolderRecursive(std::wstring folder) -> bool = 0;
        virtual auto CreateLocalFolderRecursive(std::wstring folder) -> bool = 0;
        virtual auto DeleteGivenFile(std::wstring filename) -> bool = 0;
        virtual auto FileExists(std::wstring filename) -> bool = 0;
        virtual auto FileOpenDialog(std::wstring title, UINT numFileTypes, const COMDLG_FILTERSPEC* fileTypes) const
            -> std::wstring = 0;
        [[nodiscard]] auto GetDllInstance() const -> HINSTANCE;
        [[nodiscard]] virtual auto GetFullPathToLocalFile(std::wstring relativePath) const -> std::wstring = 0;
        [[nodiscard]] virtual auto ListAllFilenamesInDirectory(std::wstring relativePath) const
            -> std::set<std::wstring> = 0;
        [[nodiscard]] virtual auto LoadLibraryRelative(std::wstring relativePath) const -> HINSTANCE = 0;
        virtual auto GetFunctionPointerFromLibrary(HINSTANCE libraryHandle, std::string functionName) const
            -> FARPROC = 0;
        virtual auto MoveFileToNewLocation(std::wstring oldName, std::wstring newName) -> bool = 0;
        virtual void UnloadLibrary(HINSTANCE handle) const = 0;
        virtual auto OpenMessageBox(LPCWSTR message, LPCWSTR title, int options) -> int = 0;
        virtual void OpenWebBrowser(std::wstring url) = 0;
        virtual void PlayWave(LPCTSTR sound) = 0;
        virtual auto ReadFromFile(std::wstring filename, bool relativePath = true) -> std::string = 0;
        virtual auto SetPermissions(std::wstring fileOrFolder, std::filesystem::perms permissions) -> bool = 0;
        virtual void WriteToFile(std::wstring filename, std::string data, bool truncate, bool binary) = 0;
        virtual void OpenExplorer(const std::wstring& location) const = 0;

        private:
        // DLL Instance for the plugin
        const HINSTANCE dllInstance;
    };
} // namespace UKControllerPlugin::Windows
