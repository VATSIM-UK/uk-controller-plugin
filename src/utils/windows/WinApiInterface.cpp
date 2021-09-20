#include "WinApiInterface.h"

namespace UKControllerPlugin::Windows {
    WinApiInterface::WinApiInterface(HINSTANCE dllInstance) : dllInstance(dllInstance){};
    WinApiInterface::~WinApiInterface() = default;
    WinApiInterface::WinApiInterface(const WinApiInterface&) = default;

    auto WinApiInterface::GetDllInstance() const -> HINSTANCE
    {
        return this->dllInstance;
    }
} // namespace UKControllerPlugin::Windows
