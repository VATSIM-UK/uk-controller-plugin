#pragma once
#include "minstack/TerminalControlArea.h"

namespace UKControllerPluginTest {
    namespace MinStack {
        class MockTerminalControlArea : public UKControllerPlugin::MinStack::TerminalControlArea
        {
        public:
            MockTerminalControlArea(
                LPCWSTR name,
                const char* charName,
                int transition,
                const char * calculationAirfield,
                bool standardPressureHigh
            ) : TerminalControlArea(name, charName, transition, calculationAirfield, standardPressureHigh) {}
            MOCK_CONST_METHOD0(MinStackAcknowledged, bool(void));
            MOCK_CONST_METHOD0(GetCurrentMinStackDisplay, LPCWSTR(void));
            MOCK_CONST_METHOD0(GetName, LPCWSTR(void));
            MOCK_CONST_METHOD0(GetCharName, const char*(void));
            MOCK_METHOD0(AcknowledgeMinStack, void(void));
            MOCK_METHOD1(SetMinStack, bool(int));
        };
    }  // namespace MinStack
}  // namespace UKControllerPluginTest
