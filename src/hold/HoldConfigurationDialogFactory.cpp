#pragma once
#include "pch/stdafx.h"
#include "hold/HoldingDataSerializer.h"
#include "hold/HoldConfigurationDialogFactory.h"
#include "hold/HoldConfigurationDialog.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldingData.h"
#include "hold/HoldWindowManager.h"
#include "hold/HoldSelectionMenu.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Create a hold configuration dialog from data
        */
        std::shared_ptr<HoldConfigurationDialog> CreateHoldConfigurationDialog(
            nlohmann::json data,
            HoldWindowManager & windowManager,
            HoldSelectionMenu & holdSelectionMenu,
            HoldProfileManager & holdProfileManager
        ) {
            std::shared_ptr<HoldConfigurationDialog> dialog = std::make_shared<HoldConfigurationDialog>(
                windowManager,
                holdSelectionMenu,
                holdProfileManager
            );

            // If not object, nothing to do
            if (!data.is_array()) {
                LogWarning("Holding data is invalid");
                return dialog;
            }

            // Check valid and add
            for (nlohmann::json::const_iterator it = data.cbegin(); it != data.cend(); ++it) {
                HoldingData data = it->get<HoldingData>();
                if (data == holdSerializerInvalid) {
                    LogWarning("Invalid hold data detected when building hold manager: " + it->dump());
                    continue;
                }

                dialog->AddHold(data);
            }

            LogInfo("Created Hold Manager with " + std::to_string(dialog->CountHolds()) + " holds");
            return dialog;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
