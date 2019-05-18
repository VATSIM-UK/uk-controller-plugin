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
            std::string filesDirectory,
            std::wstring filesDirectoryW
        )
            : WinApiInterface(dllInstance), filesDirectory(filesDirectory), filesDirectoryW(filesDirectoryW)
        {
            this->dllInstance = dllInstance;
        }

        /*
            Creates a folder if it doesn't already exist.
        */
        bool WinApi::CreateFolder(std::string folder)
        {
            std::wstring folderWide(folder.length(), L' ');
            std::copy(folder.begin(), folder.end(), folderWide.begin());
            return (CreateDirectory(folderWide.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) ? true : false;
        }

        /*
            Create a folder from the root, recursively
        */
        bool WinApi::CreateLocalFolderRecursive(std::string folder)
        {
            std::vector<std::string> tokens = HelperFunctions::TokeniseString(
                '/',
                this->GetFullPathToLocalFile(folder)
            );
            std::string currentPath;
            for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
                if (!this->CreateFolder(currentPath + *it)) {
                    return false;
                }
                currentPath += *it + "/";
            }

            return true;
        }

        /*
            Deletes a file from the filesystem.
        */
        bool WinApi::DeleteGivenFile(std::string filename)
        {
            std::wstring fileWide(filename.length(), L' ');
            std::copy(filename.begin(), filename.end(), fileWide.begin());

            return (DeleteFile(fileWide.c_str()) == TRUE) ? true : false;
        }

        /*
            Returns true if a file exists, false otherwise.
        */
        bool WinApi::FileExists(std::string filename)
        {
            std::string newFilename = this->GetFullPathToLocalFile(filename);
            std::wstring filenameWide(newFilename.length(), L' ');
            std::copy(newFilename.begin(), newFilename.end(), filenameWide.begin());

            WIN32_FIND_DATA findData;
            HANDLE hFind = FindFirstFile(filenameWide.c_str(), &findData);

            if (hFind == INVALID_HANDLE_VALUE) {
                return false;
            }

            FindClose(hFind);
            return true;
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
        std::string WinApi::GetFullPathToLocalFile(std::string relativePath) const
        {
            return this->filesDirectory + "/" + relativePath;
        }

        /*
            Gets the full path to a given file when that file is in WString
        */
        std::wstring WinApi::GetFullPathToLocalFile(std::wstring relativePath) const
        {
            return this->filesDirectoryW + L"/" + relativePath;
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
        void WinApi::WriteToFile(std::string filename, std::string data, bool truncate)
        {
            std::string newFilename = this->GetFullPathToLocalFile(filename);
            this->CreateMissingDirectories(newFilename);
            std::ofstream file(
                newFilename,
                std::ofstream::out | ((truncate) ? std::ofstream::trunc : std::ofstream::app)
            );
            file.exceptions(std::ofstream::badbit);
            if (file.is_open()) {
                file << data;
                file.close();
            }
        }

        /*
            Creates the directories needed for a given file.
        */
        void WinApi::CreateMissingDirectories(std::string endFile)
        {
            std::vector<std::string> tokens = HelperFunctions::TokeniseString('/', endFile);
            std::string currentPath;
            for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
                // We don't want to include the last item, as that's the file.
                if (*it == tokens.back()) {
                    break;
                }

                this->CreateFolder(currentPath + *it);
                currentPath += *it + "/";
            }
        }

        /*
            Return the entire contents of a file as a string.
        */
        std::string WinApi::ReadFromFile(std::string filename, bool relativePath)
        {
            return this->ReadFileContents(
                std::ifstream(
                    relativePath ? this->GetFullPathToLocalFile(filename) : filename,
                    std::ifstream::in
                )
            );
        }

        /*
            Return the entire contents of a file as a string - except the filename is widechar
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
