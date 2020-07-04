#include "pch/stdafx.h"
#include "windows/WinApi.h"
#include "historytrail/HistoryTrailDialog.h"
#include "helper/HelperFunctions.h"
#include "euroscope/GeneralSettingsDialog.h"
#include "euroscope/UserSetting.h"
#include "historytrail/HistoryTrailData.h"
#include "hold/HoldConfigurationDialog.h"

using UKControllerPlugin::HistoryTrail::HistoryTrailDialog;
using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::GeneralSettingsDialog;
using UKControllerPlugin::HistoryTrail::HistoryTrailData;
using UKControllerPlugin::Euroscope::GeneralSettingsDialog;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogCallArgument;

namespace UKControllerPlugin {
    namespace Windows {

        WinApi::WinApi(
            HINSTANCE dllInstance,
            std::wstring filesDirectory
        )
            : WinApiInterface(dllInstance), filesDirectory(filesDirectory)
        {
            this->dllInstance = dllInstance;
        }

        /*
            Creates a folder if it doesn't already exist.
        */
        bool WinApi::CreateFolder(std::wstring folder)
        {
            try {
                std::filesystem::create_directory(folder);
                return true;
            } catch (std::filesystem::filesystem_error e) {
                std::wstring message = L"Unable to create folder: "
                    + HelperFunctions::ConvertToWideString(e.what());
                this->OpenMessageBox(
                    message.c_str(),
                    L"UKCP Filesystem Error",
                    MB_ICONWARNING | MB_OK
                );
                return false;
            }
        }

        /*
            Creates a folder and all those before it.
        */
        bool WinApi::CreateFolderRecursive(std::wstring folder)
        {
            try {
                std::filesystem::create_directories(folder);
                return true;
            } catch (std::filesystem::filesystem_error e) {
                std::wstring message = L"Unable to create folder recursively: "
                    + HelperFunctions::ConvertToWideString(e.what());
                this->OpenMessageBox(
                    message.c_str(),
                    L"UKCP Filesystem Error",
                    MB_ICONWARNING | MB_OK
                );
                return false;
            }
        }

        /*
            Create a folder from the UK file root, recursively.
        */
        bool WinApi::CreateLocalFolderRecursive(std::wstring folder)
        {
            try {
                std::filesystem::create_directories(this->filesDirectory + L"/" + folder);
                return true;
            } catch (std::filesystem::filesystem_error e) {
                std::wstring message = L"Unable to create local folder recursively: "
                    + HelperFunctions::ConvertToWideString(e.what());
                this->OpenMessageBox(
                    message.c_str(),
                    L"UKCP Filesystem Error",
                    MB_ICONWARNING | MB_OK
                );
                return false;
            }
        }

        /*
            Deletes a file from the filesystem.
        */
        bool WinApi::DeleteGivenFile(std::wstring filename)
        {
            std::wstring fileWide(filename.length(), L' ');
            std::copy(filename.begin(), filename.end(), fileWide.begin());

            return (DeleteFile(fileWide.c_str()) == TRUE) ? true : false;
        }

        /*
            Returns true if a file exists, false otherwise.
        */
        bool WinApi::FileExists(std::wstring filename)
        {
            try {
                return std::filesystem::exists(this->GetFullPathToLocalFile(filename));
            } catch (std::filesystem::filesystem_error e) {
                std::wstring message = L"Unable to check if file exists: "
                    + HelperFunctions::ConvertToWideString(e.what());
                this->OpenMessageBox(
                    message.c_str(),
                    L"UKCP Filesystem Error",
                    MB_ICONWARNING | MB_OK
                );
                return false;
            }
        }

        /*
            Starts the File Open Dialog
        */
        std::wstring WinApi::FileOpenDialog(
            std::wstring title,
            UINT numFileTypes,
            const COMDLG_FILTERSPEC * fileTypes
        ) const
        {
            std::wstringstream result;
            HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
            if (SUCCEEDED(hr))
            {
                IFileOpenDialog *pFileOpen;

                // Create the FileOpenDialog object.
                hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

                if (SUCCEEDED(hr))
                {
                    // Show the Open dialog box.
                    pFileOpen->SetTitle(title.c_str());
                    pFileOpen->SetFileTypes(numFileTypes, fileTypes);
                    hr = pFileOpen->Show(NULL);

                    // Get the file name from the dialog box.
                    if (SUCCEEDED(hr))
                    {
                        IShellItem *pItem;
                        hr = pFileOpen->GetResult(&pItem);
                        if (SUCCEEDED(hr))
                        {
                            PWSTR pszFilePath;
                            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                            // Display the file name to the user.
                            if (SUCCEEDED(hr))
                            {
                                result << pszFilePath;
                                CoTaskMemFree(pszFilePath);
                            }
                            pItem->Release();
                        }
                    }
                    pFileOpen->Release();
                }
                CoUninitialize();
            }

            return result.str();
        }

        /*
            Gets the full path to a given file
        */
        std::wstring WinApi::GetFullPathToLocalFile(std::wstring relativePath) const
        {
            return this->filesDirectory + L"/" + relativePath;
        }

        /*
            Opens a Windows message box.
        */
        int WinApi::OpenMessageBox(LPCWSTR message, LPCWSTR title, int options)
        {
            return MessageBox(GetActiveWindow(), message, title, options);
        }

        /*
            Plays a wave sound file that is stored in the DLL resources.
        */
        void WinApi::PlayWave(LPCTSTR sound)
        {
            PlaySound(sound, this->dllInstance, SND_ASYNC | SND_RESOURCE);
        }

        /*
            Write a given string into a file.
        */
        void WinApi::WriteToFile(std::wstring filename, std::string data, bool truncate)
        {
            std::wstring newFilename = this->GetFullPathToLocalFile(filename);
            this->CreateMissingDirectories(newFilename);
            std::ofstream file(
                newFilename,
                std::ofstream::out | ((truncate) ? std::ofstream::trunc : std::ofstream::app)
            );
            file.exceptions(std::ofstream::badbit);
            if (file.is_open()) {
                file << data;
                file.close();
            } else {
                std::wstring message = L"File not opened for writing: " + filename;
                this->OpenMessageBox(
                    message.c_str(),
                    L"UKCP Filesystem Error",
                    MB_ICONWARNING | MB_OK
                );
            }
        }

        /*
            Creates the directories needed for a given file.
        */
        void WinApi::CreateMissingDirectories(std::wstring endFile)
        {
            try {
                std::filesystem::create_directories(endFile.substr(0, endFile.find_last_of('/')));
            } catch (std::filesystem::filesystem_error e) {
                std::wstring message = L"Error when creating directories "
                    + HelperFunctions::ConvertToWideString(e.what());
                this->OpenMessageBox(
                    message.c_str(),
                    L"UKCP Filesystem Error",
                    MB_ICONWARNING | MB_OK
                );
            }
        }

        /*
            Return the entire contents of a file as a string.
        */
        std::string WinApi::ReadFromFile(std::wstring filename, bool relativePath)
        {
            return this->ReadFileContents(
                std::ifstream(
                    relativePath ? this->GetFullPathToLocalFile(filename) : filename,
                    std::ifstream::in
                )
            );
        }

        /*
            Read from the given path, but here we don't care
            whether the string is wide or not.
        */
        std::string WinApi::ReadFileContents(std::ifstream file)
        {
            file.exceptions(std::ifstream::badbit);
            if (file.is_open()) {
                std::string data;
                data.assign((std::istreambuf_iterator<char>(file)),
                    (std::istreambuf_iterator<char>()));
                file.close();
                return data;
            } else {
                std::wstring message = L"File not opened for reading";
                this->OpenMessageBox(
                    message.c_str(),
                    L"UKCP Filesystem Error",
                    MB_ICONWARNING | MB_OK
                );
            }

            return "";
        }

        /*
            Open a dialog
        */
        void WinApi::OpenDialog(const DialogData & dialog, const DialogCallArgument * argument) const
        {
            DialogBoxParam(
                this->dllInstance,
                MAKEINTRESOURCE(dialog.dialogId),
                GetActiveWindow(),
                dialog.dialogProcedure,
                reinterpret_cast<LPARAM>(argument)
            );
        }
    }  // namespace Windows
}  // namespace UKControllerPlugin
