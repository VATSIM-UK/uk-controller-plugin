#include "pch/stdafx.h"
#include "minstack/TerminalControlArea.h"


namespace UKControllerPlugin {
    namespace MinStack {

        TerminalControlArea::TerminalControlArea(
            LPCWSTR name,
            std::string charName,
            int transition,
            std::string calculationAirfield,
            bool standardPressureHigh
        ) {
            this->name = name;
            this->charName = charName;
            this->transition = transition;
            this->calculationAirfield = calculationAirfield;
            this->currentMinStackInt = 0;
            this->userAcknowledged = false;
            this->standardHighPressure = standardPressureHigh;
        }

        TerminalControlArea::~TerminalControlArea(void)
        {

        }

        /*
            The user has acknowledged the current minimum stack level.
        */
        void TerminalControlArea::AcknowledgeMinStack(void)
        {
            this->userAcknowledged = true;
        }

        /*
            The name of the calculation airfield.
        */
        std::string TerminalControlArea::GetCalculationAirfield(void) const
        {
            return this->calculationAirfield;
        }

        /*
            Returns the "Name" of the TMA, but this time
            as a char *.
        */
        const char * TerminalControlArea::GetCharName(void) const
        {
            return this->charName.c_str();
        }

        /*
            Returns the Minimum Stack Level as a LPCWSTR. This is required
            to be displayed in the EuroScope window.
        */
        LPCWSTR TerminalControlArea::GetCurrentMinStackDisplay(void) const
        {
            if (this->currentMinStack == NULL) {
                return L"-";
            }

            return currentMinStack;
        }

        /*
            Returns the Minimum Stack Level as an integer value.
        */
        int TerminalControlArea::GetCurrentMinStackInt(void) const
        {
            return currentMinStackInt;
        }

        /*
            Returns the "Name" of the TMA.
        */
        LPCWSTR TerminalControlArea::GetName(void) const
        {
            return this->name;
        }

        /*
            Returns the transition altitude of the TMA.
        */
        int TerminalControlArea::GetTransition(void) const
        {
            return this->transition;
        }


        /*
            Returns whether or not the user has acknowledged the current minimum stack level.
        */
        bool TerminalControlArea::MinStackAcknowledged(void) const
        {
            return this->userAcknowledged;
        }

        /*
        Given a particular QNH, calculate the minimum stack level.
        */
        bool TerminalControlArea::SetMinStack(int qnh)
        {

            // Get the TMA that the airfield calculates the MSL for.
            int altitude;

            // Calculate the MSL in thousands.
            if (qnh > 1013 || (qnh == 1013 && this->standardHighPressure)) {
                altitude = this->GetTransition() + 1000;
            } else if (qnh >= 978) {
                altitude = this->GetTransition() + 2000;
            } else {
                altitude = this->GetTransition() + 3000;
            }

            // Critical region, lock the mutex
            this->updateMutex.lock();
            // Work out whether or not there's a new minimum stack level. We divide by 100 to give the 2 digit variant.
            bool newMinStack = this->currentMinStackInt != (altitude / 100);

            // Set the integer and LPCWSTR variables
            switch (altitude / 100) {
                case 60:
                    this->currentMinStack = L"60";
                    this->currentMinStackInt = 60;
                    break;
                case 70:
                    this->currentMinStack = L"70";
                    this->currentMinStackInt = 70;
                    break;
                case 80:
                    this->currentMinStack = L"80";
                    this->currentMinStackInt = 80;
                    break;
                case 90:
                    this->currentMinStack = L"90";
                    this->currentMinStackInt = 90;
                    break;
            }

            if (newMinStack) {
                this->userAcknowledged = false;
            }

            // End of critical region
            this->updateMutex.unlock();
            return newMinStack;
        }

        /*
            Returns whether or not standard pressure is considered to be
            high pressure.
        */
        bool TerminalControlArea::StandardPressureHigh(void)
        {
            return this->standardHighPressure;
        }
    }  // namespace MinStack
}  // namespace UKControllerPlugin
