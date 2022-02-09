#include "CallsignSelectionList.h"
#include "CallsignSelectionProviderInterface.h"
#include "list/ListItem.h"

namespace UKControllerPlugin::Aircraft {

    CallsignSelectionList::CallsignSelectionList(std::shared_ptr<CallsignSelectionProviderInterface> callsignProvider)
        : callsignProvider(std::move(callsignProvider))
    {
    }

    int CallsignSelectionList::ListColumns()
    {
        return 1;
    }

    std::string CallsignSelectionList::ListName()
    {
        return "Select Aircraft";
    }

    std::list<std::shared_ptr<List::ListItem>> CallsignSelectionList::ListItems()
    {
        std::list<std::shared_ptr<List::ListItem>> items;
        for (const auto& callsign : this->callsignProvider->GetCallsigns()) {
            items.push_back(
                std::make_shared<List::ListItem>(callsign, "", false, false, List::ListItemCheckedStatus::NoCheckbox));
        }
        return items;
    }

    void CallsignSelectionList::ItemSelected(const std::string& item)
    {
        this->callsignProvider->CallsignSelected(item);
    }
} // namespace UKControllerPlugin::Aircraft
