#include "CallsignSelectionList.h"
#include "CallsignSelectionListFactory.h"
#include "CallsignSelectionProviderInterface.h"
#include "list/PopupListFactory.h"

namespace UKControllerPlugin::Aircraft {

    CallsignSelectionListFactory::CallsignSelectionListFactory(List::PopupListFactory& listFactory)
        : listFactory(listFactory)
    {
    }

    auto CallsignSelectionListFactory::Create(
        std::shared_ptr<CallsignSelectionProviderInterface> provider, const std::string& description) const
        -> std::shared_ptr<List::PopupListInterface>
    {
        return listFactory.Create(std::make_shared<CallsignSelectionList>(provider), description);
    }
} // namespace UKControllerPlugin::Aircraft
