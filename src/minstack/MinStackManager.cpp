#include "pch/stdafx.h"
#include "minstack/MinStackManager.h"
#include "minstack/TerminalControlArea.h"
#include "helper/HelperFunctions.h"
#include "task/TaskRunner.h"

using UKControllerPlugin::TaskManager::TaskRunnerInterface;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace MinStack {

        /*
            Adds a TMA to the list of TMAs.
        */
        void MinStackManager::AddTerminalControlArea(std::shared_ptr<TerminalControlArea> tma)
        {
            // Add it to the list
            if (this->HasTerminalControlArea(tma->GetCharName())) {
                return;
            }

            this->tmaList[tma->GetCharName()] = tma;
            this->tmaRenderingList.push_back(tma);
        }

        /*
            Returns a vector of pointers to the constant versions of the TMAs.
        */
        std::vector<std::shared_ptr<const TerminalControlArea>> MinStackManager::GetAllTmas(void)
        {
            return this->tmaRenderingList;
        }


        /*
            Returns a pointer to the TMA that is related to the given airfield
        */
        std::shared_ptr<TerminalControlArea> MinStackManager::GetTerminalControlAreaForAirfield(std::string airfield)
        {
            for (
                std::map<std::string, std::shared_ptr<TerminalControlArea>>::iterator it = this->tmaList.begin();
                it != this->tmaList.end();
                ++it
            ) {
                if (it->second->GetCalculationAirfield().compare(airfield) == 0) {
                    return it->second;
                }
            }

            return NULL;
        }

        /*
            Returns true if the TMA is known to the class.
        */
        bool MinStackManager::HasTerminalControlArea(std::string tma)
        {
            return this->tmaList.count(tma) == 1;
        }

        /*
            Returns true if one of the TMAs managed by this
            class has it's MSL determined by the given airfield.
        */
        bool MinStackManager::IsConcernedAirfield(std::string airfield)
        {
            for (
                std::map<std::string, std::shared_ptr<TerminalControlArea>>::iterator it = this->tmaList.begin();
                it != this->tmaList.end();
                ++it
            ) {
                if (it->second->GetCalculationAirfield().compare(airfield) == 0) {
                    return true;
                }
            }

            return false;
        }

        /*
            The user has clicked to acknowledge one of the
            minimum stack levels, set the TMA to acknowledged.
        */
        void MinStackManager::MinStackClicked(const char* tma)
        {
            if (!this->HasTerminalControlArea(tma)) {
                return;
            }

            this->tmaList.at(tma)->AcknowledgeMinStack();
        }

        /*
            A new QNH has been detected, so we check the TMA exists
            then set the new MSL.
        */
        void MinStackManager::NewMetar(std::string station, std::string metar)
        {
            if (!this->IsConcernedAirfield(station)) {
                return;
            }

            int qnh = this->ProcessMetar(metar);
            if (qnh) {
                GetTerminalControlAreaForAirfield(station)->SetMinStack(qnh);
            }
        }

        /*
            When we receive a new metar, process it in tokens until we get the QNH.
        */
        int MinStackManager::ProcessMetar(std::string metar)
        {
            std::istringstream buf(metar);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> tokens(beg, end);

            for each (std::string token in tokens)
            {
                if (token[0] == 'Q' && HelperFunctions::IsAnInteger(token.substr(1))) {
                    return std::stoi(token.substr(1));
                }
            }

            return 0;
        }

        /*
            Removes a given TMA from the MinStack module.
        */
        void MinStackManager::RemoveTerminalControlArea(std::string tma)
        {
            if (this->HasTerminalControlArea(tma)) {
                return;
            }

            this->tmaList.erase(tmaList.find(tma));
        }
    }  // namespace MinStack
}  // namespace UKControllerPlugin
