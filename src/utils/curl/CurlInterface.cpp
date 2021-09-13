#include "CurlInterface.h"

namespace UKControllerPlugin::Curl {
    CurlInterface::CurlInterface() = default;
    CurlInterface::~CurlInterface() = default;
    CurlInterface::CurlInterface(const CurlInterface&) = default;
    [[nodiscard]] auto CurlInterface::operator=(const CurlInterface&) -> CurlInterface& = default;
} // namespace UKControllerPlugin::Curl
