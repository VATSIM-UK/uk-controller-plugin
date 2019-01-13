#pragma once

namespace UKControllerPlugin {
    namespace Airfield {

        /*
            A class representing a UK airfield. Contains details such as the top-down
            order, as well as who currently owns it.
        */
        class Airfield
        {
            public:
                Airfield(std::string icao, std::vector<std::string> ownership);
                std::string GetIcao(void) const;
                std::vector<std::string> GetOwnershipPresedence(void) const;
                bool operator==(const Airfield & compare);

            private:
                // The airfield ICAO
                std::string icao;

                // The order of who owns this airfield.
                std::vector<std::string> ownershipPresedence;
        };
    }  // namespace Airfield
}  // namespace UKControllerPlugin
