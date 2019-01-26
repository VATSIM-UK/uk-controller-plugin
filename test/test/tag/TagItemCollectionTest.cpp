#include "pch/pch.h"
#include "tag/TagItemCollection.h"
#include "tag/TagItemInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"

using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Tag::TagItemCollection;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace Tag {

        class FakeTagItem : public UKControllerPlugin::Tag::TagItemInterface
        {
            public:
                FakeTagItem(std::string desc, std::string data)
                    : desc(desc), data(data)
                {

                }

                std::string GetTagItemDescription(void) const
                {
                    return this->desc;
                }

                std::string GetTagItemData(
                    EuroScopeCFlightPlanInterface & flightPlan, EuroScopeCRadarTargetInterface & radarTarget
                ) {
                    return this->data;
                }

                // Tag item description
                const std::string desc;

                // Tag item data
                const std::string data;
        };

        TEST(TagItemCollection, RegisterTagItemThrowsExceptionIfIdAlreadyExists)
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

        TEST(TagItemCollection, RegisterAllItemsWithEuroscopeCallsCorrectly)
        {
            TagItemCollection collection;
            StrictMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
            EXPECT_CALL(mockPlugin, RegisterTagItem(1, "testdesc"))
                .Times(1);

            collection.RegisterTagItem(1, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            collection.RegisterAllItemsWithEuroscope(mockPlugin);
        }

        TEST(TagItemCollection, TagItemUpdateReturnsErrorIfNotRegistered)
        {
            TagItemCollection collection;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
            StrictMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
            char test[16];
            collection.TagItemUpdate(25421, test, mockFlightplan, mockRadarTarget);
            EXPECT_EQ(0, collection.errorTagItemText.compare(test));
        }

        TEST(TagItemCollection, TagItemUpdateSetsInvalidIfDataTooLarge)
        {
            TagItemCollection collection;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
            StrictMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
            char test[16];
            collection.RegisterTagItem(1, std::make_shared<FakeTagItem>("testdesc", "thisdataistoolongforthetagitem"));
            collection.TagItemUpdate(1, test, mockFlightplan, mockRadarTarget);

            EXPECT_EQ(0, collection.invalidTagItemText.compare(test));
        }

        TEST(TagItemCollection, TagItemUpdateSetsTagItemData)
        {
            TagItemCollection collection;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
            StrictMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
            char test[16];
            collection.RegisterTagItem(1, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            collection.TagItemUpdate(1, test, mockFlightplan, mockRadarTarget);

            EXPECT_EQ(0, strcmp("testdata", test));
        }

        TEST(TagItemCollection, StartsEmpty)
        {
            TagItemCollection collection;
            EXPECT_EQ(0, collection.CountHandlers());
        }

        TEST(TagItemCollection, CountHandlersReturnsNumberOfHandlers)
        {
            TagItemCollection collection;
            collection.RegisterTagItem(1, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            EXPECT_EQ(1, collection.CountHandlers());
            collection.RegisterTagItem(100, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            EXPECT_EQ(2, collection.CountHandlers());
            collection.RegisterTagItem(5, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            EXPECT_EQ(3, collection.CountHandlers());
        }

        TEST(TagItemCollection, HasHandlerForItemIdReturnsFalseIfIdNotRegistered)
        {
            TagItemCollection collection;
            collection.RegisterTagItem(2, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            EXPECT_FALSE(collection.HasHandlerForItemId(5));
        }

        TEST(TagItemCollection, HasHandlerForItemIdReturnsTrueIfIdRegistered)
        {
            TagItemCollection collection;
            collection.RegisterTagItem(5, std::make_shared<FakeTagItem>("testdesc", "testdata"));
            EXPECT_TRUE(collection.HasHandlerForItemId(5));
        }
    }  // namespace Tag
}  // namespace UKControllerPluginTest
