#include "airfield/AirfieldCollection.h"
#include "approach/ApproachModuleFactory.h"
#include "approach/ApproachSequencer.h"
#include "bootstrap/PersistenceContainer.h"
#include "wake/WakeCategoryMapperCollection.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Approach::ApproachModuleFactory;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Wake::WakeCategoryMapperCollection;

namespace UKControllerPluginTest::Approach {
    class ApproachModuleFactoryTest : public testing::Test
    {
        public:
        ApproachModuleFactoryTest()
        {
            container.plugin = std::make_unique<testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface>>();
            container.wakeCategoryMappers = std::make_unique<WakeCategoryMapperCollection>();
            container.airfields = std::make_unique<AirfieldCollection>();
        }

        PersistenceContainer container;
        ApproachModuleFactory factory;
    };

    TEST_F(ApproachModuleFactoryTest, ItMakesASequencer)
    {
        EXPECT_NE(nullptr, &factory.Sequencer());
    }

    TEST_F(ApproachModuleFactoryTest, ItOnlyHasOneSequencer)
    {
        EXPECT_EQ(&factory.Sequencer(), &factory.Sequencer());
    }

    TEST_F(ApproachModuleFactoryTest, ItMakesASequencerOptions)
    {
        EXPECT_NE(nullptr, &factory.SequencerOptions());
    }

    TEST_F(ApproachModuleFactoryTest, ItOnlyHasOneSequencerOptions)
    {
        EXPECT_EQ(&factory.SequencerOptions(), &factory.SequencerOptions());
    }

    TEST_F(ApproachModuleFactoryTest, ItMakesACalculator)
    {
        EXPECT_NE(nullptr, &factory.SpacingCalculator(container));
    }

    TEST_F(ApproachModuleFactoryTest, ItOnlyHasOneCalculator)
    {
        EXPECT_EQ(&factory.SpacingCalculator(container), &factory.SpacingCalculator(container));
    }

    TEST_F(ApproachModuleFactoryTest, ItAssertsIfContainerPluginNotSet)
    {
        container.plugin = nullptr;
        EXPECT_DEATH(static_cast<void>(factory.SpacingCalculator(container)), "Plugin is not set on container");
    }

    TEST_F(ApproachModuleFactoryTest, ItAssertsIfContainerAirfieldsNotSet)
    {
        container.airfields = nullptr;
        EXPECT_DEATH(
            static_cast<void>(factory.SpacingCalculator(container)), "Airfield collection is not set on container");
    }

    TEST_F(ApproachModuleFactoryTest, ItAssertsIfContainerWakeMappersNotSet)
    {
        container.wakeCategoryMappers = nullptr;
        EXPECT_DEATH(
            static_cast<void>(factory.SpacingCalculator(container)), "Wake category mappers is not set on container");
    }
} // namespace UKControllerPluginTest::Approach
