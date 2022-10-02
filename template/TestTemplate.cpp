#include "##FOLDERNAME##/##CLASSNAME##.h"

using ::testing::Test;
using UKControllerPlugin::##NAMESPACE##::##CLASSNAME##;

namespace UKControllerPluginTest {
    namespace ##NAMESPACE## {

        class ##CLASSNAME##Test : public Test
        {

        };

        TEST_F(##CLASSNAME##Test, ItDoesSomething)
        {
            EXPECT_TRUE(true);
        }
    }  // namespace ##NAMESPACE##
}  // namespace UKControllerPluginTest
