#include "AirfieldModel.h"
#include "controller/ControllerPositionHierarchy.h"

namespace UKControllerPlugin::Airfield {

    AirfieldModel::AirfieldModel(
        int id, std::string icao, std::unique_ptr<Controller::ControllerPositionHierarchy> topDownOrder)
        : id(id), icao(std::move(icao)), topDownOrder(std::move(topDownOrder))
    {
    }

    AirfieldModel::~AirfieldModel() = default;
    AirfieldModel::AirfieldModel(AirfieldModel&&) noexcept = default;
    auto AirfieldModel::operator=(AirfieldModel&&) noexcept -> AirfieldModel& = default;

    auto AirfieldModel::Id() const -> int
    {
        return this->id;
    }

    auto AirfieldModel::Icao() const -> std::string
    {
        return this->icao;
    }

    auto AirfieldModel::TopDownOrder() const -> const Controller::ControllerPositionHierarchy&
    {
        return *this->topDownOrder;
    }
    
    auto AirfieldModel::operator==(const AirfieldModel& compare) const -> bool
    {
        return this->id == compare.id;
    }
} // namespace UKControllerPlugin::Airfield
