#include "releases/CompareDepartureReleases.h"
#include "releases/DepartureReleaseRequest.h"

using UKControllerPlugin::Releases::DepartureReleaseRequest;
using UKControllerPlugin::Releases::CompareDepartureReleases;
using ::testing::Test;

namespace UKControllerPluginTest::Releases {

    class CompareDepartureReleasesTest : public Test
    {
        public:
            CompareDepartureReleasesTest()
            {
                request1 = std::make_shared<DepartureReleaseRequest>(
                    1,
                    "BAW123",
                    1,
                    2,
                    std::chrono::system_clock::now()
                );
                request2 = std::make_shared<DepartureReleaseRequest>(
                    2,
                    "BAW456",
                    1,
                    2,
                    std::chrono::system_clock::now()
                );
            }

            std::shared_ptr<DepartureReleaseRequest> request1;
            std::shared_ptr<DepartureReleaseRequest> request2;
            CompareDepartureReleases compare;
    };

    TEST_F(CompareDepartureReleasesTest, LessThanIntReturnsTrueIfLessThan)
    {
        EXPECT_TRUE(compare(request1, 2));
    }

    TEST_F(CompareDepartureReleasesTest, LessThanClassReturnsTrueIfLessThan)
    {
        EXPECT_TRUE(compare(0, request1));
    }

    TEST_F(CompareDepartureReleasesTest, CompareReturnsTrueIfFirstLessThanLast)
    {
        EXPECT_TRUE(compare(request1, request2));
    }
} // namespace UKControllerPluginTest::Releases
