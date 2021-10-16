#include "SelcalModule.h"
#include "SelcalParser.h"
#include "SelcalTagItem.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"

namespace UKControllerPlugin::Selcal {
    const int SELCAL_TAG_ITEM_ID = 128;
    const int SELCAL_SEPARATOR_ITEM_ID = 129;

    void BootstrapPlugin(const Bootstrap::PersistenceContainer& container)
    {
        auto tagItem = std::make_shared<SelcalTagItem>(std::make_shared<SelcalParser>());
        container.tagHandler->RegisterTagItem(SELCAL_TAG_ITEM_ID, tagItem);
        container.tagHandler->RegisterTagItem(SELCAL_SEPARATOR_ITEM_ID, tagItem);
        container.flightplanHandler->RegisterHandler(tagItem);
    }
} // namespace UKControllerPlugin::Selcal
