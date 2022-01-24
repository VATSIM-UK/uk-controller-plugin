#pragma once
#include "list/ListItemProviderInterface.h"

namespace UKControllerPlugin::Aircraft {
    class AircraftTypeMapperInterface;
} // namespace UKControllerPlugin::Aircraft

namespace UKControllerPlugin::Wake {
    class WakeSchemeCollection;
    class WakeCalculatorOptions;

    /**
     * Provides the wake schemes for the selection on the
     * calculator display.
     */
    class WakeSchemeProvider : public List::ListItemProviderInterface
    {
        public:
        WakeSchemeProvider(
            std::shared_ptr<WakeCalculatorOptions> options,
            const WakeSchemeCollection& schemes,
            const Aircraft::AircraftTypeMapperInterface& aircraftTypeMapper);
        auto ListColumns() -> int override;
        auto ListName() -> std::string override;
        auto ListItems() -> std::list<std::shared_ptr<List::ListItem>> override;
        void ItemSelected(const std::string& item) override;

        private:
        // Options
        std::shared_ptr<WakeCalculatorOptions> options;

        // For getting the schemes
        const WakeSchemeCollection& schemes;
        
        // Maps aircraft types for wake mapping
        const Aircraft::AircraftTypeMapperInterface& aircraftTypeMapper;
    };
} // namespace UKControllerPlugin::Wake
