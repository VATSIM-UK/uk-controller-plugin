#pragma once

namespace UKControllerPlugin::Controller {
    class ActiveCallsignCollection;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::IntentionCode {
    class AircraftFirExitGenerator;
    class CodeGenerator;
    class Condition;
    class IntentionCodeCollection;
    class IntentionCodeMetadata;

    [[nodiscard]] auto MakeIntentionCodeCollection(
        const nlohmann::json& codes,
        AircraftFirExitGenerator& generator,
        const Controller::ActiveCallsignCollection& activeControllers) -> std::unique_ptr<IntentionCodeCollection>;
    [[nodiscard]] auto MakeCode(const nlohmann::json& code) -> std::unique_ptr<CodeGenerator>;
    [[nodiscard]] auto MakeCondition(
        const nlohmann::json& conditions,
        AircraftFirExitGenerator& generator,
        const Controller::ActiveCallsignCollection& activeControllers,
        IntentionCodeMetadata& metadata) -> std::shared_ptr<Condition>;
    [[nodiscard]] auto MakeConditions(
        const nlohmann::json& condition,
        AircraftFirExitGenerator& generator,
        const Controller::ActiveCallsignCollection& activeControllers,
        IntentionCodeMetadata& metadata) -> std::list<std::shared_ptr<Condition>>;
    [[nodiscard]] auto ConditionsValid(const nlohmann::json& conditions) -> bool;
    [[nodiscard]] auto ConditionValid(const nlohmann::json& condition) -> bool;
    [[nodiscard]] auto CodeValid(const nlohmann::json& code) -> bool;
    [[nodiscard]] auto IntentionCodeValid(const nlohmann::json& code) -> bool;
} // namespace UKControllerPlugin::IntentionCode
