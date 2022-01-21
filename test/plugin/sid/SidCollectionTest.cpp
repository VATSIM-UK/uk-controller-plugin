#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

using ::testing::Test;
using UKControllerPlugin::Sid::SidCollection;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;

namespace UKControllerPluginTest::Sid {

    class SidCollectionTest : public Test
    {
        public:
        SidCollectionTest()
        {
            this->sid1 = std::make_shared<StandardInstrumentDeparture>(1, 1, "TEST1A", 1, 2, 3);
            this->sid2 = std::make_shared<StandardInstrumentDeparture>(2, 1, "TEST1B", 3, 4, 5);
            this->sid3 = std::make_shared<StandardInstrumentDeparture>(3, 2, "TEST1A", 5, 6, 7);
        }

        std::shared_ptr<StandardInstrumentDeparture> sid1;
        std::shared_ptr<StandardInstrumentDeparture> sid2;
        std::shared_ptr<StandardInstrumentDeparture> sid3;
        SidCollection sids;
    };

    TEST_F(SidCollectionTest, ItStartEmpty)
    {
        EXPECT_EQ(0, this->sids.CountSids());
    }

    TEST_F(SidCollectionTest, ItDoesntAddDuplicateSids)
    {
        this->sids.AddSid(this->sid1);
        this->sids.AddSid(this->sid1);
        this->sids.AddSid(this->sid1);
        EXPECT_EQ(1, this->sids.CountSids());
    }

    TEST_F(SidCollectionTest, ItAddsSids)
    {
        this->sids.AddSid(this->sid1);
        this->sids.AddSid(this->sid2);
        this->sids.AddSid(this->sid3);
        EXPECT_EQ(3, this->sids.CountSids());
    }

    TEST_F(SidCollectionTest, ItReturnsNullPtrIfItCantFindASid)
    {
        this->sids.AddSid(this->sid1);
        this->sids.AddSid(this->sid2);
        this->sids.AddSid(this->sid3);
        EXPECT_EQ(nullptr, this->sids.GetByRunwayIdAndIdentifier(1, "TEST1X"));
    }

    TEST_F(SidCollectionTest, ItReturnsASid)
    {
        this->sids.AddSid(this->sid1);
        this->sids.AddSid(this->sid2);
        this->sids.AddSid(this->sid3);
        EXPECT_EQ(this->sid2, this->sids.GetByRunwayIdAndIdentifier(1, "TEST1B"));
    }

    TEST_F(SidCollectionTest, ItReturnsASidHandlingDeprecationsInTheSectorfile)
    {
        this->sids.AddSid(this->sid1);
        this->sids.AddSid(this->sid2);
        this->sids.AddSid(this->sid3);
        EXPECT_EQ(this->sid2, this->sids.GetByRunwayIdAndIdentifier(1, "#TEST1B"));
    }

    TEST_F(SidCollectionTest, ItReturnsNullPtrIfNoEuroscopeSid)
    {
        this->sids.AddSid(this->sid1);
        this->sids.AddSid(this->sid2);
        this->sids.AddSid(this->sid3);
        EXPECT_EQ(nullptr, this->sids.GetByRunwayIdAndIdentifier(1, ""));
    }

    TEST_F(SidCollectionTest, ItReturnsASidById)
    {
        this->sids.AddSid(this->sid1);
        this->sids.AddSid(this->sid2);
        this->sids.AddSid(this->sid3);
        EXPECT_EQ(this->sid2, this->sids.GetById(2));
    }

    TEST_F(SidCollectionTest, ItReturnsNullptrIfSidNotFoundById)
    {
        this->sids.AddSid(this->sid1);
        this->sids.AddSid(this->sid2);
        this->sids.AddSid(this->sid3);
        EXPECT_EQ(nullptr, this->sids.GetById(55));
    }
} // namespace UKControllerPluginTest::Sid
