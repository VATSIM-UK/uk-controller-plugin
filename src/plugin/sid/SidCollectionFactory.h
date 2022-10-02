#pragma once

namespace UKControllerPlugin {
    namespace Sid {
        class SidCollection;

        std::unique_ptr<SidCollection> MakeSidCollection(const nlohmann::json& sidData);
        bool SidDataValid(const nlohmann::json& sidData);
        bool InitialHeadingValid(const nlohmann::json& sidData);
    } // namespace Sid
} // namespace UKControllerPlugin
