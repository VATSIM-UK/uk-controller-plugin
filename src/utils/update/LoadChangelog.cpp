#include "update/LoadChangelog.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin {
    namespace Update {

        const std::wstring changelogUrl =
            L"https://vatsim-uk.github.io/uk-controller-plugin/UserGuide/Changelog/Changelog.html";

        void LoadChangelog(Windows::WinApiInterface& windows)
        {
            windows.OpenWebBrowser(changelogUrl);
        }
    } // namespace Update
} // namespace UKControllerPlugin
