#include "pch/pch.h"
#include "push/PushEventProxyHandler.h"
#include "push/PushEventSubscription.h"

using UKControllerPlugin::Push::PushEventProxyHandler;
using UKControllerPlugin::Push::PushEventSubscription;
using testing::Test;
using testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Push {

        class PushEventProxyHandlerTest : public Test
        {
            public:
                PushEventProxyHandler handler;
        };

        TEST_F(PushEventProxyHandlerTest, ItReturnsSubscriptions)
        {
            std::set<PushEventSubscription> expected = {
                {
                    PushEventSubscription::SUB_TYPE_ALL
                }
            };

            EXPECT_EQ(expected, this->handler.GetSubscriptions());
        }
    } // namespace Push
}  // namespace UKControllerPluginTest
