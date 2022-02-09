#include "StringFormatManager.h"

namespace UKControllerPlugin::Graphics {

    StringFormatManager::StringFormatManager()
    {
    }

    auto StringFormatManager::Instance() -> StringFormatManager&
    {
        static std::unique_ptr<StringFormatManager> instance;
        if (instance == nullptr) {
            instance = std::unique_ptr<StringFormatManager>(new StringFormatManager);
        }

        return *instance;
    }

    auto StringFormatManager::GetAligned(int id, Gdiplus::StringAlignment textAlign) -> const Gdiplus::StringFormat&
    {
        if (this->formats.count(id) == 0) {
            this->formats[id] =
                std::make_shared<Gdiplus::StringFormat>(Gdiplus::StringFormatFlags::StringFormatFlagsNoClip);
            this->formats.at(id)->SetAlignment(textAlign);
            this->formats.at(id)->SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
        }

        return *this->formats.at(id);
    }

    auto StringFormatManager::GetCentreAlign() -> const Gdiplus::StringFormat&
    {
        return this->GetAligned(1, Gdiplus::StringAlignment::StringAlignmentCenter);
    }

    auto StringFormatManager::GetLeftAlign() -> const Gdiplus::StringFormat&
    {
        return this->GetAligned(2, Gdiplus::StringAlignment::StringAlignmentNear);
    }
} // namespace UKControllerPlugin::Graphics
