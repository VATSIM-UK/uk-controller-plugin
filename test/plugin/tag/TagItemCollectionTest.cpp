#include "pch/pch.h"
#include "tag/TagItemCollection.h"
#include "tag/TagItemInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "tag/TagData.h"

using testing::Test;
using UKControllerPlugin::Tag::TagData;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Tag::TagItemCollection;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace Tag {

        class TagItemCollectionTest : public Test
        {
            public:
                double fontSize = 24.1;
                COLORREF tagColour = RGB(255, 255, 255);
                int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
                char itemString[16] = "Foooooo";
        };

        class FakeTagItem : public UKControllerPlugin::Tag::TagItemInterface
        {
            public:
                FakeTagItem(std::string desc, std::string data)
                    : desc(desc), data(data)
                {

                }

                std::string GetTagItemDescription(int tagItemId) const
                {
                    return this->desc;
                }

                void SetTagItemData(TagData& tagData) {
                    tagData.SetItemString(this->data);
                }

                // Tag item description
                const std::string desc;

                // Tag item data
                const std::string data;
        };

        TEST_F(TagItemCollectionTest, RegisterTagItemThrowsExceptionIfIdAlreadyExists)
        {
            TagItemCollection collection;
            EXPECT_NO_THROW(
                collection.RegisterTagItem(1, std::make_shared<FakeTagItem>("testdesc", "testdata"))
            );
            EXPECT_THROW(
                collection.RegisterTagItem(1, std::make_shared<FakeTagItem>("testdesc", "testdata")),
                std::invalid_argument
            );
        }

        TEST_F(TagItemCollectionTest, RegisterAllItemsWithEuroscopeCallsCorrectly)
        {
            TagItemCollection collection;
            StrictMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
            EXPECT_CALL(mockPlugin, RegisterTagItem(1, "testdesc"))
                .Times(1);

            collection.RegisterTagItem(1, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            collection.RegisterAllItemsWithEuroscope(mockPlugin);
        }

        TEST_F(TagItemCollectionTest, TagItemUpdateReturnsErrorIfNotRegistered)
        {
            TagItemCollection collection;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
            StrictMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
            TagData tagData(
                mockFlightplan,
                mockRadarTarget,
                1,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                itemString,
                &euroscopeColourCode,
                &tagColour,
                &fontSize
            );

            collection.TagItemUpdate(tagData);
            EXPECT_EQ(collection.errorTagItemText, tagData.GetItemString());
        }

        TEST_F(TagItemCollectionTest, TagItemUpdateSetsTagItemData)
        {
            TagItemCollection collection;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
            StrictMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
            TagData tagData(
                mockFlightplan,
                mockRadarTarget,
                1,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                itemString,
                &euroscopeColourCode,
                &tagColour,
                &fontSize
            );

            collection.RegisterTagItem(1, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            collection.TagItemUpdate(tagData);
            EXPECT_EQ("testdata", tagData.GetItemString());
        }

        TEST_F(TagItemCollectionTest, StartsEmpty)
        {
            TagItemCollection collection;
            EXPECT_EQ(0, collection.CountHandlers());
        }

        TEST_F(TagItemCollectionTest, CountHandlersReturnsNumberOfHandlers)
        {
            TagItemCollection collection;
            collection.RegisterTagItem(1, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            EXPECT_EQ(1, collection.CountHandlers());
            collection.RegisterTagItem(100, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            EXPECT_EQ(2, collection.CountHandlers());
            collection.RegisterTagItem(5, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            EXPECT_EQ(3, collection.CountHandlers());
        }

        TEST_F(TagItemCollectionTest, HasHandlerForItemIdReturnsFalseIfIdNotRegistered)
        {
            TagItemCollection collection;
            collection.RegisterTagItem(2, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            EXPECT_FALSE(collection.HasHandlerForItemId(5));
        }

        TEST_F(TagItemCollectionTest, HasHandlerForItemIdReturnsTrueIfIdRegistered)
        {
            TagItemCollection collection;
            collection.RegisterTagItem(5, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            EXPECT_TRUE(collection.HasHandlerForItemId(5));
        }
    }  // namespace Tag
}  // namespace UKControllerPluginTest
