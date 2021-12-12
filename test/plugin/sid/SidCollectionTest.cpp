#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"
#include "memory"

using ::testing::Test;
using UKControllerPlugin::Sid::SidCollection;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;

namespace UKControllerPluginTest::Sid {

    class SidCollectionTest : public Test
    {
        public:
        SidCollectionTest()
        {
            this->sid1 = std::make_shared<StandardInstrumentDeparture>("EGLL", "TEST1A", 1, 2, 3);
            this->sid2 = std::make_shared<StandardInstrumentDeparture>("EGLL", "TEST1B", 3, 4, 5);
            this->sid3 = std::make_shared<StandardInstrumentDeparture>("EGBB", "TEST1A", 5, 6, 7);
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
        EXPECT_EQ(nullptr, this->sids.GetByAirfieldAndIdentifier("EGLL", "TEST1X"));
    }

    TEST_F(SidCollectionTest, ItReturnsASid)
    {
        this->sids.AddSid(this->sid1);
        this->sids.AddSid(this->sid2);
        this->sids.AddSid(this->sid3);
        EXPECT_EQ(this->sid2, this->sids.GetByAirfieldAndIdentifier("EGLL", "TEST1B"));
    }

    TEST_F(SidCollectionTest, ItReturnsASidByFlightplan)
    {
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;

        ON_CALL(mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGLL"));

        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("TEST1B"));

        this->sids.AddSid(this->sid1);
        this->sids.AddSid(this->sid2);
        this->sids.AddSid(this->sid3);
        EXPECT_EQ(this->sid2, this->sids.GetForFlightplan(mockFlightplan));
    }

    TEST_F(SidCollectionTest, ItReturnsNullptrIfSidNotFoundByFlightplan)
    {
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;

        ON_CALL(mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGLL"));

        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("TEST1Z"));

        this->sids.AddSid(this->sid1);
        this->sids.AddSid(this->sid2);
        this->sids.AddSid(this->sid3);
        EXPECT_EQ(nullptr, this->sids.GetForFlightplan(mockFlightplan));
    }
} // namespace UKControllerPluginTest::Sid
