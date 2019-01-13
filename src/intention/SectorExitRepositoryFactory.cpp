#include "pch/stdafx.h"
#include "intention/SectorExitRepositoryFactory.h"
#include "intention/SectorExitRepository.h"
#include "intention/SectorExitPoint.h"
#include "intention/SectorExitPointEtrat.h"
#include "intention/SectorExitPointVeule.h"
#include "intention/SectorExitPointLelna.h"
#include "intention/SectorExitPointShanwick.h"

using UKControllerPlugin::IntentionCode::SectorExitPoint;
using UKControllerPlugin::IntentionCode::SectorExitPointEtrat;
using UKControllerPlugin::IntentionCode::SectorExitPointVeule;
using UKControllerPlugin::IntentionCode::SectorExitPointLelna;
using UKControllerPlugin::IntentionCode::SectorExitPointShanwick;
using UKControllerPlugin::IntentionCode::SectorExitRepository;

namespace UKControllerPlugin {
    namespace IntentionCode {

        std::unique_ptr<SectorExitRepository> SectorExitRepositoryFactory::Create()
        {
            std::map<std::string, std::unique_ptr<SectorExitPoint>> points;

            // Dover
            points["KOK"] = std::make_unique<SectorExitPoint>("KOK", "D1", SectorExitPoint::outEast);
            points["TRACA"] = std::make_unique<SectorExitPoint>("TRACA", "D2", SectorExitPoint::outEast);
            points["MOTOX"] = std::make_unique<SectorExitPoint>("MOTOX", "D3", SectorExitPoint::outEast);

            // Clacton
            points["SOMVA"] = std::make_unique<SectorExitPoint>("SOMVA", "C1", SectorExitPoint::outEast);
            points["REDFA"] = std::make_unique<SectorExitPoint>("REDFA", "C2", SectorExitPoint::outEast);
            points["SASKI"] = std::make_unique<SectorExitPoint>("SASKI", "C3", SectorExitPoint::outEast);

            // North Sea
            points["LONAM"] = std::make_unique<SectorExitPoint>(
                "LONAM",
                "F",
                SectorExitPoint::outEast || SectorExitPoint::outSouthEast
            );
            points["TOPPA"] = std::make_unique<SectorExitPoint>("TOPPA", "F", SectorExitPoint::outEast);
            points["ROKAN"] = std::make_unique<SectorExitPoint>("ROKAN", "F", SectorExitPoint::outEast);
            points["LAMSO"] = std::make_unique<SectorExitPoint>("LAMSO", "F", SectorExitPoint::outEast);
            points["GODOS"] = std::make_unique<SectorExitPoint>("GODOS", "F", SectorExitPoint::outEast);
            points["MOLIX"] = std::make_unique<SectorExitPoint>("MOLIX", "F", SectorExitPoint::outEast);

            // Copenhagen
            points["VAXIT"] = std::make_unique<SectorExitPoint>("VAXIT", "K1", SectorExitPoint::outEast);
            points["TINAC"] = std::make_unique<SectorExitPoint>("TINAC", "K2", SectorExitPoint::outEast);
            points["PETIL"] = std::make_unique<SectorExitPoint>("PETIL", "K3", SectorExitPoint::outEast);
            points["INBOB"] = std::make_unique<SectorExitPoint>("INBOB", "K4", SectorExitPoint::outEast);

            // Stavanger
            points["PEPIN"] = std::make_unique<SectorExitPoint>("PEPIN", "Z1", SectorExitPoint::outEast);
            points["ORVIK"] = std::make_unique<SectorExitPoint>("ORVIK", "Z2", SectorExitPoint::outEast);
            points["KLONN"] = std::make_unique<SectorExitPoint>("KLONN", "Z3", SectorExitPoint::outEast);
            points["ALOTI"] = std::make_unique<SectorExitPoint>("ALOTI", "Z4", SectorExitPoint::outEast);
            points["NIVUN"] = std::make_unique<SectorExitPoint>("NIVUN", "Z5", SectorExitPoint::outEast);

            // Reykjavik
            points["RATSU"] = std::make_unique<SectorExitPoint>("RATSU", "R", SectorExitPoint::outNorth);
            points["MATIK"] = std::make_unique<SectorExitPoint>("MATIK", "R1", SectorExitPoint::outNorth);
            points["OLKER"] = std::make_unique<SectorExitPoint>("OLKER", "R2", SectorExitPoint::outNorth);
            points["GONUT"] = std::make_unique<SectorExitPoint>("GONUT", "R3", SectorExitPoint::outNorth);
            points["LIRKI"] = std::make_unique<SectorExitPoint>("LIRKI", "R4", SectorExitPoint::outNorth);
            points["GUNPA"] = std::make_unique<SectorExitPoint>("GUNPA", "R5", SectorExitPoint::outNorth);

            // Shanwick
            points["ATSIX"] = std::make_unique<SectorExitPoint>("ATSIX", "Y", SectorExitPoint::outWest);
            points["BALIX"] = std::make_unique<SectorExitPoint>("BALIX", "N9", SectorExitPoint::outWest);
            points["ERAKA"] = std::make_unique<SectorExitPoint>(
                "ERAKA",
                "N8",
                SectorExitPoint::outWest | SectorExitPoint::outNorthWest
            );

            // ADODO isn't really an exit point, but it's near ERAKA
            points["ADODO"] = std::make_unique<SectorExitPoint>(
                "ADODO",
                "N8",
                SectorExitPoint::outWest | SectorExitPoint::outNorthWest
            );
            points["GOMUP"] = std::make_unique<SectorExitPointShanwick>("GOMUP", "N7", SectorExitPoint::outWest);
            points["IBROD"] = std::make_unique<SectorExitPoint>("IBROD", "N", SectorExitPoint::outWest);
            points["MIMKU"] = std::make_unique<SectorExitPointShanwick>("MIMKU", "N6", SectorExitPoint::outWest);
            points["NIBOG"] = std::make_unique<SectorExitPointShanwick>("NIBOG", "N5", SectorExitPoint::outWest);

            // Strumble
            points["VATRY"] = std::make_unique<SectorExitPoint>("VATRY", "S4", SectorExitPoint::outWest);
            points["BAKUR"] = std::make_unique<SectorExitPoint>("BAKUR", "S3", SectorExitPoint::outWest);
            points["SLANY"] = std::make_unique<SectorExitPoint>("SLANY", "S2", SectorExitPoint::outWest);
            points["BANBA"] = std::make_unique<SectorExitPoint>("BANBA", "S", SectorExitPoint::outWest);
            points["EVRIN"] = std::make_unique<SectorExitPoint>("EVRIN", "S", SectorExitPoint::outWest);

            // Irish Overflights
            points["MOLAK"] = std::make_unique<SectorExitPoint>("MOLAK", "G1", SectorExitPoint::outWest);
            points["NIPIT"] = std::make_unique<SectorExitPoint>("NIPIT", "G2", SectorExitPoint::outWest);
            points["ERNAN"] = std::make_unique<SectorExitPoint>("ERNAN", "G3", SectorExitPoint::outWest);
            points["DEGOS"] = std::make_unique<SectorExitPoint>("DEGOS", "G4", SectorExitPoint::outWest);
            points["NIMAT"] = std::make_unique<SectorExitPoint>("NIMAT", "G5", SectorExitPoint::outWest);

            // Brest
            points["ANNET"] = std::make_unique<SectorExitPoint>("ANNET", "B4", SectorExitPoint::outSouth);
            points["LIZAD"] = std::make_unique<SectorExitPoint>("LIZAD", "B4", SectorExitPoint::outSouth);
            points["GANTO"] = std::make_unique<SectorExitPoint>("GANTO", "B4", SectorExitPoint::outSouth);
            points["SUPAP"] = std::make_unique<SectorExitPoint>("SUPAP", "B4", SectorExitPoint::outSouth);
            points["PEMAK"] = std::make_unique<SectorExitPoint>("PEMAK", "B4", SectorExitPoint::outSouth);
            points["SALCO"] = std::make_unique<SectorExitPoint>("SALCO", "B3", SectorExitPoint::outSouth);
            points["MANIG"] = std::make_unique<SectorExitPoint>("MANIG", "B2", SectorExitPoint::outSouth);
            points["SKESO"] = std::make_unique<SectorExitPoint>("SKESO", "B", SectorExitPoint::outSouth);
            points["SKERY"] = std::make_unique<SectorExitPoint>("SKERY", "B", SectorExitPoint::outSouth);

            // Hern
            points["ORTAC"] = std::make_unique<SectorExitPoint>("ORTAC", "H", SectorExitPoint::outSouth);
            points["LORKU"] = std::make_unique<SectorExitPoint>("LORKU", "H4", SectorExitPoint::outSouth);
            points["LELNA"] = std::make_unique<SectorExitPointLelna>("LELNA", "H2", SectorExitPoint::outSouth);

            // Paris
            points["NEVIL"] = std::make_unique<SectorExitPoint>("NEVIL", "W", SectorExitPoint::outSouth);
            points["ANGLO"] = std::make_unique<SectorExitPoint>("ANGLO", "W", SectorExitPoint::outSouth);
            points["ETRAT"] = std::make_unique<SectorExitPoint>("ETRAT", "E", SectorExitPoint::outSouth);
            points["VEULE"] = std::make_unique<SectorExitPointVeule>("VEULE", "V", SectorExitPoint::outSouth);
            points["PETAX"] = std::make_unique<SectorExitPoint>("PETAX", "P", SectorExitPoint::outSouth);

            // Ireland
            points["BAGSO"] = std::make_unique<SectorExitPoint>("BAGSO", "L", SectorExitPoint::outWest);
            points["BOYNE"] = std::make_unique<SectorExitPoint>("BOYNE", "L", SectorExitPoint::outWest);
            points["LIPGO"] = std::make_unique<SectorExitPoint>("LIPGO", "T", SectorExitPoint::outWest);
            points["MORAG"] = std::make_unique<SectorExitPoint>("MORAG", "T", SectorExitPoint::outWest);

            // Irish Sea
            points["NORLA"] = std::make_unique<SectorExitPoint>("NORLA", "M", SectorExitPoint::outWest);
            points["MOPAT"] = std::make_unique<SectorExitPoint>("MOPAT", "M", SectorExitPoint::outWest);
            points["SAMON"] = std::make_unique<SectorExitPoint>("SAMON", "M", SectorExitPoint::outWest);
            points["LEDGO"] = std::make_unique<SectorExitPoint>("LEDGO", "M", SectorExitPoint::outWest);
            points["LESLU"] = std::make_unique<SectorExitPoint>("LESLU", "M", SectorExitPoint::outWest);

            // Lands End
            points["ARKIL"] = std::make_unique<SectorExitPoint>(
                "ARKIL",
                "A",
                SectorExitPoint::outWest | SectorExitPoint::outSouthWest
            );
            points["LULOX"] = std::make_unique<SectorExitPoint>(
                "LULOX",
                "A",
                SectorExitPoint::outWest | SectorExitPoint::outSouthWest
            );
            points["TURLU"] = std::make_unique<SectorExitPoint>(
                "TURLU",
                "A",
                SectorExitPoint::outWest | SectorExitPoint::outSouthWest
            );
            points["GAPLI"] = std::make_unique<SectorExitPoint>(
                "GAPLI",
                "A",
                SectorExitPoint::outWest | SectorExitPoint::outSouthWest
            );
            points["RATKA"] = std::make_unique<SectorExitPoint>(
                "RATKA",
                "A",
                SectorExitPoint::outWest | SectorExitPoint::outSouthWest
            );
            points["BISKI"] = std::make_unique<SectorExitPoint>(
                "BISKI",
                "A",
                SectorExitPoint::outWest | SectorExitPoint::outSouthWest
            );
            points["TAKAS"] = std::make_unique<SectorExitPoint>(
                "TAKAS",
                "A",
                SectorExitPoint::outWest | SectorExitPoint::outSouthWest
            );

            return std::make_unique<SectorExitRepository>(std::move(points));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
