#pragma once
#include "dialog/DialogProviderInterface.h"
#include "dialog/DialogData.h"
#include "dialog/DialogCallArgument.h"

namespace UKControllerPluginTest {
    namespace Dialog {

        class MockDialogProvider : public UKControllerPlugin::Dialog::DialogProviderInterface
        {
            public:
            MockDialogProvider();
            virtual ~MockDialogProvider();
            MOCK_CONST_METHOD2(
                OpenDialog,
                void(
                    const UKControllerPlugin::Dialog::DialogData&,
                    const UKControllerPlugin::Dialog::DialogCallArgument*));
        };
    } // namespace Dialog
} // namespace UKControllerPluginTest
