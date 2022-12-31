
#pragma once

namespace UKControllerPlugin::IntentionCode {

    class CodeGenerator;
    class Condition;

    /**
     * Represents a single intention code from the API.
     */
    class IntentionCodeModel
    {
        public:
        IntentionCodeModel(int id, std::unique_ptr<CodeGenerator> codeGenerator, std::unique_ptr<Condition> condition);
        ~IntentionCodeModel();
        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto Generator() const -> const CodeGenerator&;
        [[nodiscard]] auto Conditions() const -> const Condition&;

        private:
        // The API id of the code
        int id;
        // Generates the code
        std::unique_ptr<CodeGenerator> codeGenerator;
        // The condition that must be met for this code to apply
        std::unique_ptr<Condition> condition;
    };

} // namespace UKControllerPlugin::IntentionCode
