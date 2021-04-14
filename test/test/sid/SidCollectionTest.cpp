#include "pch/pch.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

using ::testing::Test;
using UKControllerPlugin::Sid::SidCollection;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;

namespace UKControllerPluginTest {
    namespace Sid {

        class SidCollectionTest : public Test
        {
            public:
                SidCollectionTest()
                {
                    this->sid1.reset(new StandardInstrumentDeparture("EGLL", "TEST1A", 1, 2));
                    this->sid2.reset(new StandardInstrumentDeparture("EGLL", "TEST1B", 3, 4));
                    this->sid3.reset(new StandardInstrumentDeparture("EGBB", "TEST1A", 5, 6));
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
    }  // namespace Sid
}  // namespace UKControllerPluginTest
