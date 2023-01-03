#include "MockWinApi.h"

namespace UKControllerPluginTest::Windows {
    MockWinApi::MockWinApi() : WinApiInterface(NULL)
    {
    }

    MockWinApi::~MockWinApi() = default;

    std::string MockWinApi::ReadFromFile(std::wstring path, bool relative)
    {
        return ReadFromFileMock(path, relative);
    }
} // namespace UKControllerPluginTest::Windows
