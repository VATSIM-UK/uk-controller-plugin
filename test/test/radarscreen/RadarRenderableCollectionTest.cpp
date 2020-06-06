#include "pch/pch.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "mock/MockRadarRendererableInterface.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "helper/Matchers.h"
#include "mock/MockGraphicsInterface.h"

using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPluginTest::RadarScreen::MockRadarRenderableInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPluginTest::Windows::MockGraphicsInterface;
using ::testing::Ref;
using ::testing::Return;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace RadarScreen {

        TEST(RadarRenderableCollection, ReserveRendererIdentifierReturnsOneByDefault)
        {
            RadarRenderableCollection collection;
            EXPECT_EQ(1, collection.ReserveRendererIdentifier());
        }

        TEST(RadarRenderableCollection, ReserveRendererIdentifierPreventDuplicateIds)
        {
            RadarRenderableCollection collection;
            EXPECT_EQ(1, collection.ReserveRendererIdentifier());
            EXPECT_EQ(2, collection.ReserveRendererIdentifier());
            EXPECT_EQ(3, collection.ReserveRendererIdentifier());
            EXPECT_EQ(4, collection.ReserveRendererIdentifier());
            EXPECT_EQ(5, collection.ReserveRendererIdentifier());
        }

        TEST(RadarRenderableCollection, ReserveRendererIdentifierSkipsIdIfAlreadyTaken)
        {
            RadarRenderableCollection collection;
            collection.RegisterRenderer(1, std::make_shared<MockRadarRenderableInterface>(), collection.afterLists);
            EXPECT_EQ(2, collection.ReserveRendererIdentifier());
        }


        TEST(RadarRenderableCollection, RegisterRendererThrowsExceptionIfRendererAlreadyExists)
        {
            RadarRenderableCollection collection;
            EXPECT_NO_THROW(
                collection.RegisterRenderer(1, std::make_shared<MockRadarRenderableInterface>(), collection.afterLists)
            );
            EXPECT_THROW(
                collection.RegisterRenderer(1, std::make_shared<MockRadarRenderableInterface>(), collection.afterLists),
                std::invalid_argument
            );
        }

        TEST(RadarRenderableCollection, RegisterRendererAddsRenderersToInitialPhase)
        {
            RadarRenderableCollection collection;
            collection.RegisterRenderer(1, std::make_shared<MockRadarRenderableInterface>(), collection.initialPhase);
            EXPECT_EQ(1, collection.CountRenderers());
            EXPECT_EQ(1, collection.CountRenderersInPhase(collection.initialPhase));
            EXPECT_EQ(0, collection.CountRenderersInPhase(collection.afterLists));
            EXPECT_EQ(0, collection.CountRenderersInPhase(collection.beforeTags));
            EXPECT_EQ(0, collection.CountRenderersInPhase(collection.afterTags));
        }

        TEST(RadarRenderableCollection, RegisterRendererAddsRenderersToAfterListsPhase)
        {
            RadarRenderableCollection collection;
            collection.RegisterRenderer(1, std::make_shared<MockRadarRenderableInterface>(), collection.afterLists);
            EXPECT_EQ(1, collection.CountRenderers());
            EXPECT_EQ(0, collection.CountRenderersInPhase(collection.initialPhase));
            EXPECT_EQ(1, collection.CountRenderersInPhase(collection.afterLists));
            EXPECT_EQ(0, collection.CountRenderersInPhase(collection.beforeTags));
            EXPECT_EQ(0, collection.CountRenderersInPhase(collection.afterTags));
        }

        TEST(RadarRenderableCollection, RegisterRendererAddsRenderersToBeforeTagsPhase)
        {
            RadarRenderableCollection collection;
            collection.RegisterRenderer(1, std::make_shared<MockRadarRenderableInterface>(), collection.beforeTags);
            EXPECT_EQ(1, collection.CountRenderers());
            EXPECT_EQ(0, collection.CountRenderersInPhase(collection.initialPhase));
            EXPECT_EQ(0, collection.CountRenderersInPhase(collection.afterLists));
            EXPECT_EQ(1, collection.CountRenderersInPhase(collection.beforeTags));
            EXPECT_EQ(0, collection.CountRenderersInPhase(collection.afterTags));
        }

        TEST(RadarRenderableCollection, RegisterRendererAddsRenderersToAfterTagsPhase)
        {
            RadarRenderableCollection collection;
            collection.RegisterRenderer(1, std::make_shared<MockRadarRenderableInterface>(), collection.afterTags);
            EXPECT_EQ(1, collection.CountRenderers());
            EXPECT_EQ(0, collection.CountRenderersInPhase(collection.initialPhase));
            EXPECT_EQ(0, collection.CountRenderersInPhase(collection.afterLists));
            EXPECT_EQ(0, collection.CountRenderersInPhase(collection.beforeTags));
            EXPECT_EQ(1, collection.CountRenderersInPhase(collection.afterTags));
        }

        TEST(RadarRenderableCollection, RegisterRendererThrowsExceptionForInvalidRenderingPhase)
        {
            RadarRenderableCollection collection;
            EXPECT_THROW(
                collection.RegisterRenderer(1, std::make_shared<MockRadarRenderableInterface>(), -999),
                std::invalid_argument
            );
        }

        TEST(RadarRenderableCollection, CountRenderersCountsTotalRenderers)
        {
            RadarRenderableCollection collection;
            collection.RegisterRenderer(1, std::make_shared<MockRadarRenderableInterface>(), collection.initialPhase);
            collection.RegisterRenderer(2, std::make_shared<MockRadarRenderableInterface>(), collection.beforeTags);
            collection.RegisterRenderer(3, std::make_shared<MockRadarRenderableInterface>(), collection.afterLists);
            collection.RegisterRenderer(4, std::make_shared<MockRadarRenderableInterface>(), collection.afterTags);
            EXPECT_EQ(4, collection.CountRenderers());
        }

        TEST(RadarRenderableCollection, ReserveScreenObjectIdentifierThrowsExceptionIfInvalidRenderer)
        {
            RadarRenderableCollection collection;
            EXPECT_THROW(collection.ReserveScreenObjectIdentifier(1), std::invalid_argument);
        }

        TEST(RadarRenderableCollection, ReserveScreenObjectIdentifierStartsAtOne)
        {
            RadarRenderableCollection collection;
            collection.RegisterRenderer(
                collection.ReserveRendererIdentifier(),
                std::make_shared<MockRadarRenderableInterface>(), collection.initialPhase
            );
            EXPECT_EQ(1, collection.ReserveScreenObjectIdentifier(1));
        }

        TEST(RadarRenderableCollection, ReserveScreenObjectIdentifierIncreasesIncrementally)
        {
            RadarRenderableCollection collection;
            collection.RegisterRenderer(
                collection.ReserveRendererIdentifier(),
                std::make_shared<MockRadarRenderableInterface>(),
                collection.initialPhase
            );
            EXPECT_EQ(1, collection.ReserveScreenObjectIdentifier(1));
            EXPECT_EQ(2, collection.ReserveScreenObjectIdentifier(1));
            EXPECT_EQ(3, collection.ReserveScreenObjectIdentifier(1));
            EXPECT_EQ(4, collection.ReserveScreenObjectIdentifier(1));
            EXPECT_EQ(5, collection.ReserveScreenObjectIdentifier(1));
        }

        TEST(RadarRenderableCollection, LeftClickScreenObjectMapsScreenObjectToRenderer)
        {
            RadarRenderableCollection collection;
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer1 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer2 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();

            int keyRendererId = collection.ReserveRendererIdentifier();
            collection.RegisterRenderer(keyRendererId, renderer1, collection.beforeTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer2, collection.beforeTags);


            int screenObjectId = collection.ReserveScreenObjectIdentifier(1);
            std::string desc = "testdesc";
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;

            EXPECT_CALL(*renderer1, LeftClick(Ref(mockRadarScreen), screenObjectId, desc, POINT {}, RECT {}))
                .Times(1);

            collection.LeftClickScreenObject(mockRadarScreen, screenObjectId, desc, {}, {});
        }

        TEST(RadarRenderableCollection, RightClickScreenObjectMapsScreenObjectToRenderer)
        {
            RadarRenderableCollection collection;
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer1 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer2 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();

            int keyRendererId = collection.ReserveRendererIdentifier();
            collection.RegisterRenderer(keyRendererId, renderer1, collection.beforeTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer2, collection.beforeTags);

            // Setup the test vars
            int screenObjectId = collection.ReserveScreenObjectIdentifier(1);
            std::string desc = "testdesc";
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;

            // One fake screen object, to prove we don't call the wrong one
            collection.ReserveScreenObjectIdentifier(2);

            EXPECT_CALL(*renderer1, RightClick(screenObjectId, desc, Ref(mockRadarScreen)))
                .Times(1);

            collection.RightClickScreenObject(screenObjectId, desc, mockRadarScreen);
        }

        TEST(RadarRenderableCollection, MoveMapsScreenObjectToRenderer)
        {
            RadarRenderableCollection collection;
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer1 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer2 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();

            int keyRendererId = collection.ReserveRendererIdentifier();
            collection.RegisterRenderer(keyRendererId, renderer1, collection.beforeTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer2, collection.beforeTags);

            // Setup test vars
            int screenObjectId = collection.ReserveScreenObjectIdentifier(1);
            RECT pos = { 1, 2, 3, 4 };
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;

            // One fake screen object, to prove we don't call the wrong one
            collection.ReserveScreenObjectIdentifier(2);

            EXPECT_CALL(*renderer1, Move(RectEq(pos), ""))
                .Times(1);

            collection.MoveScreenObject(screenObjectId, {}, pos);
        }

        TEST(RadarRenderableCollection, RenderCallsCorrectRendererInitialPhase)
        {
            RadarRenderableCollection collection;
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer1 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer2 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer3 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer4 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();

            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer1, collection.initialPhase);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer2, collection.beforeTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer3, collection.afterTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer4, collection.afterLists);

            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            StrictMock<MockGraphicsInterface> mockGraphics;

            EXPECT_CALL(*renderer1, Render(Ref(mockGraphics), Ref(mockRadarScreen)))
                .Times(1);

            EXPECT_CALL(*renderer1, IsVisible())
                .Times(1)
                .WillOnce(Return(true));

            collection.Render(collection.initialPhase, mockGraphics, mockRadarScreen);
        }

        TEST(RadarRenderableCollection, RenderCallsCorrectRendererBeforeTags)
        {
            RadarRenderableCollection collection;
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer1 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer2 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer3 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer4 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();

            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer1, collection.initialPhase);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer2, collection.beforeTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer3, collection.afterTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer4, collection.afterLists);

            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            StrictMock<MockGraphicsInterface> mockGraphics;

            EXPECT_CALL(*renderer2, Render(Ref(mockGraphics), Ref(mockRadarScreen)))
                .Times(1);

            EXPECT_CALL(*renderer2, IsVisible())
                .Times(1)
                .WillOnce(Return(true));

            collection.Render(collection.beforeTags, mockGraphics, mockRadarScreen);
        }

        TEST(RadarRenderableCollection, RenderCallsCorrectRendererAfterTags)
        {
            RadarRenderableCollection collection;
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer1 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer2 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer3 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer4 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();

            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer1, collection.initialPhase);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer2, collection.beforeTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer3, collection.afterTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer4, collection.afterLists);

            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            StrictMock<MockGraphicsInterface> mockGraphics;

            EXPECT_CALL(*renderer3, Render(Ref(mockGraphics), Ref(mockRadarScreen)))
                .Times(1);

            EXPECT_CALL(*renderer3, IsVisible())
                .Times(1)
                .WillOnce(Return(true));

            collection.Render(collection.afterTags, mockGraphics, mockRadarScreen);
        }

        TEST(RadarRenderableCollection, RenderCallsCorrectRendererAfterLists)
        {
            RadarRenderableCollection collection;
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer1 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer2 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer3 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer4 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();

            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer1, collection.initialPhase);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer2, collection.beforeTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer3, collection.afterTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer4, collection.afterLists);

            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            StrictMock<MockGraphicsInterface> mockGraphics;

            EXPECT_CALL(*renderer4, Render(Ref(mockGraphics), Ref(mockRadarScreen)))
                .Times(1);

            EXPECT_CALL(*renderer4, IsVisible())
                .Times(1)
                .WillOnce(Return(true));

            collection.Render(collection.afterLists, mockGraphics, mockRadarScreen);
        }

        TEST(RadarRenderableCollection, RenderDoesntRenderIfNotVisible)
        {
            RadarRenderableCollection collection;
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer1 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer2 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer3 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer4 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();

            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer1, collection.initialPhase);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer2, collection.beforeTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer3, collection.afterTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer4, collection.afterLists);

            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            StrictMock<MockGraphicsInterface> mockGraphics;

            EXPECT_CALL(*renderer1, IsVisible())
                .Times(1)
                .WillOnce(Return(false));

            collection.Render(collection.initialPhase, mockGraphics, mockRadarScreen);
        }

        TEST(RadarRenderableCollection, ResetPositionResetsPositionOfAllRenderers)
        {
            RadarRenderableCollection collection;
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer1 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer2 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();
            std::shared_ptr<StrictMock<MockRadarRenderableInterface>> renderer3 =
                std::make_shared<StrictMock<MockRadarRenderableInterface>>();

            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer1, collection.initialPhase);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer2, collection.beforeTags);
            collection.RegisterRenderer(collection.ReserveRendererIdentifier(), renderer3, collection.afterTags);

            EXPECT_CALL(*renderer1, ResetPosition())
                .Times(1);

            EXPECT_CALL(*renderer2, ResetPosition())
                .Times(1);

            EXPECT_CALL(*renderer3, ResetPosition())
                .Times(1);

            collection.ResetPosition();
        }
    }  // namespace RadarScreen
}  // namespace UKControllerPluginTest
