#pragma once

namespace UKControllerPlugin::IntentionCode {
    class IntentionCodeModel;

    /*
        Represents some helpful metadata about the intention code for easy access,
        such as FIR exit points.
    */
    using IntentionCodeMetadata = struct IntentionCodeMetadata
    {
        // An associated FIR exit point, if there is one.
        int exitPoint = -1;
    };
} // namespace UKControllerPlugin::IntentionCode
