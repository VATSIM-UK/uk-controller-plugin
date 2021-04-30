#pragma once

namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows

    namespace Update {

        extern const std::wstring changelogUrl;

        void LoadChangelog(Windows::WinApiInterface& windows);
    } // namespace Update
} // namespace UKControllerPlugin
