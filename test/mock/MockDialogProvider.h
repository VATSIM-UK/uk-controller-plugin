#include "pch/pch.h"
#include "dialog/DialogProviderInterface.h"
#include "dialog/DialogData.h"

namespace UKControllerPluginTest {
    namespace Dialog {
        
        template <typename TD, typename TC>
        class MockDialogProvider : public UKControllerPlugin::Dialog::DialogProviderInterface
        {
            public:
                MockDialogProvider(
                    const UKControllerPlugin::Dialog::DialogData expectedData,
                    const TD expectedDialogArgument,
                    const TC expectedContextArgument
                ) : expectedData(expectedData), expectedDialogArgument(expectedDialogArgument),
                    expectedContextArgument(expectedContextArgument)
                {
                }

                void OpenDialog(
                    const UKControllerPlugin::Dialog::DialogData & dialog,
                    const UKControllerPlugin::Dialog::DialogCallArgument * args
                ) const {
                    this->callCount++;
                    EXPECT_TRUE(this->expectedData == dialog);
                    EXPECT_TRUE(this->expectedDialogArgument == *reinterpret_cast<TD *>(args->dialogArgument));
                    EXPECT_TRUE(this->expectedContextArgument == *reinterpret_cast<TC *>(args->contextArgument));
                }

                mutable unsigned int callCount = 0;

                // The expected dialog data
                const UKControllerPlugin::Dialog::DialogData expectedData;

                // The expected dialog argument
                const TD expectedDialogArgument;

                // The expected context argument
                const TC expectedContextArgument;
        };
    }  // namespace Dialog
}  // namespace UKControllerPluginTest
