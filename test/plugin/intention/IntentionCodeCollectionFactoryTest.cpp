#include "controller/ActiveCallsignCollection.h"
#include "intention/AirfieldIdentifier.h"
#include "intention/AllOf.h"
#include "intention/AnyOf.h"
#include "intention/ArrivalAirfieldPattern.h"
#include "intention/ArrivalAirfields.h"
#include "intention/ControllerPositionStartsWith.h"
#include "intention/CruisingLevelAbove.h"
#include "intention/ExitingFirAtPoint.h"
#include "intention/FullAirfieldIdentifier.h"
#include "intention/IntentionCodeCollection.h"
#include "intention/IntentionCodeCollectionFactory.h"
#include "intention/IntentionCodeMetadata.h"
#include "intention/IntentionCodeModel.h"
#include "intention/MaximumCruisingAltitude.h"
#include "intention/Not.h"
#include "intention/RoutingVia.h"
#include "intention/SingleCode.h"

using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::IntentionCode::AirfieldIdentifier;
using UKControllerPlugin::IntentionCode::AllOf;
using UKControllerPlugin::IntentionCode::AnyOf;
using UKControllerPlugin::IntentionCode::ArrivalAirfieldPattern;
using UKControllerPlugin::IntentionCode::ArrivalAirfields;
using UKControllerPlugin::IntentionCode::CodeValid;
using UKControllerPlugin::IntentionCode::ConditionsValid;
using UKControllerPlugin::IntentionCode::ConditionValid;
using UKControllerPlugin::IntentionCode::ControllerPositionStartsWith;
using UKControllerPlugin::IntentionCode::CruisingLevelAbove;
using UKControllerPlugin::IntentionCode::ExitingFirAtPoint;
using UKControllerPlugin::IntentionCode::FullAirfieldIdentifier;
using UKControllerPlugin::IntentionCode::IntentionCodeCollection;
using UKControllerPlugin::IntentionCode::IntentionCodeModel;
using UKControllerPlugin::IntentionCode::IntentionCodeValid;
using UKControllerPlugin::IntentionCode::MakeIntentionCodeCollection;
using UKControllerPlugin::IntentionCode::MaximumCruisingAltitude;
using UKControllerPlugin::IntentionCode::Not;
using UKControllerPlugin::IntentionCode::RoutingVia;
using UKControllerPlugin::IntentionCode::SingleCode;

namespace UKControllerPluginTest::IntentionCode {
    class IntentionCodeCollectionFactoryTest : public testing::Test
    {
        public:
        IntentionCodeCollectionFactoryTest()
            : callsigns(std::make_shared<ActiveCallsignCollection>()),
              generator(std::make_shared<testing::NiceMock<MockAircraftFirExitGenerator>>())
        {
        }

        std::shared_ptr<ActiveCallsignCollection> callsigns;
        std::shared_ptr<testing::NiceMock<MockAircraftFirExitGenerator>> generator;
    };

    TEST_F(IntentionCodeCollectionFactoryTest, ConditionsAreValid)
    {
        const auto subCondition = nlohmann::json{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}};
        const auto subCondition2 = nlohmann::json{{"type", "controller_position_starts_with"}, {"starts_with", "LON"}};
        EXPECT_TRUE(ConditionsValid(nlohmann::json::array({subCondition, subCondition2})));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ConditionsInvalidIfNotArray)
    {
        const auto subCondition = nlohmann::json{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}};
        const auto subCondition2 =
            nlohmann::json{{"type", "controller_position_starts_with"}, {"not_starts_with", "LON"}};
        EXPECT_FALSE(ConditionsValid(nlohmann::json::array({subCondition, subCondition2})));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ConditionsInvalidAConditionIsInvalid)
    {
        EXPECT_FALSE(ConditionsValid(nlohmann::json::object()));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ConditionIsInvalidIfNotAnObject)
    {
        EXPECT_FALSE(ConditionValid(nlohmann::json::array()));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ConditionIsInvalidIfDoesntContainType)
    {
        const auto condition =
            nlohmann::json{{"not_type", "arrival_airfields"}, {"airfields", nlohmann::json::array({"EGKK", "EGLL"})}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ConditionIsInvalidIfTypeNotString)
    {
        const auto condition = nlohmann::json{{"type", 1}, {"airfields", nlohmann::json::array({"EGKK", "EGLL"})}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ArrivalAirfieldsConditionIsValid)
    {
        const auto condition =
            nlohmann::json{{"type", "arrival_airfields"}, {"airfields", nlohmann::json::array({"EGKK", "EGLL"})}};
        EXPECT_TRUE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ArrivalAirfieldsConditionIsInvalidNoAirfields)
    {
        const auto condition =
            nlohmann::json{{"type", "arrival_airfields"}, {"not_airfields", nlohmann::json::array({"EGKK", "EGLL"})}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ArrivalAirfieldsConditionIsInvalidAirfieldsNotArray)
    {
        const auto condition = nlohmann::json{{"type", "arrival_airfields"}, {"airfields", nlohmann::json::object()}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ArrivalAirfieldsConditionIsInvalidIfAirfieldIsInvalid)
    {
        const auto condition =
            nlohmann::json{{"type", "arrival_airfields"}, {"airfields", nlohmann::json::array({"EGKK", 2, "EGLL"})}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ArrivalAirfieldPatternConditionIsValid)
    {
        const auto condition = nlohmann::json{{"type", "arrival_airfield_pattern"}, {"pattern", "EG"}};
        EXPECT_TRUE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ArrivalAirfieldPatternConditionIsInvalidNoPattern)
    {
        const auto condition = nlohmann::json{{"type", "arrival_airfield_pattern"}, {"not_pattern", "EG"}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ArrivalAirfieldsConditionIsInvalidPatternNotString)
    {
        const auto condition = nlohmann::json{{"type", "arrival_airfield_pattern"}, {"pattern", 123}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, MaximumCruisingLevelConditionIsValid)
    {
        const auto condition = nlohmann::json{{"type", "maximum_cruising_level"}, {"level", 15000}};
        EXPECT_TRUE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, MaximumCruisingLevelConditionIsInvalidNoLevel)
    {
        const auto condition = nlohmann::json{{"type", "maximum_cruising_level"}, {"not_level", 15000}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, MaximumCruisingLevelConditionIsInvalidLevelNotInteger)
    {
        const auto condition = nlohmann::json{{"type", "arrival_airfield_pattern"}, {"level", "abc"}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, CrusingLevelAboveConditionIsValid)
    {
        const auto condition = nlohmann::json{{"type", "cruising_level_above"}, {"level", 15000}};
        EXPECT_TRUE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, CrusingLevelAboveConditionIsInvalidNoLevel)
    {
        const auto condition = nlohmann::json{{"type", "cruising_level_above"}, {"not_level", 15000}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, CrusingLevelAboveConditionIsInvalidLevelNotInteger)
    {
        const auto condition = nlohmann::json{{"type", "cruising_level_above"}, {"level", "abc"}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, RoutingViaPatternConditionIsValid)
    {
        const auto condition = nlohmann::json{{"type", "routing_via"}, {"point", "KOK"}};
        EXPECT_TRUE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, RoutingViaPatternConditionIsInvalidNoVia)
    {
        const auto condition = nlohmann::json{{"type", "routing_via"}, {"not_point", "KOK"}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, RoutingViaConditionIsInvalidViaNotString)
    {
        const auto condition = nlohmann::json{{"type", "routing_via"}, {"point", 123}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ControllerPositionStartsWithConditionIsValid)
    {
        const auto condition = nlohmann::json{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}};
        EXPECT_TRUE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ControllerPositionStartsWithConditionIsInvalidNoVia)
    {
        const auto condition = nlohmann::json{{"type", "controller_position_starts_with"}, {"not_starts_with", "SCO"}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ControllerPositionStartsWithIsInvalidViaNotString)
    {
        const auto condition = nlohmann::json{{"type", "controller_position_starts_with"}, {"starts_with", 123}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, NotConditionIsValid)
    {
        const auto subCondition = nlohmann::json{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}};
        const auto condition = nlohmann::json{{"type", "not"}, {"conditions", nlohmann::json::array({subCondition})}};
        EXPECT_TRUE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, NotConditionIsInvalidNoConditions)
    {
        const auto subCondition = nlohmann::json{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}};
        const auto condition =
            nlohmann::json{{"type", "not"}, {"not_conditions", nlohmann::json::array({subCondition})}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, NotIsInvalidSubconditionInvalid)
    {
        const auto subCondition =
            nlohmann::json{{"type", "controller_position_starts_with"}, {"not_starts_with", "SCO"}};
        const auto condition = nlohmann::json{{"type", "not"}, {"conditions", nlohmann::json::array({subCondition})}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, AllOfConditionIsValid)
    {
        const auto subCondition = nlohmann::json{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}};
        const auto condition =
            nlohmann::json{{"type", "all_of"}, {"conditions", nlohmann::json::array({subCondition})}};
        EXPECT_TRUE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, AllOfConditionIsInvalidNoConditions)
    {
        const auto subCondition = nlohmann::json{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}};
        const auto condition =
            nlohmann::json{{"type", "all_of"}, {"not_conditions", nlohmann::json::array({subCondition})}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, AllOfIsInvalidSubconditionInvalid)
    {
        const auto subCondition =
            nlohmann::json{{"type", "controller_position_starts_with"}, {"not_starts_with", "SCO"}};
        const auto condition =
            nlohmann::json{{"type", "all_of"}, {"conditions", nlohmann::json::array({subCondition})}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, AnyOfConditionIsValid)
    {
        const auto subCondition = nlohmann::json{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}};
        const auto condition =
            nlohmann::json{{"type", "any_of"}, {"conditions", nlohmann::json::array({subCondition})}};
        EXPECT_TRUE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, AnyOfConditionIsInvalidNoConditions)
    {
        const auto subCondition = nlohmann::json{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}};
        const auto condition =
            nlohmann::json{{"type", "any_of"}, {"not_conditions", nlohmann::json::array({subCondition})}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, AnyOfIsInvalidSubconditionInvalid)
    {
        const auto subCondition =
            nlohmann::json{{"type", "controller_position_starts_with"}, {"not_starts_with", "SCO"}};
        const auto condition =
            nlohmann::json{{"type", "any_of"}, {"conditions", nlohmann::json::array({subCondition})}};
        EXPECT_FALSE(ConditionValid(condition));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, CodeIsInvalidNotObject)
    {
        const auto code = nlohmann::json::array();
        EXPECT_FALSE(CodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, CodeIsInvalidNoType)
    {
        const auto code = nlohmann::json{{"not_type", "airfield_identifier"}};
        EXPECT_FALSE(CodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, CodeIsInvalidTypeNotString)
    {
        const auto code = nlohmann::json{{"type", 123}};
        EXPECT_FALSE(CodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, CodeIsInvalidTypeNotValid)
    {
        const auto code = nlohmann::json{{"type", "foo"}};
        EXPECT_FALSE(CodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, CodeIsValidAirfieldIdentifier)
    {
        const auto code = nlohmann::json{{"type", "airfield_identifier"}};
        EXPECT_TRUE(CodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, CodeIsValidSingleCode)
    {
        const auto code = nlohmann::json{{"type", "single_code"}, {"code", "AM"}};
        EXPECT_TRUE(CodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, SingleCodeIsInvalidCodeMissing)
    {
        const auto code = nlohmann::json{{"type", "single_code"}, {"not_code", "AM"}};
        EXPECT_FALSE(CodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, SingleCodeIsInvalidCodeNotString)
    {
        const auto code = nlohmann::json{{"type", "single_code"}, {"code", 123}};
        EXPECT_FALSE(CodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, IntentionCodeIsValid)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "airfield_identifier"}}},
            {"conditions", nlohmann::json::array({{{"type", "arrival_airfield_pattern"}, {"pattern", "EG"}}})}};
        EXPECT_TRUE(IntentionCodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, IntentionCodeIsInValidIfNotObject)
    {
        EXPECT_FALSE(IntentionCodeValid(nlohmann::json::array()));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, IntentionCodeIsInvalidIdMissing)
    {
        const auto code = nlohmann::json{
            {"code", {{"type", "airfield_identifier"}}},
            {"conditions", nlohmann::json::array({{{"type", "arrival_airfield_pattern"}, {"pattern", "EG"}}})}};
        EXPECT_FALSE(IntentionCodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, IntentionCodeIsInvalidIdNotInteger)
    {
        const auto code = nlohmann::json{
            {"id", "abc"},
            {"code", {{"type", "airfield_identifier"}}},
            {"conditions", nlohmann::json::array({{{"type", "arrival_airfield_pattern"}, {"pattern", "EG"}}})}};
        EXPECT_FALSE(IntentionCodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, IntentionCodeIsInvalidCodeMissing)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"conditions", nlohmann::json::array({{{"type", "arrival_airfield_pattern"}, {"pattern", "EG"}}})}};
        EXPECT_FALSE(IntentionCodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, IntentionCodeIsInvalidCodeInvalid)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", "abc"},
            {"conditions", nlohmann::json::array({{{"type", "arrival_airfield_pattern"}, {"pattern", "EG"}}})}};
        EXPECT_FALSE(IntentionCodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, IntentionCodeIsInvalidConditionsMissing)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "airfield_identifier"}}},
        };
        EXPECT_FALSE(IntentionCodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, IntentionCodeIsInvalidConditionsInvalid)
    {
        const auto code = nlohmann::json{{"id", 1}, {"code", {{"type", "airfield_identifier"}}}, {"conditions", "abc"}};
        EXPECT_FALSE(IntentionCodeValid(code));
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesASingleItemCollectionIfDependencyInvalid)
    {
        const auto collection = MakeIntentionCodeCollection(nlohmann::json::object(), generator, callsigns);
        EXPECT_EQ(1, collection->Count());

        const auto intention = collection->FindById(-1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(-1, intention->Id());

        try {
            static_cast<void>(dynamic_cast<const FullAirfieldIdentifier&>(intention->Generator()));
        } catch (std::bad_cast&) {
            FAIL();
        }

        try {
            const auto& castedCondition = dynamic_cast<const AllOf&>(intention->Conditions());
            EXPECT_EQ(0, castedCondition.Subconditions().size());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItAddsTheDefaultFallbackCodeForCompleteDependency)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "airfield_identifier"}}},
            {"conditions", nlohmann::json::array({{{"type", "arrival_airfield_pattern"}, {"pattern", "EG"}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);
        EXPECT_EQ(2, collection->Count());

        // Checking the default condition
        const auto intention = collection->FindById(-1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(-1, intention->Id());

        try {
            static_cast<void>(dynamic_cast<const SingleCode&>(intention->Generator()));
            EXPECT_EQ("", dynamic_cast<const SingleCode&>(intention->Generator()).Code());
        } catch (std::bad_cast&) {
            FAIL();
        }

        try {
            const auto& castedCondition = dynamic_cast<const AllOf&>(intention->Conditions());
            EXPECT_EQ(0, castedCondition.Subconditions().size());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesAirfieldIdentifierCode)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "airfield_identifier"}}},
            {"conditions", nlohmann::json::array({{{"type", "arrival_airfield_pattern"}, {"pattern", "EG"}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);

        const auto intention = collection->FindById(1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(1, intention->Id());

        try {
            static_cast<void>(dynamic_cast<const AirfieldIdentifier&>(intention->Generator()));
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesSingleCodeCode)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "single_code"}, {"code", "AM"}}},
            {"conditions", nlohmann::json::array({{{"type", "arrival_airfield_pattern"}, {"pattern", "EG"}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);

        const auto intention = collection->FindById(1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(1, intention->Id());

        try {
            const auto& castedGenerator = dynamic_cast<const SingleCode&>(intention->Generator());
            EXPECT_EQ("AM", castedGenerator.Code());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesACodeWithArrivalAirfieldsCondition)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "single_code"}, {"code", "AM"}}},
            {"conditions",
             nlohmann::json::array(
                 {{{"type", "arrival_airfields"}, {"airfields", nlohmann::json::array({"EGKK", "EGLL"})}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);

        const auto intention = collection->FindById(1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(1, intention->Id());

        try {
            const auto& castedConditions = dynamic_cast<const AllOf&>(intention->Conditions());
            const auto& subconditions = castedConditions.Subconditions();
            EXPECT_EQ(1, subconditions.size());
            const auto firstCondition = dynamic_cast<const ArrivalAirfields&>(*subconditions.front());
            EXPECT_EQ(std::set<std::string>({"EGKK", "EGLL"}), firstCondition.Airfields());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesACodeWithArrivalAirfieldPatternCondition)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "single_code"}, {"code", "AM"}}},
            {"conditions", nlohmann::json::array({{{"type", "arrival_airfield_pattern"}, {"pattern", "EG"}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);

        const auto intention = collection->FindById(1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(1, intention->Id());

        try {
            const auto& castedConditions = dynamic_cast<const AllOf&>(intention->Conditions());
            const auto& subconditions = castedConditions.Subconditions();
            EXPECT_EQ(1, subconditions.size());
            const auto firstCondition = dynamic_cast<const ArrivalAirfieldPattern&>(*subconditions.front());
            EXPECT_EQ("EG", firstCondition.Pattern());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesACodeWithAFirExitPointCondition)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "single_code"}, {"code", "AM"}}},
            {"conditions", nlohmann::json::array({{{"type", "exit_point"}, {"exit_point", 2}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);

        const auto intention = collection->FindById(1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(1, intention->Id());

        try {
            const auto& castedConditions = dynamic_cast<const AllOf&>(intention->Conditions());
            const auto& subconditions = castedConditions.Subconditions();
            EXPECT_EQ(1, subconditions.size());
            const auto firstCondition = dynamic_cast<const ExitingFirAtPoint&>(*subconditions.front());
            EXPECT_EQ(2, firstCondition.ExitPoint());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(2, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesACodeWithAMaximumCrusingLevelCondition)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "single_code"}, {"code", "AM"}}},
            {"conditions", nlohmann::json::array({{{"type", "maximum_cruising_level"}, {"level", 19500}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);

        const auto intention = collection->FindById(1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(1, intention->Id());

        try {
            const auto& castedConditions = dynamic_cast<const AllOf&>(intention->Conditions());
            const auto& subconditions = castedConditions.Subconditions();
            EXPECT_EQ(1, subconditions.size());
            const auto firstCondition = dynamic_cast<const MaximumCruisingAltitude&>(*subconditions.front());
            EXPECT_EQ(19500, firstCondition.Altitude());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesACodeWithACruisingLevelAboveCondition)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "single_code"}, {"code", "AM"}}},
            {"conditions", nlohmann::json::array({{{"type", "cruising_level_above"}, {"level", 19500}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);

        const auto intention = collection->FindById(1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(1, intention->Id());

        try {
            const auto& castedConditions = dynamic_cast<const AllOf&>(intention->Conditions());
            const auto& subconditions = castedConditions.Subconditions();
            EXPECT_EQ(1, subconditions.size());
            const auto firstCondition = dynamic_cast<const CruisingLevelAbove&>(*subconditions.front());
            EXPECT_EQ(19500, firstCondition.Level());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesACodeWithARoutingViaCondition)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "single_code"}, {"code", "AM"}}},
            {"conditions", nlohmann::json::array({{{"type", "routing_via"}, {"point", "KOK"}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);

        const auto intention = collection->FindById(1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(1, intention->Id());

        try {
            const auto& castedConditions = dynamic_cast<const AllOf&>(intention->Conditions());
            const auto& subconditions = castedConditions.Subconditions();
            EXPECT_EQ(1, subconditions.size());
            const auto firstCondition = dynamic_cast<const RoutingVia&>(*subconditions.front());
            EXPECT_EQ("KOK", firstCondition.Via());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesACodeWithAControllerPositionStartsWithCondition)
    {
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "single_code"}, {"code", "AM"}}},
            {"conditions",
             nlohmann::json::array({{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);

        const auto intention = collection->FindById(1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(1, intention->Id());

        try {
            const auto& castedConditions = dynamic_cast<const AllOf&>(intention->Conditions());
            const auto& subconditions = castedConditions.Subconditions();
            EXPECT_EQ(1, subconditions.size());
            const auto firstCondition = dynamic_cast<const ControllerPositionStartsWith&>(*subconditions.front());
            EXPECT_EQ("SCO", firstCondition.Pattern());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesACodeWithAnAnyOfCondition)
    {
        const auto subconditions =
            nlohmann::json::array({{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}}});
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "single_code"}, {"code", "AM"}}},
            {"conditions", nlohmann::json::array({{{"type", "any_of"}, {"conditions", subconditions}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);

        const auto intention = collection->FindById(1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(1, intention->Id());

        try {
            const auto& castedConditions = dynamic_cast<const AllOf&>(intention->Conditions());
            const auto& subconditions = castedConditions.Subconditions();
            EXPECT_EQ(1, subconditions.size());
            const auto firstCondition = dynamic_cast<const AnyOf&>(*subconditions.front());
            const auto& anyOfConditions = firstCondition.Subconditions();
            EXPECT_EQ(1, anyOfConditions.size());
            const auto subcondition = dynamic_cast<const ControllerPositionStartsWith&>(*anyOfConditions.front());
            EXPECT_EQ("SCO", subcondition.Pattern());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesACodeWithAnAllOfCondition)
    {
        const auto subconditions =
            nlohmann::json::array({{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}}});
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "single_code"}, {"code", "AM"}}},
            {"conditions", nlohmann::json::array({{{"type", "all_of"}, {"conditions", subconditions}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);

        const auto intention = collection->FindById(1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(1, intention->Id());

        try {
            const auto& castedConditions = dynamic_cast<const AllOf&>(intention->Conditions());
            const auto& subconditions = castedConditions.Subconditions();
            EXPECT_EQ(1, subconditions.size());
            const auto firstCondition = dynamic_cast<const AllOf&>(*subconditions.front());
            const auto& allOfConditions = firstCondition.Subconditions();
            EXPECT_EQ(1, allOfConditions.size());
            const auto subcondition = dynamic_cast<const ControllerPositionStartsWith&>(*allOfConditions.front());
            EXPECT_EQ("SCO", subcondition.Pattern());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }

    TEST_F(IntentionCodeCollectionFactoryTest, ItCreatesACodeWithANotCondition)
    {
        const auto subconditions =
            nlohmann::json::array({{{"type", "controller_position_starts_with"}, {"starts_with", "SCO"}}});
        const auto code = nlohmann::json{
            {"id", 1},
            {"code", {{"type", "single_code"}, {"code", "AM"}}},
            {"conditions", nlohmann::json::array({{{"type", "not"}, {"conditions", subconditions}}})}};

        const auto collection = MakeIntentionCodeCollection(nlohmann::json::array({code}), generator, callsigns);

        const auto intention = collection->FindById(1);
        EXPECT_NE(nullptr, intention);
        EXPECT_EQ(1, intention->Id());

        try {
            const auto& castedConditions = dynamic_cast<const AllOf&>(intention->Conditions());
            const auto& subconditions = castedConditions.Subconditions();
            EXPECT_EQ(1, subconditions.size());
            const auto notCondition = dynamic_cast<const Not&>(*subconditions.front());
            const auto notSubcondition = dynamic_cast<const AllOf&>(notCondition.Subcondition());
            const auto& allOfSubconditions = notSubcondition.Subconditions();
            EXPECT_EQ(1, allOfSubconditions.size());
            const auto subcondition = dynamic_cast<const ControllerPositionStartsWith&>(*allOfSubconditions.front());
            EXPECT_EQ("SCO", subcondition.Pattern());
        } catch (std::bad_cast&) {
            FAIL();
        }

        EXPECT_EQ(-1, intention->Metadata().exitPoint);
    }
} // namespace UKControllerPluginTest::IntentionCode
