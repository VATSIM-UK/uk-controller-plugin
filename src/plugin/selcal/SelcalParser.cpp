#include "ParsedSelcal.h"
#include "SelcalParser.h"

namespace UKControllerPlugin::Selcal {
    SelcalParser::SelcalParser() : selcalPattern("(\\s+|^)SEL/([ABCDEFGHJKLMPQRS]{4})(\\s+|$)")
    {
    }

    auto SelcalParser::ParseFromString(const std::string& string) const -> std::shared_ptr<ParsedSelcal>
    {
        std::smatch matches;
        std::regex_search(string, matches, selcalPattern);
        if (matches.empty() || DuplicateLetter(matches[2]) || CharactersOutOfOrder(matches[2])) {
            return nullptr;
        }

        return std::make_shared<ParsedSelcal>(matches[2]);
    }

    auto SelcalParser::CharactersOutOfOrder(const std::string& code) -> bool
    {
        return code[0] > code[1] || code[2] > code[3];
    }

    auto SelcalParser::DuplicateLetter(const std::string& code) -> bool
    {
        std::vector<char> letters;
        for (const auto& letter : code) {
            if (std::find(letters.begin(), letters.end(), letter) != letters.cend()) {
                return true;
            }

            letters.emplace_back(letter);
        }

        return false;
    }
} // namespace UKControllerPlugin::Selcal
