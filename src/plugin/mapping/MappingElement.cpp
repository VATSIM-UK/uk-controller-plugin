#include "DisplayRule.h"
#include "MappingElement.h"
#include "MappingElementDrawer.h"

namespace UKControllerPlugin::Mapping {

    MappingElement::MappingElement(
        int id,
        std::string type,
        std::string label,
        EuroScopePlugIn::CPosition position,
        std::set<std::shared_ptr<DisplayRule>> displayRules,
        std::shared_ptr<MappingElementDrawer> drawer)
        : id(id), type(std::move(type)), label(std::move(label)), position(std::move(position)),
          displayRules(std::move(displayRules)), drawer(std::move(drawer))
    {
    }

    auto MappingElement::Id() const -> int
    {
        return id;
    }

    auto MappingElement::Type() const -> const std::string&
    {
        return type;
    }

    auto MappingElement::Label() const -> const std::string&
    {
        return label;
    }

    auto MappingElement::ShouldDisplay() const -> bool
    {
        for (const auto& rule : displayRules) {
            if (!rule->Passes()) {
                return false;
            }
        }

        return true;
    }

    auto MappingElement::Position() const -> const EuroScopePlugIn::CPosition&
    {
        return position;
    }

    void MappingElement::Draw(Windows::GdiGraphicsInterface& graphics, Gdiplus::Rect& area)
    {
        this->drawer->Draw(graphics, area);
    }
} // namespace UKControllerPlugin::Mapping
