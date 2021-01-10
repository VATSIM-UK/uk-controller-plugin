#include "pch/stdafx.h"
#include "intention/SectorExitRepositoryFactory.h"
#include "intention/SectorExitRepository.h"
#include "intention/SectorExitPoint.h"
#include "intention/SectorExitPointEtrat.h"
#include "intention/SectorExitPointVeule.h"
#include "intention/SectorExitPointLelna.h"
#include "intention/SectorExitPointScottish.h"

namespace UKControllerPlugin {
    namespace IntentionCode {

        std::unique_ptr<SectorExitRepository> SectorExitRepositoryFactory::Create()
        {
            std::map<std::string, std::unique_ptr<SectorExitPoint>> points;

            // Dover
            points["KOK"] = std::make_unique<SectorExitPoint>(
                "KOK",
                "D1",
                SectorExitPoint::outEast | SectorExitPoint::outSouthEast
            );
            points["TRACA"] = std::make_unique<SectorExitPoint>(
                "TRACA",
                "D2",
                SectorExitPoint::outEast | SectorExitPoint::outSouthEast
            );
            points["MOTOX"] = std::make_unique<SectorExitPoint>(
                "MOTOX",
                "D3",
                SectorExitPoint::outEast | SectorExitPoint::outSouthEast
            );
            points["RINTI"] = std::make_unique<SectorExitPoint>("RINTI", "D4", SectorExitPoint::outSouthEast);

            // Clacton
            points["SOMVA"] = std::make_unique<SectorExitPoint>("SOMVA", "C1", SectorExitPoint::outEast);
            points["REDFA"] = std::make_unique<SectorExitPoint>("REDFA", "C2", SectorExitPoint::outEast);
            points["SASKI"] = std::make_unique<SectorExitPoint>("SASKI", "C3", SectorExitPoint::outEast);

            // North Sea
            points["LONAM"] = std::make_unique<SectorExitPoint>(
                "LONAM",
                "F",
                SectorExitPoint::outEast | SectorExitPoint::outSouthEast
            );
            points["TOPPA"] = std::make_unique<SectorExitPoint>("TOPPA", "F", SectorExitPoint::outEast);
            points["ROKAN"] = std::make_unique<SectorExitPoint>("ROKAN", "F", SectorExitPoint::outEast);
            points["LAMSO"] = std::make_unique<SectorExitPoint>("LAMSO", "F", SectorExitPoint::outEast);
            points["GODOS"] = std::make_unique<SectorExitPoint>("GODOS", "F", SectorExitPoint::outEast);
            points["MOLIX"] = std::make_unique<SectorExitPoint>("MOLIX", "F", SectorExitPoint::outEast);

            // Copenhagen
            points["VAXIT"] = std::make_unique<SectorExitPoint>(
                "VAXIT",
                "K1",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );
            points["TINAC"] = std::make_unique<SectorExitPoint>(
                "TINAC",
                "K2",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );
            points["PETIL"] = std::make_unique<SectorExitPoint>(
                "PETIL",
                "K2",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );
            points["INBOB"] = std::make_unique<SectorExitPoint>(
                "INBOB",
                "K3",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );
            points["LESRA"] = std::make_unique<SectorExitPoint>(
                "LESRA",
                "K3",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );
            points["SOPTO"] = std::make_unique<SectorExitPoint>(
                "SOPTO",
                "K3",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );

            // Stavanger
            points["PEPIN"] = std::make_unique<SectorExitPoint>(
                "PEPIN",
                "Z1",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );
            points["ORVIK"] = std::make_unique<SectorExitPoint>(
                "ORVIK",
                "Z2",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );
            points["KLONN"] = std::make_unique<SectorExitPoint>(
                "KLONN",
                "Z3",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );
            points["ALOTI"] = std::make_unique<SectorExitPoint>(
                "ALOTI",
                "Z4",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );
            points["NIVUN"] = std::make_unique<SectorExitPoint>(
                "NIVUN",
                "Z5",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );
            points["BEREP"] = std::make_unique<SectorExitPoint>(
                "BEREP",
                "Z6",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );
            points["RIGVU"] = std::make_unique<SectorExitPoint>(
                "RIGVU",
                "Z7",
                SectorExitPoint::outEast | SectorExitPoint::outNorthEast
            );

            // Reykjavik
            points["RATSU"] = std::make_unique<SectorExitPoint>(
                "RATSU",
                "R",
                SectorExitPoint::outNorth | SectorExitPoint::outNorthWest
            );
            points["MATIK"] = std::make_unique<SectorExitPoint>("MATIK", "R1", SectorExitPoint::outNorth);
            points["NALAN"] = std::make_unique<SectorExitPoint>("NALAN", "R2", SectorExitPoint::outNorth);
            points["OSBON"] = std::make_unique<SectorExitPoint>("OSBON", "R3", SectorExitPoint::outNorth);
            points["PEMOS"] = std::make_unique<SectorExitPoint>("PEMOS", "R5", SectorExitPoint::outNorth);
            points["RIXUN"] = std::make_unique<SectorExitPoint>("RIXUN", "R6", SectorExitPoint::outNorth);
            points["SOSAR"] = std::make_unique<SectorExitPoint>("SOSAR", "R7", SectorExitPoint::outNorth);
            points["GUNPA"] = std::make_unique<SectorExitPoint>("GUNPA", "R8", SectorExitPoint::outNorth);

            // Shanwick
            points["LUSEN"] = std::make_unique<SectorExitPoint>(
                "LUSEN",
                "Y8",
                SectorExitPoint::outNorthWest | SectorExitPoint::outNorth
            );
            points["ATSIX"] = std::make_unique<SectorExitPoint>(
                "ATSIX",
                "Y7",
                SectorExitPoint::outWest | SectorExitPoint::outNorthWest
            );
            points["ORTAV"] = std::make_unique<SectorExitPoint>(
                "ORTAV",
                "Y6",
                SectorExitPoint::outWest | SectorExitPoint::outNorthWest
            );
            points["BALIX"] = std::make_unique<SectorExitPoint>(
                "BALIX",
                "Y5",
                SectorExitPoint::outWest | SectorExitPoint::outNorthWest
            );
            points["ADODO"] = std::make_unique<SectorExitPoint>(
                "ADODO",
                "Y4",
                SectorExitPoint::outWest | SectorExitPoint::outNorthWest
            );
            points["ERAKA"] = std::make_unique<SectorExitPoint>(
                "ERAKA",
                "Y3",
                SectorExitPoint::outWest | SectorExitPoint::outNorthWest
            );
            points["ETILO"] = std::make_unique<SectorExitPoint>(
                "ETILO",
                "Y2",
                SectorExitPoint::outWest | SectorExitPoint::outNorthWest
            );

            // Shanwick NOTA
            points["GOMUP"] = std::make_unique<SectorExitPoint>(
                "GOMUP",
                "Y1",
                SectorExitPoint::outWest | SectorExitPoint::outNorthWest
            );
            points["IBROD"] = std::make_unique<SectorExitPoint>("IBROD", "N7", SectorExitPoint::outWest);
            points["AMLAD"] = std::make_unique<SectorExitPoint>("AMLAD", "N6", SectorExitPoint::outWest);
            points["MIMKU"] = std::make_unique<SectorExitPoint>("MIMKU", "N5", SectorExitPoint::outWest);
            points["APSOV"] = std::make_unique<SectorExitPoint>("APSOV", "N4", SectorExitPoint::outWest);
            points["KUGUR"] = std::make_unique<SectorExitPoint>("KUGUR", "N3", SectorExitPoint::outWest);
            points["LUTOV"] = std::make_unique<SectorExitPoint>("LUTOV", "N2", SectorExitPoint::outWest);
            points["NIBOG"] = std::make_unique<SectorExitPoint>("NIBOG", "N1", SectorExitPoint::outWest);

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
            points["NEVRI"] = std::make_unique<SectorExitPoint>("NEVRI", "G7", SectorExitPoint::outWest);
            points["RUBEX"] = std::make_unique<SectorExitPoint>("RUBEX", "G9", SectorExitPoint::outWest);

            // Brest
            points["ANNET"] = std::make_unique<SectorExitPoint>("ANNET", "B4", SectorExitPoint::outSouth);
            points["LIZAD"] = std::make_unique<SectorExitPoint>("LIZAD", "B4", SectorExitPoint::outSouth);
            points["GANTO"] = std::make_unique<SectorExitPoint>("GANTO", "B5", SectorExitPoint::outSouth);
            points["SUPAP"] = std::make_unique<SectorExitPoint>("SUPAP", "B6", SectorExitPoint::outSouth);
            points["PEMAK"] = std::make_unique<SectorExitPoint>("PEMAK", "B7", SectorExitPoint::outSouth);
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
            points["LEDGO"] = std::make_unique<SectorExitPoint>("LEDGO", "M1", SectorExitPoint::outWest);
            points["MOPAT"] = std::make_unique<SectorExitPoint>("MOPAT", "M1", SectorExitPoint::outWest);
            points["LESLU"] = std::make_unique<SectorExitPoint>("LESLU", "M", SectorExitPoint::outWest);
            points["NORLA"] = std::make_unique<SectorExitPoint>("NORLA", "M", SectorExitPoint::outWest);
            points["SAMON"] = std::make_unique<SectorExitPoint>("SAMON", "M", SectorExitPoint::outWest);

            // Lands End
            points["ARKIL"] = std::make_unique<SectorExitPoint>(
                "ARKIL",
                "A1",
                SectorExitPoint::outWest | SectorExitPoint::outSouthWest
            );
            points["LULOX"] = std::make_unique<SectorExitPoint>(
                "LULOX",
                "A1",
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

            // EGPX Specific codes

            // Irish Sea
            points["INKOB"] = std::make_unique<SectorExitPointScottish>(
                "INKOB", "E4", SectorExitPoint::outEast | SectorExitPoint::outSouthEast
            );
            points["SOSIM"] = std::make_unique<SectorExitPointScottish>(
                "SOSIM", "E5", SectorExitPoint::outEast | SectorExitPoint::outSouthEast
            );
            points["KELLY"] = std::make_unique<SectorExitPointScottish>(
                "KELLY", "E7", SectorExitPoint::outEast | SectorExitPoint::outSouthEast
            );
            points["TUPEM"] = std::make_unique<SectorExitPointScottish>(
                "TUPEM", "E8", SectorExitPoint::outEast | SectorExitPoint::outSouthEast
            );
            points["BELOX"] = std::make_unique<SectorExitPointScottish>(
                "BELOX", "E9", SectorExitPoint::outEast | SectorExitPoint::outSouthEast
            );

            // Mainland
            points["SUBUK"] = std::make_unique<SectorExitPointScottish>("SUBUK", "H6", SectorExitPoint::outSouth);
            points["LAKEY"] = std::make_unique<SectorExitPointScottish>("LAKEY", "H7", SectorExitPoint::outSouth);
            points["BINTI"] = std::make_unique<SectorExitPointScottish>(
                "BINTI", "U1", SectorExitPoint::outSouth | SectorExitPoint::outSouthWest
            );
            points["TILNI"] = std::make_unique<SectorExitPointScottish>(
                "TILNI", "U2", SectorExitPoint::outSouth | SectorExitPoint::outSouthWest
            );

            // South East
            points["ERKIT"] = std::make_unique<SectorExitPointScottish>(
                "ERKIT", "U3", SectorExitPoint::outSouthEast | SectorExitPoint::outSouth
            );
            points["ROVNI"] = std::make_unique<SectorExitPointScottish>(
                "ROVNI", "U4", SectorExitPoint::outSouthWest | SectorExitPoint::outSouth
            );
            points["ELNAB"] = std::make_unique<SectorExitPointScottish>(
                "ELNAB", "U5", SectorExitPoint::outSouth
            );
            points["ADGEG"] = std::make_unique<SectorExitPointScottish>(
                "ADGEG", "U6", SectorExitPoint::outSouthEast | SectorExitPoint::outEast
            );
            points["AKOKO"] = std::make_unique<SectorExitPointScottish>(
                "AKOKO", "U7", SectorExitPoint::outSouthEast | SectorExitPoint::outEast
            );

            return std::make_unique<SectorExitRepository>(std::move(points));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
