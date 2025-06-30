#include "AbstractHoldLevelRestriction.h"
#include "DeemedSeparatedHold.h"
#include "HoldDisplay.h"
#include "HoldDisplayFunctions.h"
#include "HoldManager.h"
#include "HoldModule.h"
#include "HoldingAircraft.h"
#include "HoldingData.h"
#include "ProximityHold.h"
#include "PublishedHoldCollection.h"
#include "dialog/DialogManager.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "euroscope/UserSetting.h"
#include "geometry/Measurement.h"
#include "geometry/MeasurementUnit.h"
#include "graphics/GdiGraphicsInterface.h"
#include "list/PopupListInterface.h"
#include "navaids/Navaid.h"
#include "graphics/GdiplusBrushes.h"

using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPlugin {
    namespace Hold {
        HoldDisplay::HoldDisplay(
            EuroscopePluginLoopbackInterface& plugin,
            HoldManager& holdManager,
            const Navaids::Navaid& navaid,
            const PublishedHoldCollection& publishedHoldCollection,
            const DialogManager& dialogManager,
            const GdiplusBrushes& brushes,
            std::shared_ptr<List::PopupListInterface> addAircraftSelector)
            : navaid(navaid), publishedHolds(publishedHoldCollection.GetForFix(navaid.identifier)),
              holdManager(holdManager), plugin(plugin), dialogManager(dialogManager),
              publishedHoldCollection(publishedHoldCollection), addAircraftSelector(addAircraftSelector), 
              verticalSpeedAscentPen(Gdiplus::Color(7, 237, 7), 2.5f), verticalSpeedDescentPen(Gdiplus::Color(7, 237, 7), 2.5f), 
              brushes(brushes), fontFamily(L"EuroScope"),
              font(&fontFamily, 12, Gdiplus::FontStyleBold, Gdiplus::UnitPixel),
              plusFont(&fontFamily, 18, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel),
              stringFormat(Gdiplus::StringFormatFlags::StringFormatFlagsNoClip), dataStartHeight(0),
              minimiseClickRect({}), informationClickRect({})
        {
            verticalSpeedAscentPen.SetStartCap(Gdiplus::LineCapArrowAnchor);
            verticalSpeedDescentPen.SetEndCap(Gdiplus::LineCapArrowAnchor);

            this->stringFormat.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
            this->stringFormat.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);

            this->Move(this->windowPos);
        }

        /*
            A button has been clicked
        */
        void HoldDisplay::ButtonClicked(std::string button)
        {
            if (button == "plus") {
                this->maximumLevel += 1000;
            } else if (button == "minus") {
                if (this->maximumLevel == this->minimumLevel) {
                    return;
                }

                this->maximumLevel -= 1000;
            } else if (button == "allLevels") {
                this->minimumLevel = this->publishedHolds.size() ? (*this->publishedHolds.cbegin())->minimum : 7000;
                this->maximumLevel = this->publishedHolds.size() ? (*this->publishedHolds.cbegin())->maximum : 15000;
            } else if (button == "add") {
                std::shared_ptr<EuroScopeCFlightPlanInterface> fp = this->plugin.GetSelectedFlightplan();

                if (!fp) {
                    LogWarning("Tried to add aircraft to hold " + this->navaid.identifier + " but none selected");
                    return;
                }

                if (!fp->IsTrackedByUser()) {
                    LogInfo(
                        "Tried to add aircraft " + fp->GetCallsign() + " to hold but it is not tracked by the user");
                    return;
                }

                this->holdManager.AssignAircraftToHold(fp->GetCallsign(), this->navaid.identifier, true);
            } else if (button == "minimise") {
                this->minimised = !this->minimised;
            } else if (button == "information") {
                this->showHoldInformation = !this->showHoldInformation;
            } else if (button == "options") {
                this->dialogManager.OpenDialog(IDD_HOLD_PARAMS, reinterpret_cast<LPARAM>(this));
            } else if (button == "prevhold") {
                if (this->selectedPublishedHoldIndex == 0) {
                    return;
                }

                this->selectedPublishedHoldIndex--;
            } else if (button == "nexthold") {
                if (this->publishedHolds.empty() ||
                    this->selectedPublishedHoldIndex == this->publishedHolds.size() - 1) {
                    return;
                }

                this->selectedPublishedHoldIndex++;
            }
        }

        void HoldDisplay::CallsignClicked(
            std::string callsign, EuroscopeRadarLoopbackInterface& radarScreen, POINT mousePos, RECT area)
        {
            radarScreen.TogglePluginTagFunction(callsign, popupMenuTagItemId, mousePos, area);
        }

        void HoldDisplay::ClearedLevelClicked(
            std::string callsign, EuroscopeRadarLoopbackInterface& radarScreen, POINT mousePos, RECT area)
        {
            radarScreen.ToggleTemporaryAltitudePopupList(callsign, mousePos, area);
        }

        void HoldDisplay::ButtonRightClicked(const std::string& button)
        {
            if (button == "add") {
                addAircraftSelector->Trigger({this->addButtonClickRect.left, this->addButtonClickRect.top});
            }
        }

        unsigned int HoldDisplay::GetPublishedHoldIndex() const
        {
            return this->selectedPublishedHoldIndex;
        }

        /*
            Get the rectangle to render the background of the hold display
        */
        Gdiplus::Rect HoldDisplay::GetHoldViewBackgroundRender(
            const std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>>& aircraft) const
        {
            Gdiplus::Rect area{
                this->windowPos.x,
                this->windowPos.y,
                this->windowWidth,
                this->dataStartOffset + ((((this->maximumLevel - this->minimumLevel) / 1000) + 2) * this->lineHeight)};

            for (std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>>::const_iterator it =
                     aircraft.cbegin();
                 it != aircraft.cend();
                 ++it) {
                area.Height += (it->second.size() - 1) * this->lineHeight;
            }

            return area;
        }

        /*
            Get the data start height
        */
        INT HoldDisplay::GetDataStartHeight(void) const
        {
            return this->dataStartHeight;
        }

        /*
            Return the position of the window
        */
        POINT HoldDisplay::GetDisplayPos(void) const
        {
            return this->windowPos;
        }

        /*
            Return the renderable area for the title bar
        */
        Gdiplus::Rect HoldDisplay::GetTitleArea(void) const
        {
            return this->titleArea;
        }

        /*
            Return the click area for the title bar
        */
        RECT HoldDisplay::GetTitleClickArea(void) const
        {
            return this->titleRect;
        }

        /*
            Get renderable area for the minimise button
        */
        Gdiplus::Rect HoldDisplay::GetMinimiseArea(void) const
        {
            return this->minimiseButtonArea;
        }

        /*
            Get clickable area for the minimise button
        */
        RECT HoldDisplay::GetMinimiseClickArea(void) const
        {
            return this->minimiseClickRect;
        }

        /*
            Return the renderable area for the information button
        */
        Gdiplus::Rect HoldDisplay::GetInformationArea(void) const
        {
            return this->informationButtonArea;
        }

        /*
            Return the clickable area for the information button
        */
        RECT HoldDisplay::GetInformationClickArea(void) const
        {
            return this->informationClickRect;
        }

        Gdiplus::Rect HoldDisplay::GetOptionsArea(void) const
        {
            return this->optionsButtonArea;
        }

        RECT HoldDisplay::GetOptionsClickArea(void) const
        {
            return this->optionsClickRect;
        }

        /*
            Return the renderable area for the plus button
        */
        Gdiplus::Rect HoldDisplay::GetPlusArea(void) const
        {
            return this->plusButtonRect;
        }

        /*
            Return the clickable area for the plus button
        */
        RECT HoldDisplay::GetPlusClickArea(void) const
        {
            return this->plusButtonClickRect;
        }

        /*
            Return the renderable area for the minus button
        */
        Gdiplus::Rect HoldDisplay::GetMinusArea(void) const
        {
            return this->minusButtonRect;
        }

        /*
            Return the clickable area for the minus button
        */
        RECT HoldDisplay::GetMinusClickArea(void) const
        {
            return this->minusButtonClickRect;
        }

        /*
            Return the renderable area for the all levels button
        */
        Gdiplus::Rect HoldDisplay::GetAllArea(void) const
        {
            return this->allButtonRect;
        }

        /*
            Return the clickable area for the all levels button
        */
        RECT HoldDisplay::GetAllClickArea(void) const
        {
            return this->allButtonClickRect;
        }

        /*
            Return the renderable area for the add button
        */
        Gdiplus::Rect HoldDisplay::GetAddArea(void) const
        {
            return this->addButtonRect;
        }

        /*
            Return the clickable area for the add button
        */
        RECT HoldDisplay::GetAddClickArea(void) const
        {
            return this->addButtonClickRect;
        }

        Gdiplus::Point HoldDisplay::GetUnderButtonLineLeft(void) const
        {
            return this->underButtonLineLeft;
        }

        Gdiplus::Point HoldDisplay::GetUnderButtonLineRight(void) const
        {
            return this->underButtonLineRight;
        }

        int HoldDisplay::GetMaximumLevel(void) const
        {
            return this->maximumLevel;
        }

        int HoldDisplay::GetMinimumLevel(void) const
        {
            return this->minimumLevel;
        }

        void HoldDisplay::SetMaximumLevel(int level)
        {
            this->maximumLevel = level;
        }

        void HoldDisplay::SetMinimumLevel(int level)
        {
            this->minimumLevel = level;
        }

        /*
         * Filter aircraft out of holding levels for the VSL and remove any levels that
         * we don't want to keep.
         */
        void HoldDisplay::FilterVslDisplayLevels(
            std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>>& levelMap) const
        {
            for (auto level = levelMap.begin(); level != levelMap.end();) {
                // Filter out aircraft at each level that we don't need to keep
                this->FilterAircraftAtLevel(level->first, level->second);

                // Filter out levels that have nothing we need to display
                if (this->ShouldFilterVslLevel(level->second)) {
                    level = levelMap.erase(level);
                } else {
                    ++level;
                }
            }
        }

        /*
         * Determine whether or not the VSL level should be filtered out
         */
        bool HoldDisplay::ShouldFilterVslLevel(
            const std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>& holdingAircraft) const
        {
            return this->NoAircraftAssignedToHold(holdingAircraft);
        }

        /*
         * Returns true if no aircraft in the list are assigned to the
         * holding aircraft.
         */
        bool HoldDisplay::NoAircraftAssignedToHold(
            const std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>& holdingAircraft) const
        {
            return std::find_if(
                       holdingAircraft.cbegin(),
                       holdingAircraft.cend(),
                       [this](const std::shared_ptr<HoldingAircraft>& aircraft) -> bool {
                           return this->AircraftAssignedToHold(aircraft);
                       }) == holdingAircraft.cend();
        }

        /*
         * Filters out aircraft at a given holding level that don't meeting the criteria to be
         * kept in the VSL.
         */
        void HoldDisplay::FilterAircraftAtLevel(
            int level, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>& holdingAircraft) const
        {
            /*
             * Filter out aircraft that are:
             *
             * - Not assigned to the hold (thus are just in proximity) AND
             * - Are assigned to a deemed separated hold AND
             * - Are within the limits of the assigned deemed separated hold AND
             * - Are not in direct conflict with an aircraft assigned to this hold
             */
            for (auto aircraftIt = holdingAircraft.begin(); aircraftIt != holdingAircraft.end();) {
                const std::shared_ptr<HoldingAircraft> aircraft = *aircraftIt;

                /*
                 * Filter out the aircraft where:
                 *
                 * 1. The aircraft is not assigned to hold at this navaid
                 * 2. The aircraft is assigned to hold at a different navaid
                 * 3. The aircraft is within a published hold that is deemed separated
                 * from all the aircraft holding within published holds at this navaid.
                 */
                if (!this->AircraftAssignedToHold(aircraft) &&
                    aircraft->GetAssignedHold() != aircraft->GetNoHoldAssigned() &&
                    this->AircraftInDeemedSeparatedHold(level, aircraft, holdingAircraft)) {
                    aircraftIt = holdingAircraft.erase(aircraftIt);
                } else {
                    ++aircraftIt;
                }
            }
        }

        /*
         * Checks whether the aircraft in question is within a deemed separated hold
         * and not conflicting with another aircraft.
         */
        bool HoldDisplay::AircraftInDeemedSeparatedHold(
            int level,
            const std::shared_ptr<HoldingAircraft>& aircraft,
            const std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>& aircraftAtLevel) const
        {
            // Make sure there's a radar target for the aircraft we're checking.
            std::shared_ptr<EuroScopeCRadarTargetInterface> aircraftRadarTarget =
                this->plugin.GetRadarTargetForCallsign(aircraft->GetCallsign());

            if (!aircraftRadarTarget) {
                return false;
            }

            // Iterate each aircraft at the level
            return std::find_if(
                       aircraftAtLevel.cbegin(),
                       aircraftAtLevel.cend(),
                       [&level, &aircraftRadarTarget, &aircraft, this](
                           const std::shared_ptr<HoldingAircraft>& conflictingAircraft) -> bool {
                           std::shared_ptr<EuroScopeCRadarTargetInterface> conflictingRadarTarget =
                               this->plugin.GetRadarTargetForCallsign(conflictingAircraft->GetCallsign());

                           /*
                            * 1. Check that the conflicting aircraft has a radar target
                            * 2. Check that the conflicting aircraft is actually assigned to hold here
                            * 3. Check each of the published holds at this fix
                            */
                           return conflictingRadarTarget != nullptr &&
                                  this->AircraftAssignedToHold(conflictingAircraft) &&
                                  std::find_if(
                                      this->publishedHolds.cbegin(),
                                      this->publishedHolds.cend(),
                                      [this, &level, &conflictingRadarTarget, &aircraft, &aircraftRadarTarget](
                                          const HoldingData* const publishedHold) -> bool {
                                          /*
                                           * 3a. Make sure that the level the aircraft are at is within the published
                                           * hold 3b. Check each of the deemed separated holds for this published hold
                                           */
                                          return publishedHold->LevelWithinHold(level) &&
                                                 std::find_if(
                                                     publishedHold->deemedSeparatedHolds.cbegin(),
                                                     publishedHold->deemedSeparatedHolds.cend(),
                                                     [this,
                                                      &level,
                                                      &aircraft,
                                                      &aircraftRadarTarget,
                                                      &conflictingRadarTarget](
                                                         const std::unique_ptr<DeemedSeparatedHold>&
                                                             deemedSeparatedHold) -> bool {
                                                         const HoldingData& publishedSeparatedHold =
                                                             this->publishedHoldCollection.GetById(
                                                                 deemedSeparatedHold->identifier);

                                                         /*
                                                          * 3bi. Check the deemed separated hold is published
                                                          * 3bii. Check if the level is within the deemed separated hold
                                                          * 3biii. Check if the aircraft that we're interested in is
                                                          * assigned to the deemed separated hold 3biv. Check that the
                                                          * distance between the two aircraft is greater than the force
                                                          * VSL insert distance.
                                                          */
                                                         return publishedSeparatedHold !=
                                                                    this->publishedHoldCollection.noHold &&
                                                                publishedSeparatedHold.LevelWithinHold(level) &&
                                                                aircraft->GetAssignedHold() ==
                                                                    publishedSeparatedHold.fix &&
                                                                aircraftRadarTarget->GetPosition().DistanceTo(
                                                                    conflictingRadarTarget->GetPosition()) >
                                                                    deemedSeparatedHold->vslInsertDistance;
                                                     }) != publishedHold->deemedSeparatedHolds.cend();
                                      }) != publishedHolds.cend();
                       }) != aircraftAtLevel.cend();
        }

        /*
         * Returns whether the given aircraft has been assigned to the hold.
         */
        bool HoldDisplay::AircraftAssignedToHold(const std::shared_ptr<HoldingAircraft>& aircraft) const
        {
            return aircraft->GetAssignedHold() == this->navaid.identifier;
        }

        /*
            Maps the holding aircraft to their occupied levels and filters out ones we dont
            want to display in the VSL
        */
        std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>>
        HoldDisplay::MapAircraftToLevels(
            const std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>& aircraft) const
        {
            std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> levelMap;
            std::shared_ptr<EuroScopeCRadarTargetInterface> rt;

            for (auto it = aircraft.cbegin(); it != aircraft.cend(); ++it) {
                // Check for the radar target
                rt = this->plugin.GetRadarTargetForCallsign((*it)->GetCallsign());
                if (!rt) {
                    continue;
                }

                // If the aircraft is above the displaying levels of the hold, dont map
                int occupied = GetOccupiedLevel(rt->GetFlightLevel(), rt->GetVerticalSpeed());
                if (occupied > this->maximumLevel || occupied < this->minimumLevel) {
                    continue;
                }

                levelMap[occupied].insert(*it);
            }

            // Filter out levels that we aren't interested in
            this->FilterVslDisplayLevels(levelMap);
            return levelMap;
        }

        /*
            Return whether or not the display is minimised.
        */
        bool HoldDisplay::IsMinimised(void) const
        {
            return this->minimised;
        }

        /*
            Return whether or not the display is in informtion mode
        */
        bool HoldDisplay::IsInInformationMode(void) const
        {
            return this->showHoldInformation;
        }

        /*
            Load data from an ASR
        */
        void HoldDisplay::LoadDataFromAsr(UserSetting& userSetting)
        {
            this->minimised = userSetting.GetBooleanEntry("hold" + this->navaid.identifier + "Minimised", false);

            this->minimumLevel = userSetting.GetIntegerEntry(
                "hold" + this->navaid.identifier + "MinLevel",
                this->publishedHolds.size() ? (*this->publishedHolds.cbegin())->minimum : 7000);

            this->maximumLevel = userSetting.GetIntegerEntry(
                "hold" + this->navaid.identifier + "MaxLevel",
                this->publishedHolds.size() ? (*this->publishedHolds.cbegin())->maximum : 15000);

            this->Move(
                {userSetting.GetIntegerEntry("hold" + this->navaid.identifier + "PositionX", 100),
                 userSetting.GetIntegerEntry("hold" + this->navaid.identifier + "PositionY", 100)});
        }

        /*
            Move the display
        */
        void HoldDisplay::Move(const POINT& pos)
        {
            // General window pos
            this->windowPos = pos;
            this->dataStartHeight = this->dataStartOffset + pos.y;

            // Title bar
            this->titleArea.X = pos.x;
            this->titleArea.Y = pos.y;

            this->titleRect = {
                this->titleArea.X,
                this->titleArea.Y,
                this->titleArea.X + this->titleArea.Width,
                this->titleArea.Y + this->titleArea.Height};

            // Minimise Button
            this->minimiseButtonArea.X = pos.x + 5;
            this->minimiseButtonArea.Y = this->titleArea.Y + 2;

            this->minimiseClickRect = {
                this->minimiseButtonArea.X,
                this->minimiseButtonArea.Y,
                this->minimiseButtonArea.X + this->minimiseButtonArea.Width,
                this->minimiseButtonArea.Y + this->minimiseButtonArea.Height};

            // Information Button
            this->informationButtonArea.X = minimiseButtonArea.X + minimiseButtonArea.Width + 5;
            this->informationButtonArea.Y = this->titleArea.Y + 2;

            this->informationClickRect = {
                this->informationButtonArea.X,
                this->informationButtonArea.Y,
                this->informationButtonArea.X + this->informationButtonArea.Width,
                this->informationButtonArea.Y + this->informationButtonArea.Height};

            // Options button
            this->optionsButtonArea.X = informationButtonArea.X + informationButtonArea.Width + 5;
            this->optionsButtonArea.Y = this->titleArea.Y + 2;

            this->optionsClickRect = {
                this->optionsButtonArea.X,
                this->optionsButtonArea.Y,
                this->optionsButtonArea.X + this->optionsButtonArea.Width,
                this->optionsButtonArea.Y + this->optionsButtonArea.Height};

            // Minus button
            this->minusButtonRect.X = pos.x + this->buttonStartOffsetX + 5;
            this->minusButtonRect.Y = this->buttonStartOffsetY + pos.y;

            this->minusButtonClickRect = {
                this->minusButtonRect.X,
                this->minusButtonRect.Y,
                this->minusButtonRect.X + this->minusButtonRect.Width,
                this->minusButtonRect.Y + this->minusButtonRect.Height};

            // Plus button
            this->plusButtonRect.X = pos.x + this->buttonStartOffsetX + 55;
            this->plusButtonRect.Y = this->buttonStartOffsetY + pos.y;

            this->plusButtonClickRect = {
                this->plusButtonRect.X,
                this->plusButtonRect.Y,
                this->plusButtonRect.X + this->plusButtonRect.Width,
                this->plusButtonRect.Y + this->plusButtonRect.Height};

            // Show all levels button
            this->allButtonRect.X = pos.x + this->buttonStartOffsetX + 105;
            this->allButtonRect.Y = this->buttonStartOffsetY + pos.y;

            this->allButtonClickRect = {
                this->allButtonRect.X,
                this->allButtonRect.Y,
                this->allButtonRect.X + this->allButtonRect.Width,
                this->allButtonRect.Y + this->allButtonRect.Height};

            // Add button
            this->addButtonRect.X = pos.x + this->buttonStartOffsetX + 155;
            this->addButtonRect.Y = this->buttonStartOffsetY + pos.y;

            this->addButtonClickRect = {
                this->addButtonRect.X,
                this->addButtonRect.Y,
                this->addButtonRect.X + this->addButtonRect.Width,
                this->addButtonRect.Y + this->addButtonRect.Height};

            // Line under the buttons
            this->underButtonLineLeft.X = pos.x;
            this->underButtonLineLeft.Y = this->allButtonRect.Y + this->allButtonRect.Height + 7;

            this->underButtonLineRight.X = pos.x + this->windowWidth;
            this->underButtonLineRight.Y = this->allButtonRect.Y + this->allButtonRect.Height + 7;
        }

        /*
            Draw the rounded rectangles that form the action buttons
        */
        void
        HoldDisplay::DrawRoundRectangle(GdiGraphicsInterface& graphics, const Gdiplus::Rect& rect, UINT8 radius) const
        {
            Gdiplus::GraphicsPath path;
            path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - (radius * 2), rect.Y);
            path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y, radius * 2, radius * 2, 270, 90);
            path.AddLine(
                rect.X + rect.Width, rect.Y + radius, rect.X + rect.Width, rect.Y + rect.Height - (radius * 2));
            path.AddArc(
                rect.X + rect.Width - (radius * 2), rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 0, 90);
            path.AddLine(
                rect.X + rect.Width - (radius * 2), rect.Y + rect.Height, rect.X + radius, rect.Y + rect.Height);
            path.AddArc(rect.X, rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 90, 90);
            path.AddLine(rect.X, rect.Y + rect.Height - (radius * 2), rect.X, rect.Y + radius);
            path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
            path.CloseFigure();
            graphics.DrawPath(path, Gdiplus::Pen(this->brushes.border, 1.5f));
        }

        /*
            Render textual data about the hold itself.
        */
        void HoldDisplay::RenderHoldInformation(
            GdiGraphicsInterface& graphics,
            EuroscopeRadarLoopbackInterface& radarScreen,
            const int screenObjectId) const
        {
            // Black background and white border
            Gdiplus::Rect borderRect = {
                this->windowPos.x, this->windowPos.y, this->windowWidth, this->informationDisplayWindowHeight};

            graphics.FillRect(borderRect, Gdiplus::SolidBrush(this->brushes.background));
            graphics.DrawRect(borderRect, Gdiplus::Pen(this->brushes.border, 1.5f));

            // Render the title bar
            this->RenderTitleBar(graphics, radarScreen, screenObjectId);

            Gdiplus::Rect dataRect = {
                this->windowPos.x, this->dataStartHeight - 15, this->windowWidth, this->lineHeight};

            // Render a message if no published holds
            if (this->publishedHolds.empty()) {
                graphics.DrawString(std::wstring(L"No published holds found."), dataRect, Gdiplus::SolidBrush(this->brushes.text));
                return;
            }

            // Display the published hold selection buttons
            const HoldingData* hold = *std::next(this->publishedHolds.cbegin(), this->selectedPublishedHoldIndex);

            Gdiplus::Rect buttonRect = {this->windowPos.x + 5, this->titleArea.GetBottom() + 5, 20, 20};

            // Left
            graphics.DrawRect(buttonRect, Gdiplus::Pen(this->brushes.text));
            graphics.DrawString(L"<", buttonRect, Gdiplus::SolidBrush(this->brushes.text));
            radarScreen.RegisterScreenObject(
                screenObjectId,
                this->navaid.identifier + "/prevhold",
                RECT{buttonRect.GetLeft(), buttonRect.GetTop(), buttonRect.GetRight(), buttonRect.GetBottom()},
                false);

            // Right
            buttonRect.X = this->titleArea.GetRight() - 25;
            graphics.DrawRect(buttonRect, Gdiplus::Pen(this->brushes.text));
            graphics.DrawString(L">", buttonRect, Gdiplus::SolidBrush(this->brushes.text));
            radarScreen.RegisterScreenObject(
                screenObjectId,
                this->navaid.identifier + "/nexthold",
                RECT{buttonRect.GetLeft(), buttonRect.GetTop(), buttonRect.GetRight(), buttonRect.GetBottom()},
                false);

            // Text
            buttonRect.X = this->windowPos.x + 30;
            buttonRect.Width = this->titleArea.Width - 60;
            graphics.DrawString(
                L"Hold " + std::to_wstring(this->selectedPublishedHoldIndex + 1) + L" of " +
                    std::to_wstring(this->publishedHolds.size()),
                buttonRect,
                Gdiplus::SolidBrush(this->brushes.text));

            // Render the data
            graphics.DrawString(ConvertToTchar(hold->description), dataRect, Gdiplus::SolidBrush(this->brushes.text));

            dataRect.Y = dataRect.Y + this->lineHeight + 5;
            graphics.DrawString(
                std::wstring(L"Fix: ") + ConvertToTchar(this->navaid.identifier), dataRect, Gdiplus::SolidBrush(this->brushes.text));

            dataRect.Y = dataRect.Y + this->lineHeight + 5;
            graphics.DrawString(std::wstring(L"Inbound: ") + ConvertToTchar(hold->inbound), dataRect, Gdiplus::SolidBrush(this->brushes.text));

            dataRect.Y = dataRect.Y + this->lineHeight + 5;
            graphics.DrawString(
                std::wstring(L"Turn: ") + ConvertToTchar(hold->turnDirection), dataRect, Gdiplus::SolidBrush(this->brushes.text));

            dataRect.Y = dataRect.Y + this->lineHeight + 5;
            graphics.DrawString(std::wstring(L"Maximum: ") + ConvertToTchar(hold->maximum), dataRect, Gdiplus::SolidBrush(this->brushes.text));

            dataRect.Y = dataRect.Y + this->lineHeight + 5;
            graphics.DrawString(std::wstring(L"Minimum: ") + ConvertToTchar(hold->minimum), dataRect, Gdiplus::SolidBrush(this->brushes.text));

            dataRect.Y = dataRect.Y + this->lineHeight + 5;

            if (*hold->outboundLeg->unit == Geometry::MeasurementUnitType::None) {
                graphics.DrawString(std::wstring(L"Outbound Leg: --"), dataRect, Gdiplus::SolidBrush(this->brushes.text));
            } else {
                graphics.DrawString(
                    std::wstring(L"Outbound Leg: ") + FormatOutboundLegValue(hold->outboundLeg->value) + L" " +
                        ConvertToTchar(hold->outboundLeg->unit->description),
                    dataRect,
                    Gdiplus::SolidBrush(this->brushes.text));
            }
        }

        /*
            Draw the title bar for the hold display
        */
        void HoldDisplay::RenderTitleBar(
            GdiGraphicsInterface& graphics,
            EuroscopeRadarLoopbackInterface& radarScreen,
            const int screenObjectId) const
        {
            // Title bar
            radarScreen.RegisterScreenObject(screenObjectId, this->navaid.identifier, this->titleRect, true);
            graphics.FillRect(this->titleArea, Gdiplus::SolidBrush(this->brushes.header));
            graphics.DrawRect(this->titleArea, Gdiplus::Pen(this->brushes.border));

            std::wstring holdName = ConvertToTchar(this->navaid.identifier);
            graphics.DrawString(ConvertToTchar(this->navaid.identifier), this->titleArea, Gdiplus::SolidBrush(this->brushes.text));
            graphics.DrawLine(
                Gdiplus::Pen(this->brushes.border),
                Gdiplus::Point{this->titleArea.X, this->titleArea.Y + this->titleArea.Height},
                Gdiplus::Point{this->titleArea.X + this->titleArea.Width, this->titleArea.Y + this->titleArea.Height});

            // Minimise Button
            graphics.FillRect(this->minimiseButtonArea, Gdiplus::SolidBrush(this->brushes.background));
            graphics.DrawRect(this->minimiseButtonArea, Gdiplus::Pen(this->brushes.border));
            radarScreen.RegisterScreenObject(
                screenObjectId, this->navaid.identifier + "/minimise", this->minimiseClickRect, false);

            // Information button
            graphics.FillRect(this->informationButtonArea, Gdiplus::SolidBrush(this->brushes.background));
            graphics.DrawRect(this->informationButtonArea, Gdiplus::Pen(this->brushes.border));
            graphics.DrawString(L"i", this->informationButtonArea, Gdiplus::SolidBrush(this->brushes.text));
            radarScreen.RegisterScreenObject(
                screenObjectId, this->navaid.identifier + "/information", this->informationClickRect, false);

            // Options button
            graphics.FillRect(this->optionsButtonArea, Gdiplus::SolidBrush(this->brushes.background));
            graphics.DrawRect(this->optionsButtonArea, Gdiplus::Pen(this->brushes.border));
            graphics.DrawString(L"o", this->optionsButtonArea, Gdiplus::SolidBrush(this->brushes.text));
            radarScreen.RegisterScreenObject(
                screenObjectId, this->navaid.identifier + "/options", this->optionsClickRect, false);
        }

        void HoldDisplay::RenderActionButtons(
            GdiGraphicsInterface& graphics,
            EuroscopeRadarLoopbackInterface& radarScreen,
            const int screenObjectId) const
        {
            this->DrawRoundRectangle(graphics, minusButtonRect, 5);
            graphics.DrawString(L"-", minusButtonRect, Gdiplus::SolidBrush(this->brushes.text));
            radarScreen.RegisterScreenObject(
                screenObjectId, this->navaid.identifier + "/minus", this->minusButtonClickRect, false);

            this->DrawRoundRectangle(graphics, plusButtonRect, 5);
            graphics.DrawString(L"+", plusButtonRect, Gdiplus::SolidBrush(this->brushes.text));
            radarScreen.RegisterScreenObject(
                screenObjectId, this->navaid.identifier + "/plus", this->plusButtonClickRect, false);

            this->DrawRoundRectangle(graphics, addButtonRect, 5);
            graphics.DrawString(L"ADD", addButtonRect, Gdiplus::SolidBrush(this->brushes.text));
            radarScreen.RegisterScreenObject(
                screenObjectId, this->navaid.identifier + "/add", this->addButtonClickRect, false);

            this->DrawRoundRectangle(graphics, allButtonRect, 5);
            graphics.DrawString(L"ALL", allButtonRect, Gdiplus::SolidBrush(this->brushes.text));
            radarScreen.RegisterScreenObject(
                screenObjectId, this->navaid.identifier + "/allLevels", this->allButtonClickRect, false);

            graphics.DrawLine(Gdiplus::Pen(this->brushes.border), this->underButtonLineLeft, this->underButtonLineRight);
        }

        /*
            Render the managed hold data - positions of aircraft in the hold etc
        */
        void HoldDisplay::RenderManagedHoldDisplay(
            GdiGraphicsInterface& graphics,
            EuroscopeRadarLoopbackInterface& radarScreen,
            const int screenObjectId) const
        {
            // Get the aircraft in each hold level
            const std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> holdingAircraft =
                this->MapAircraftToLevels(this->holdManager.GetAircraftForHold(this->navaid.identifier));

            // Render the background
            Gdiplus::Rect backgroundRect = this->GetHoldViewBackgroundRender(holdingAircraft);
            graphics.FillRect(backgroundRect, Gdiplus::SolidBrush(this->brushes.background));

            // Render the title bar
            this->RenderTitleBar(graphics, radarScreen, screenObjectId);

            // Action buttons
            this->RenderActionButtons(graphics, radarScreen, screenObjectId);

            // Hold display
            Gdiplus::Rect numbersDisplay = {this->windowPos.x + 10, this->dataStartHeight, 30, this->lineHeight};

            // The row in question
            Gdiplus::Rect holdRow = {this->windowPos.x, this->dataStartHeight, this->windowWidth, this->lineHeight};

            // Rects for rendering the actual data
            Gdiplus::Rect callsignDisplay = {this->windowPos.x + 45, this->dataStartHeight, 60, this->lineHeight};

            Gdiplus::Rect actualLevelDisplay = {this->windowPos.x + 100, this->dataStartHeight, 40, this->lineHeight};

            Gdiplus::Point verticalSpeedArrowDisplayStart = {
                this->windowPos.x + 135,
                this->dataStartHeight + (this->lineHeight / 3),
            };

            Gdiplus::Point verticalSpeedArrowDisplayEnd = {
                this->windowPos.x + 135,
                this->dataStartHeight + this->lineHeight - (this->lineHeight / 3),
            };

            Gdiplus::Rect clearedLevelDisplay = {this->windowPos.x + 145, this->dataStartHeight, 30, this->lineHeight};

            Gdiplus::Rect timeInHoldDisplay = {this->windowPos.x + 180, this->dataStartHeight, 30, this->lineHeight};

            // Loop over all the possible levels in the hold and render
            for (int level = this->maximumLevel; level >= this->minimumLevel; level -= 1000) {
                bool levelRestricted = false;
                bool aircraftInProximity = false;

                // Loop the published holds and check if we need to render any restrictions
                for (std::set<const HoldingData*>::const_iterator publishedIt = this->publishedHolds.cbegin();
                     publishedIt != this->publishedHolds.cend();
                     ++publishedIt) {
                    // Print the restrictions
                    for (auto it = (*publishedIt)->restrictions.cbegin(); it != (*publishedIt)->restrictions.cend();
                         ++it) {
                        if ((*it)->LevelRestricted(level)) {
                            levelRestricted = true;
                            break;
                        }
                    }
                }

                // No holding aircraft at this level, so just render the blank display
                if (holdingAircraft.count(level) == 0) {

                    // Render the restrictions
                    if (levelRestricted) {
                        graphics.FillRect(holdRow, Gdiplus::SolidBrush(this->brushes.border));
                    }

                    // Render the numbers
                    graphics.DrawString(GetLevelDisplayString(level), numbersDisplay, Gdiplus::SolidBrush(this->brushes.text));

                    // Increase the lines
                    holdRow.Y = holdRow.Y + this->lineHeight;
                    numbersDisplay.Y = numbersDisplay.Y + this->lineHeight;
                    callsignDisplay.Y = callsignDisplay.Y + this->lineHeight;
                    actualLevelDisplay.Y = actualLevelDisplay.Y + this->lineHeight;
                    clearedLevelDisplay.Y = clearedLevelDisplay.Y + this->lineHeight;
                    timeInHoldDisplay.Y = timeInHoldDisplay.Y + this->lineHeight;
                    verticalSpeedArrowDisplayStart.Y = verticalSpeedArrowDisplayStart.Y + this->lineHeight;
                    verticalSpeedArrowDisplayEnd.Y = verticalSpeedArrowDisplayEnd.Y + this->lineHeight;

                } else {
                    std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraftAtLevel =
                        holdingAircraft.at(level);
                    int aircraftIndex = 0;

                    std::shared_ptr<EuroScopeCRadarTargetInterface> rt;
                    std::shared_ptr<EuroScopeCFlightPlanInterface> fp;

                    // We have holding aircraft to deal with, render them in
                    for (std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>::const_iterator it =
                             aircraftAtLevel.cbegin();
                         it != aircraftAtLevel.cend();
                         ++it) {
                        // Render the restrictions
                        if (levelRestricted) {
                            graphics.FillRect(holdRow, Gdiplus::SolidBrush(this->brushes.border));
                        }

                        // Render the numbers
                        if (aircraftIndex == 0) {
                            graphics.DrawString(GetLevelDisplayString(level), numbersDisplay, Gdiplus::SolidBrush(this->brushes.text));
                        }

                        rt = this->plugin.GetRadarTargetForCallsign((*it)->GetCallsign());
                        fp = this->plugin.GetFlightplanForCallsign((*it)->GetCallsign());

                        if (fp && rt) {
                            if (rt->GetPosition().DistanceTo(this->navaid.coordinates) < this->sameLevelBoxDistance) {
                                aircraftInProximity = true;
                            }

                            // Callsign
                            std::wstring callsign = ConvertToTchar((*it)->GetCallsign());
                            graphics.DrawString(callsign, callsignDisplay, Gdiplus::SolidBrush(this->brushes.text));
                            radarScreen.RegisterScreenObject(
                                screenObjectId,
                                this->navaid.identifier + "/callsign/" + fp->GetCallsign(),
                                {callsignDisplay.X,
                                 callsignDisplay.Y,
                                 callsignDisplay.X + callsignDisplay.Width,
                                 callsignDisplay.Y + callsignDisplay.Height},
                                false);

                            // Reported level
                            graphics.DrawString(
                                GetLevelDisplayString(rt->GetFlightLevel()), actualLevelDisplay, Gdiplus::SolidBrush(this->brushes.text));
                            if (GetVerticalSpeedDirection(rt->GetVerticalSpeed()) == 1) {
                                graphics.DrawLine(
                                    this->verticalSpeedAscentPen,
                                    verticalSpeedArrowDisplayStart,
                                    verticalSpeedArrowDisplayEnd);
                            } else if (GetVerticalSpeedDirection(rt->GetVerticalSpeed()) == -1) {
                                graphics.DrawLine(
                                    this->verticalSpeedDescentPen,
                                    verticalSpeedArrowDisplayStart,
                                    verticalSpeedArrowDisplayEnd);
                            }

                            // Cleared level - plus a clickspot for the aircraft in question
                            graphics.DrawString(
                                fp->GetClearedAltitude() == 0 ? L"---"
                                                              : GetLevelDisplayString(fp->GetClearedAltitude()),
                                clearedLevelDisplay,
                                Gdiplus::SolidBrush(this->brushes.highlightedAircraftText));
                            radarScreen.RegisterScreenObject(
                                screenObjectId,
                                this->navaid.identifier + "/cleared/" + fp->GetCallsign(),
                                {clearedLevelDisplay.X,
                                 clearedLevelDisplay.Y,
                                 clearedLevelDisplay.X + clearedLevelDisplay.Width,
                                 clearedLevelDisplay.Y + clearedLevelDisplay.Height},
                                false);

                            // Time in hold, if it's assigned to this one
                            if ((*it)->GetAssignedHold() == navaid.identifier) {
                                auto holdProximity = (*it)->GetProximityHold(navaid.identifier);
                                if (holdProximity != nullptr && holdProximity->HasEntered()) {
                                    std::wstring timeString = GetTimeInHoldDisplayString(holdProximity->EnteredAt());
                                    graphics.DrawString(timeString, timeInHoldDisplay, Gdiplus::SolidBrush(this->brushes.text));
                                }
                            }
                        }

                        aircraftIndex++;

                        // Increase the lines
                        holdRow.Y = holdRow.Y + this->lineHeight;
                        numbersDisplay.Y = numbersDisplay.Y + this->lineHeight;
                        callsignDisplay.Y = callsignDisplay.Y + this->lineHeight;
                        actualLevelDisplay.Y = actualLevelDisplay.Y + this->lineHeight;
                        clearedLevelDisplay.Y = clearedLevelDisplay.Y + this->lineHeight;
                        timeInHoldDisplay.Y = timeInHoldDisplay.Y + this->lineHeight;
                        verticalSpeedArrowDisplayStart.Y = verticalSpeedArrowDisplayStart.Y + this->lineHeight;
                        verticalSpeedArrowDisplayEnd.Y = verticalSpeedArrowDisplayEnd.Y + this->lineHeight;
                    }

                    // If we have multiple at the level, with one close to the hold, draw a bounding box
                    if (aircraftAtLevel.size() > 1 && aircraftInProximity) {
                        Gdiplus::Rect boundingBox{
                            holdRow.X + 10,
                            holdRow.Y - (static_cast<INT>(aircraftAtLevel.size()) * this->lineHeight),
                            holdRow.Width - 20,
                            (static_cast<INT>(aircraftAtLevel.size()) * this->lineHeight)};
                        graphics.DrawRect(boundingBox, Gdiplus::Pen(this->brushes.text));
                    }
                }
            }

            // Border around whole thing, draw this last
            graphics.DrawRect(
                RECT{
                    this->windowPos.x,
                    this->windowPos.y,
                    this->windowPos.x + this->windowWidth,
                    holdRow.Y + this->lineHeight},
                Gdiplus::Pen(this->brushes.border));
        }

        void HoldDisplay::PaintWindow(
            GdiGraphicsInterface& graphics,
            EuroscopeRadarLoopbackInterface& radarScreen,
            const int screenObjectId) const
        {
            // Minimised, just render the title bar.
            if (this->minimised) {
                this->RenderTitleBar(graphics, radarScreen, screenObjectId);
                return;
            }

            // Render the correct view.
            this->showHoldInformation ? this->RenderHoldInformation(graphics, radarScreen, screenObjectId)
                                      : this->RenderManagedHoldDisplay(graphics, radarScreen, screenObjectId);
        }

        /*
            Save display data to the ASR
        */
        void HoldDisplay::SaveDataToAsr(UserSetting& userSetting) const
        {
            // Minimised window
            userSetting.Save(
                "hold" + this->navaid.identifier + "Minimised",
                "Hold " + this->navaid.identifier + " Minimised",
                this->minimised);

            // X position
            userSetting.Save(
                "hold" + this->navaid.identifier + "PositionX",
                "Hold " + this->navaid.identifier + " X Position",
                this->windowPos.x);

            // Y position
            userSetting.Save(
                "hold" + this->navaid.identifier + "PositionY",
                "Hold " + this->navaid.identifier + " Y Position",
                this->windowPos.y);

            // Min level
            userSetting.Save(
                "hold" + this->navaid.identifier + "MinLevel",
                "Hold " + this->navaid.identifier + " Minimum Level",
                this->minimumLevel);

            // Max level
            userSetting.Save(
                "hold" + this->navaid.identifier + "MaxLevel",
                "Hold " + this->navaid.identifier + " Maximum Level",
                this->maximumLevel);
        }
    } // namespace Hold
} // namespace UKControllerPlugin
