#include "AirfieldIdentifier.h"
#include "AllOf.h"
#include "AnyOf.h"
#include "ArrivalAirfieldPattern.h"
#include "ArrivalAirfields.h"
#include "Condition.h"
#include "ControllerPositionStartsWith.h"
#include "CruisingLevelAbove.h"
#include "ExitingFirAtPoint.h"
#include "FullAirfieldIdentifier.h"
#include "IntentionCodeCollection.h"
#include "IntentionCodeCollectionFactory.h"
#include "IntentionCodeMetadata.h"
#include "IntentionCodeModel.h"
#include "MaximumCruisingAltitude.h"
#include "Not.h"
#include "RoutingVia.h"
#include "SingleCode.h"

namespace UKControllerPlugin::IntentionCode {

    auto ConditionsValid(const nlohmann::json& conditions) -> bool
    {
        if (!conditions.is_array()) {
            return false;
        }

        for (const auto& condition : conditions) {
            if (!ConditionValid(condition)) {
                return false;
            }
        }

        return true;
    }

    auto ConditionValid(const nlohmann::json& condition) -> bool
    {
        if (!condition.is_object() || !condition.contains("type") || !condition.at("type").is_string()) {
            return false;
        }

        const auto conditionType = condition.at("type").get<std::string>();
        if (conditionType == "arrival_airfields") {
            // Airfield conditions must be an array of airfield ICAO codes
            return condition.contains("airfields") && condition.at("airfields").is_array() &&
                   std::find_if_not(
                       condition.at("airfields").cbegin(),
                       condition.at("airfields").cend(),
                       [](const nlohmann::json& airfield) { return airfield.is_string(); }) ==
                       condition.at("airfields").cend();
        }

        if (conditionType == "arrival_airfield_pattern") {
            return condition.contains("pattern") && condition.at("pattern").is_string();
        }

        if (conditionType == "exit_point") {
            return condition.contains("exit_point") && condition.at("exit_point").is_number_integer();
        }

        if (conditionType == "maximum_cruising_level") {
            return condition.contains("level") && condition.at("level").is_number_integer();
        }

        if (conditionType == "cruising_level_above") {
            return condition.contains("level") && condition.at("level").is_number_integer();
        }

        if (conditionType == "routing_via") {
            return condition.contains("point") && condition.at("point").is_string();
        }

        if (conditionType == "controller_position_starts_with") {
            return condition.contains("starts_with") && condition.at("starts_with").is_string();
        }

        if (conditionType == "not" || conditionType == "any_of" || conditionType == "all_of") {
            return condition.contains("conditions") && ConditionsValid(condition.at("conditions"));
        }

        return false;
    }

    auto CodeValid(const nlohmann::json& code) -> bool
    {
        if (!code.is_object() || !code.contains("type") || !code.at("type").is_string()) {
            return false;
        }

        const auto codeType = code.at("type").get<std::string>();
        if (codeType == "airfield_identifier") {
            return true;
        }

        return codeType == "single_code" && code.contains("code") && code.at("code").is_string();
    }

    auto IntentionCodeValid(const nlohmann::json& code) -> bool
    {
        return code.is_object() && code.contains("id") && code.at("id").is_number_integer() && code.contains("code") &&
               CodeValid(code.at("code")) && code.contains("conditions") && ConditionsValid(code.at("conditions"));
    }

    auto MakeIntentionCodeCollection(
        const nlohmann::json& codes,
        std::shared_ptr<AircraftFirExitGenerator> generator,
        std::shared_ptr<const Controller::ActiveCallsignCollection> activeControllers)
        -> std::shared_ptr<IntentionCodeCollection>
    {
        auto collection = std::make_shared<IntentionCodeCollection>();

        if (!codes.is_array()) {
            LogWarning("Intention codes dependency is invalid");
            // Add the fallback - full airfield ICAO
            collection->Add(std::make_shared<IntentionCodeModel>(
                -1,
                std::make_unique<FullAirfieldIdentifier>(),
                std::make_unique<AllOf>(std::list<std::shared_ptr<Condition>>({})),
                std::unique_ptr<IntentionCodeMetadata>(new IntentionCodeMetadata)));
            return collection;
        }

        for (const auto& code : codes) {
            if (!IntentionCodeValid(code)) {
                LogWarning("Intention code invalid: " + code.dump());
                continue;
            }

            auto metadata = std::unique_ptr<IntentionCodeMetadata>(new IntentionCodeMetadata);
            collection->Add(std::make_shared<IntentionCodeModel>(
                code.at("id").get<int>(),
                MakeCode(code.at("code")),
                std::make_unique<AllOf>(MakeConditions(code.at("conditions"), generator, activeControllers, *metadata))

                    ,
                std::move(metadata)));
        }

        // Add full airfield icao fallback
        collection->Add(std::make_shared<IntentionCodeModel>(
            -1,
            std::make_unique<FullAirfieldIdentifier>(),
            std::make_unique<AllOf>(std::list<std::shared_ptr<Condition>>({})),
            std::unique_ptr<IntentionCodeMetadata>(new IntentionCodeMetadata)));

        LogInfo("Loaded " + std::to_string(collection->Count()) + " intention codes");
        return collection;
    }

    auto MakeCode(const nlohmann::json& code) -> std::unique_ptr<CodeGenerator>
    {
        if (code.at("type").get<std::string>() == "airfield_identifier") {
            return std::make_unique<AirfieldIdentifier>();
        }

        return std::make_unique<SingleCode>(code.at("code").get<std::string>());
    }

    auto MakeConditions(
        const nlohmann::json& conditions,
        std::shared_ptr<AircraftFirExitGenerator> generator,
        std::shared_ptr<const Controller::ActiveCallsignCollection> activeControllers,
        IntentionCodeMetadata& metadata) -> std::list<std::shared_ptr<Condition>>
    {
        auto conditionList = std::list<std::shared_ptr<Condition>>();
        for (const auto& condition : conditions) {
            conditionList.push_back(MakeCondition(condition, generator, activeControllers, metadata));
        }

        return conditionList;
    }

    auto MakeCondition(
        const nlohmann::json& condition,
        std::shared_ptr<AircraftFirExitGenerator> generator,
        std::shared_ptr<const Controller::ActiveCallsignCollection> activeControllers,
        IntentionCodeMetadata& metadata) -> std::shared_ptr<Condition>
    {
        const auto conditionType = condition.at("type").get<std::string>();
        if (conditionType == "arrival_airfields") {
            return std::make_shared<ArrivalAirfields>(condition.at("airfields"));
        }

        if (conditionType == "arrival_airfield_pattern") {
            return std::make_shared<ArrivalAirfieldPattern>(condition.at("pattern"));
        }

        if (conditionType == "exit_point") {
            metadata.exitPoint = condition.at("exit_point");
            return std::make_shared<ExitingFirAtPoint>(generator, condition.at("exit_point"));
        }

        if (conditionType == "maximum_cruising_level") {
            return std::make_shared<MaximumCruisingAltitude>(condition.at("level"));
        }

        if (conditionType == "cruising_level_above") {
            return std::make_shared<CruisingLevelAbove>(condition.at("level"));
        }

        if (conditionType == "routing_via") {
            return std::make_shared<RoutingVia>(condition.at("point"));
        }

        if (conditionType == "controller_position_starts_with") {
            return std::make_shared<ControllerPositionStartsWith>(activeControllers, condition.at("starts_with"));
        }

        if (conditionType == "any_of") {
            return std::make_shared<AnyOf>(
                MakeConditions(condition.at("conditions"), generator, activeControllers, metadata));
        }

        if (conditionType == "all_of") {
            return std::make_shared<AllOf>(
                MakeConditions(condition.at("conditions"), generator, activeControllers, metadata));
        }

        // Not is a bit special - its implicitly an inversion of AllOf
        return std::make_shared<Not>(std::make_shared<AllOf>(
            MakeConditions(condition.at("conditions"), generator, activeControllers, metadata)));
    }
} // namespace UKControllerPlugin::IntentionCode
