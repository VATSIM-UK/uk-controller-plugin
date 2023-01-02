#include "BootstrapProviderCollection.h"
#include "BootstrapProviderCollectionFactory.h"
#include "approach/ApproachBootstrapProvider.h"
#include "intention/IntentionCodeBootstrapProvider.h"

namespace UKControllerPlugin::Bootstrap {
    auto Make() -> std::unique_ptr<BootstrapProviderCollection>
    {
        auto collection = std::make_unique<BootstrapProviderCollection>();
        collection->AddProvider(std::make_unique<Approach::ApproachBootstrapProvider>());
        collection->AddProvider(std::make_unique<IntentionCode::IntentionCodeBootstrapProvider>());

        return collection;
    }
} // namespace UKControllerPlugin::Bootstrap
