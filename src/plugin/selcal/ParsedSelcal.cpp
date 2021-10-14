#include "ParsedSelcal.h"

namespace UKControllerPlugin::Selcal {

    ParsedSelcal::ParsedSelcal(std::string code) : code(std::move(code))
    {
    }

    auto ParsedSelcal::GetRaw() const -> const std::string&
    {
        return this->code;
    }

    auto ParsedSelcal::GetWithSeparator() const -> std::string
    {
        return code.substr(0, 2) + "-" + code.substr(2);
    }
} // namespace UKControllerPlugin::Selcal
