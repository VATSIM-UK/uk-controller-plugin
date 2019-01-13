#include "pch/stdafx.h"
#include "euroscope/CompareFunctions.h"
#include "euroscope/CallbackFunction.h"
#include "tag/TagFunction.h"

using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin {
    namespace Euroscope {

        bool CompareFunctions::operator()(const CallbackFunction & callback, int id) const
        {
            return callback.functionId < id;
        }

        bool CompareFunctions::operator()(int id, const CallbackFunction & callback) const
        {
            return id < callback.functionId;
        }

        bool CompareFunctions::operator()(const CallbackFunction & a, const CallbackFunction & b)
        {
            return a.functionId < b.functionId;
        }

        bool CompareFunctions::operator()(const TagFunction & callback, int id) const
        {
            return callback.functionId < id;
        }

        bool CompareFunctions::operator()(int id, const TagFunction & callback) const
        {
            return id < callback.functionId;
        }

        bool CompareFunctions::operator()(const TagFunction & a, const TagFunction & b)
        {
            return a.functionId < b.functionId;
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
