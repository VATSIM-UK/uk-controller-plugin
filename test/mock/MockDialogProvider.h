#include "pch/pch.h"
#include "dialog/DialogProviderInterface.h"
#include "dialog/DialogData.h"

namespace UKControllerPluginTest {
    namespace Dialog {
        
        class MockDialogProvider : public UKControllerPlugin::Dialog::DialogProviderInterface
        {
            public:
                MOCK_CONST_METHOD1(OpenDialog, void(const UKControllerPlugin::Dialog::DialogData &));
        };
    }  // namespace Dialog
}  // namespace UKControllerPluginTest
