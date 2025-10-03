#include "BootstrapProviderCollection.h"
#include "BootstrapProviderCollectionFactory.h"
#include "approach/ApproachBootstrapProvider.h"
#ifdef ENABLE_ECFMP
#include "ecfmp/ECFMPBootstrapProvider.h"
#endif
#include "intention/IntentionCodeBootstrapProvider.h"

namespace UKControllerPlugin::Bootstrap {
    auto Make() -> std::unique_ptr<BootstrapProviderCollection>
    {
        auto collection = std::make_unique<BootstrapProviderCollection>();
        collection->AddProvider(std::make_unique<Approach::ApproachBootstrapProvider>());
#ifdef ENABLE_ECFMP
        collection->AddProvider(std::make_unique<ECFMP::ECFMPBootstrapProvider>());
#endif
        collection->AddProvider(std::make_unique<IntentionCode::IntentionCodeBootstrapProvider>());

        return collection;
    }
} // namespace UKControllerPlugin::Bootstrap
