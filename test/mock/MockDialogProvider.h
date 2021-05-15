#pragma once
#include "pch/testingutilspch.h"
#include "dialog/DialogProviderInterface.h"
#include "dialog/DialogData.h"
#include "dialog/DialogCallArgument.h"

namespace UKControllerPluginTest {
    namespace Dialog {

        class MockDialogProvider : public UKControllerPlugin::Dialog::DialogProviderInterface
        {
            public:

                MOCK_CONST_METHOD2(
                    OpenDialog,
                    void(
                        const UKControllerPlugin::Dialog::DialogData &,
                        const UKControllerPlugin::Dialog::DialogCallArgument *
                    )
                );

        };
    }  // namespace Dialog
}  // namespace UKControllerPluginTest
