#include "ExitDetermination.h"
#include "FirExitPoint.h"

namespace UKControllerPlugin::IntentionCode {
    FirExitPoint::FirExitPoint(
        int id, std::string identifier, bool internal, std::shared_ptr<ExitDetermination> exitDetermination)
        : id(id), identifier(std::move(identifier)), internal(internal), exitDetermination(std::move(exitDetermination))
    {
    }

    auto FirExitPoint::Id() const -> int
    {
        return id;
    }

    auto FirExitPoint::Identifier() const -> const std::string&
    {
        return identifier;
    }

    auto FirExitPoint::IsInternal() const -> bool
    {
        return internal;
    }

    auto FirExitPoint::FirExitDetermination() const -> const ExitDetermination&
    {
        return *exitDetermination;
    }
} // namespace UKControllerPlugin::IntentionCode
