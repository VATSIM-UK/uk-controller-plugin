#pragma once

namespace UKControllerPlugin::IntentionCode {

    class FirExitPoint;

    /**
     * All the FIR exit points in one handy place
     */
    class FirExitPointCollection
    {
        public:
        void Add(const std::shared_ptr<FirExitPoint>& point);
        [[nodiscard]] auto CountPoints() const -> size_t;
        [[nodiscard]] auto PointById(int id) const -> std::shared_ptr<FirExitPoint>;
        [[nodiscard]] auto PointByIdentifier(const std::string& identifier) const -> std::shared_ptr<FirExitPoint>;

        private:
        // All the points, indexed by identifier
        std::map<std::string, std::shared_ptr<FirExitPoint>> points;
    };

} // namespace UKControllerPlugin::IntentionCode
