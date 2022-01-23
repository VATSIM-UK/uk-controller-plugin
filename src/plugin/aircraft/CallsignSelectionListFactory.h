#pragma once

namespace UKControllerPlugin::List {
    class PopupList;
    class PopupListFactory;
} // namespace UKControllerPlugin::List

namespace UKControllerPlugin::Aircraft {
    class CallsignSelectionProviderInterface;
    class CallsignSelectionList;

    class CallsignSelectionListFactory
    {
        public:
        CallsignSelectionListFactory(List::PopupListFactory& listFactory);
        [[nodiscard]] auto
        Create(std::shared_ptr<CallsignSelectionProviderInterface> provider, const std::string& description) const
            -> std::shared_ptr<List::PopupList>;

        private:
        // For creating lists
        List::PopupListFactory& listFactory;
    };
} // namespace UKControllerPlugin::Aircraft
