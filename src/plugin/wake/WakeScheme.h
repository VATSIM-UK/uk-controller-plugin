#pragma once

#include <string>
#include <list>
#include "WakeSchemeFactory.h"

namespace UKControllerPlugin::Wake {
    class WakeCategory;

    class WakeScheme
    {
        public:
        WakeScheme(int id, std::string key, std::string name, std::list<std::shared_ptr<WakeCategory>> categories);
        auto Id() const -> int;
        auto Key() const -> const std::string&;
        auto Name() const -> const std::string&;
        auto Categories() const -> const std::list<std::shared_ptr<WakeCategory>>&;

        private:
        int id;

        std::string key;

        std::string name;

        std::list<std::shared_ptr<WakeCategory>> categories;
    };
} // namespace UKControllerPlugin::Wake
