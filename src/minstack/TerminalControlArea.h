#pragma once

namespace UKControllerPlugin {
    namespace MinStack {
        /*
            TerminalControlArea class

            Stores information about a Terminal Control Area (TMA)
            which can then be used in combination with a new QNH
            to calculate the MSL in the TMA. Also stores the MSL value.

            The class can export a vector of Structs, which provide data
            to classes that may wish to render the data to the screen.
        */
        class TerminalControlArea
        {
        public:
            TerminalControlArea(
                LPCWSTR name,
                std::string charName,
                int transition,
                std::string calculationAirfield,
                bool statndardPressureHigh
            );
            ~TerminalControlArea(void);
            virtual void AcknowledgeMinStack(void);
            std::string  GetCalculationAirfield(void) const;
            virtual const char * GetCharName(void) const;
            virtual LPCWSTR GetCurrentMinStackDisplay(void) const;
            int GetCurrentMinStackInt(void) const;
            virtual LPCWSTR GetName(void) const;
            int GetTransition(void) const;
            virtual bool MinStackAcknowledged(void) const;
            virtual bool SetMinStack(int qnh);
        private:
            bool StandardPressureHigh(void);

            // The name of the TMA as a LPCWSTR
            LPCWSTR name;

            // The name of the TMA as a const char *
            std::string charName;

            // The airfield used to calculate MSL in this TMA.
            std::string calculationAirfield;

            // The transition altitude
            int transition;

            // The current minimum stack level as a EuroScope displayable value
            LPCWSTR currentMinStack;

            // The current minimum stack level as an integer value
            int currentMinStackInt;

            // Is there a new Min Stack that the user hasn't acknowledged?
            bool userAcknowledged;

            // Is QNH 1013 considered high pressure or low pressure
            bool standardHighPressure;

            // A mutex to ensure that new Min Stacks get written as a whole
            std::mutex updateMutex;
        };
    }  // namespace MinStack
}  // namespace UKControllerPlugin
