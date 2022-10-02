#include "push/PushEventProxyHandler.h"
#include "push/PushEventSubscription.h"

using testing::NiceMock;
using testing::Test;
using UKControllerPlugin::Push::PushEventProxyHandler;
using UKControllerPlugin::Push::PushEventSubscription;

namespace UKControllerPluginTest {
    namespace Push {

        class PushEventProxyHandlerTest : public Test
        {
            public:
            PushEventProxyHandler handler;
        };

        TEST_F(PushEventProxyHandlerTest, ItReturnsSubscriptions)
        {
            std::set<PushEventSubscription> expected = {{PushEventSubscription::SUB_TYPE_ALL}};

            EXPECT_EQ(expected, this->handler.GetPushEventSubscriptions());
        }
    } // namespace Push
} // namespace UKControllerPluginTest
