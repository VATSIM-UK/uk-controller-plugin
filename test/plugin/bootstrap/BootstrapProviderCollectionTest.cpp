#include "bootstrap/BootstrapProviderCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/MenuToggleableDisplayFactory.h"
#include "radarscreen/RadarRenderableCollection.h"

using UKControllerPlugin::Bootstrap::BootstrapProviderCollection;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::MenuToggleableDisplayFactory;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;

namespace UKControllerPluginTest::Bootstrap {
    class BootstrapProviderCollectionTest : public testing::Test
    {
        public:
        BootstrapProviderCollectionTest()
            : provider(std::make_shared<testing::NiceMock<MockBootstrapProvider>>()),
              provider2(std::make_shared<testing::NiceMock<MockBootstrapProvider>>()),
              displayFactory(functions, configurables)
        {
        }

        std::shared_ptr<testing::NiceMock<MockBootstrapProvider>> provider;
        std::shared_ptr<testing::NiceMock<MockBootstrapProvider>> provider2;
        BootstrapProviderCollection collection;
        PersistenceContainer container;
        RadarRenderableCollection radarRenderables;
        ConfigurableDisplayCollection configurables;
        FunctionCallEventHandler functions;
        MenuToggleableDisplayFactory displayFactory;
        AsrEventHandlerCollection asrHandler;
    };

    TEST_F(BootstrapProviderCollectionTest, ItStartsWithNoProviders)
    {
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(BootstrapProviderCollectionTest, ItAddsAProvider)
    {
        collection.AddProvider(provider);
        collection.AddProvider(provider2);
        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(BootstrapProviderCollectionTest, ItDoesntAddDuplicateProviders)
    {
        collection.AddProvider(provider);
        collection.AddProvider(provider);
        collection.AddProvider(provider);
        collection.AddProvider(provider2);
        collection.AddProvider(provider2);
        collection.AddProvider(provider2);
        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(BootstrapProviderCollectionTest, ItDoesntHaveAProviderByType)
    {
        EXPECT_FALSE(collection.HasProvider([](const BootstrapProviderInterface& provider) -> bool {
            try {
                static_cast<void>(dynamic_cast<const MockBootstrapProvider&>(provider));
                return true;
            } catch (std::bad_cast&) {
                return false;
            }
        }));
    }

    TEST_F(BootstrapProviderCollectionTest, ItDoesHaveAProviderByType)
    {
        collection.AddProvider(provider);
        EXPECT_TRUE(collection.HasProvider([](const BootstrapProviderInterface& provider) -> bool {
            try {
                static_cast<void>(dynamic_cast<const MockBootstrapProvider&>(provider));
                return true;
            } catch (std::bad_cast&) {
                return false;
            }
        }));
    }

    TEST_F(BootstrapProviderCollectionTest, ItBootstrapsForThePlugin)
    {
        collection.AddProvider(provider);
        collection.AddProvider(provider2);

        EXPECT_CALL(*provider, BootstrapPlugin(testing::Ref(container))).Times(1);

        EXPECT_CALL(*provider2, BootstrapPlugin(testing::Ref(container))).Times(1);

        collection.BootstrapPlugin(container);
    }

    TEST_F(BootstrapProviderCollectionTest, ItBootstrapsForRadarScreens)
    {
        collection.AddProvider(provider);
        collection.AddProvider(provider2);

        EXPECT_CALL(
            *provider,
            BootstrapRadarScreen(
                testing::Ref(container),
                testing::Ref(radarRenderables),
                testing::Ref(configurables),
                testing::Ref(asrHandler),
                testing::Ref(displayFactory)))
            .Times(1);

        EXPECT_CALL(
            *provider2,
            BootstrapRadarScreen(
                testing::Ref(container),
                testing::Ref(radarRenderables),
                testing::Ref(configurables),
                testing::Ref(asrHandler),
                testing::Ref(displayFactory)))
            .Times(1);

        collection.BootstrapRadarScreen(container, radarRenderables, configurables, asrHandler, displayFactory);
    }
} // namespace UKControllerPluginTest::Bootstrap
