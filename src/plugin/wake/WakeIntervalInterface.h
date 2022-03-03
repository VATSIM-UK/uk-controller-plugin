#pragma once

namespace UKControllerPlugin::Wake {
    /**
     * An interface for classes that implement wake intervals
     */
    class WakeIntervalInterface
    {
        public:
        virtual ~WakeIntervalInterface() = default;
        [[nodiscard]] virtual auto Value() const -> double = 0;
        [[nodiscard]] virtual auto Unit() const -> const std::string& = 0;
    };
} // namespace UKControllerPlugin::Wake
