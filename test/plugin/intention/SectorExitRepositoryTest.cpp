#include "intention/SectorExitPoint.h"
#include "intention/SectorExitRepository.h"
#include "intention/SectorExitRepositoryFactory.h"

using ::testing::NiceMock;
using ::testing::Return;
using UKControllerPlugin::IntentionCode::SectorExitRepository;
using UKControllerPlugin::IntentionCode::SectorExitRepositoryFactory;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;

namespace UKControllerPluginTest {
    namespace IntentionCode {

        TEST(SectorExitRepository, HasSectorExitPointReturnsTrueIfPointExists)
        {
            EXPECT_TRUE(SectorExitRepositoryFactory::Create()->HasSectorExitPoint("KONAN"));
        }

        TEST(SectorExitRepository, ConstructorInitialisesSectorExitPoints)
        {
            std::unique_ptr<SectorExitRepository> repo = SectorExitRepositoryFactory::Create();
            NiceMock<MockEuroscopeExtractedRouteInterface> mockRoute;
            ON_CALL(mockRoute, GetPointsNumber()).WillByDefault(Return(-1));

            ON_CALL(mockRoute, GetPointName(0)).WillByDefault(Return(""));

            // Brussels

            // Koksy / KONAN
            ASSERT_TRUE(repo->GetSectorExitPoint("KONAN").GetName().compare("KONAN") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("KONAN").GetIntentionCode(mockRoute, 0, 37000).compare("D") == 0);
            ASSERT_EQ(repo->outEast | repo->outSouthEast, repo->GetSectorExitPoint("KONAN").GetOutDirection());

            // TRACA
            ASSERT_TRUE(repo->GetSectorExitPoint("TRACA").GetName().compare("TRACA") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("TRACA").GetIntentionCode(mockRoute, 0, 37000).compare("D2") == 0);
            ASSERT_EQ(repo->outEast | repo->outSouthEast, repo->GetSectorExitPoint("TRACA").GetOutDirection());

            // MOTOX
            ASSERT_TRUE(repo->GetSectorExitPoint("MOTOX").GetName().compare("MOTOX") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("MOTOX").GetIntentionCode(mockRoute, 0, 37000).compare("D3") == 0);
            ASSERT_EQ(repo->outEast | repo->outSouthEast, repo->GetSectorExitPoint("MOTOX").GetOutDirection());

            // RINTI
            ASSERT_TRUE(repo->GetSectorExitPoint("RINTI").GetName().compare("RINTI") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("RINTI").GetIntentionCode(mockRoute, 0, 37000).compare("D4") == 0);
            ASSERT_EQ(repo->outSouthEast, repo->GetSectorExitPoint("RINTI").GetOutDirection());

            // Clacton

            // SOMVA
            ASSERT_TRUE(repo->GetSectorExitPoint("SOMVA").GetName().compare("SOMVA") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("SOMVA").GetIntentionCode(mockRoute, 0, 37000).compare("C1") == 0);
            ASSERT_EQ(repo->outEast, repo->GetSectorExitPoint("SOMVA").GetOutDirection());

            // REDFA
            ASSERT_TRUE(repo->GetSectorExitPoint("REDFA").GetName().compare("REDFA") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("REDFA").GetIntentionCode(mockRoute, 0, 37000).compare("C2") == 0);
            ASSERT_EQ(repo->outEast, repo->GetSectorExitPoint("REDFA").GetOutDirection());

            // SASKI
            ASSERT_TRUE(repo->GetSectorExitPoint("SASKI").GetName().compare("SASKI") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("SASKI").GetIntentionCode(mockRoute, 0, 37000).compare("C3") == 0);
            ASSERT_EQ(repo->outEast, repo->GetSectorExitPoint("SASKI").GetOutDirection());

            // North Sea

            // LONAM
            ASSERT_TRUE(repo->GetSectorExitPoint("LONAM").GetName().compare("LONAM") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LONAM").GetIntentionCode(mockRoute, 0, 37000).compare("F") == 0);
            ASSERT_EQ(repo->outEast | repo->outSouthEast, repo->GetSectorExitPoint("LONAM").GetOutDirection());

            // TOPPA
            ASSERT_TRUE(repo->GetSectorExitPoint("TOPPA").GetName().compare("TOPPA") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("TOPPA").GetIntentionCode(mockRoute, 0, 37000).compare("F") == 0);
            ASSERT_EQ(repo->outEast, repo->GetSectorExitPoint("TOPPA").GetOutDirection());

            // ROKAN
            ASSERT_TRUE(repo->GetSectorExitPoint("ROKAN").GetName().compare("ROKAN") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ROKAN").GetIntentionCode(mockRoute, 0, 37000).compare("F") == 0);
            ASSERT_EQ(repo->outEast, repo->GetSectorExitPoint("ROKAN").GetOutDirection());

            // LAMSO
            ASSERT_TRUE(repo->GetSectorExitPoint("LAMSO").GetName().compare("LAMSO") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LAMSO").GetIntentionCode(mockRoute, 0, 37000).compare("F") == 0);
            ASSERT_EQ(repo->outEast, repo->GetSectorExitPoint("LAMSO").GetOutDirection());

            // GODOS
            ASSERT_TRUE(repo->GetSectorExitPoint("GODOS").GetName().compare("GODOS") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("GODOS").GetIntentionCode(mockRoute, 0, 37000).compare("F") == 0);
            ASSERT_EQ(repo->outEast, repo->GetSectorExitPoint("GODOS").GetOutDirection());

            // MOLIX
            ASSERT_TRUE(repo->GetSectorExitPoint("MOLIX").GetName().compare("MOLIX") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("MOLIX").GetIntentionCode(mockRoute, 0, 37000).compare("F") == 0);
            ASSERT_EQ(repo->outEast, repo->GetSectorExitPoint("MOLIX").GetOutDirection());

            // Copenhagen

            // VAXIT
            ASSERT_TRUE(repo->GetSectorExitPoint("VAXIT").GetName().compare("VAXIT") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("VAXIT").GetIntentionCode(mockRoute, 0, 37000).compare("K1") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("VAXIT").GetOutDirection());

            // TINAC
            ASSERT_TRUE(repo->GetSectorExitPoint("TINAC").GetName().compare("TINAC") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("TINAC").GetIntentionCode(mockRoute, 0, 37000).compare("K2") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("TINAC").GetOutDirection());

            // GOREV
            ASSERT_TRUE(repo->GetSectorExitPoint("GOREV").GetName().compare("GOREV") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("GOREV").GetIntentionCode(mockRoute, 0, 37000).compare("K2") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("GOREV").GetOutDirection());

            // PETIL
            ASSERT_TRUE(repo->GetSectorExitPoint("PETIL").GetName().compare("PETIL") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("PETIL").GetIntentionCode(mockRoute, 0, 37000).compare("K2") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("PETIL").GetOutDirection());

            // INBOB
            ASSERT_TRUE(repo->GetSectorExitPoint("INBOB").GetName().compare("INBOB") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("INBOB").GetIntentionCode(mockRoute, 0, 37000).compare("K3") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("INBOB").GetOutDirection());

            // LESRA
            ASSERT_TRUE(repo->GetSectorExitPoint("LESRA").GetName().compare("LESRA") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LESRA").GetIntentionCode(mockRoute, 0, 37000).compare("K3") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("LESRA").GetOutDirection());

            // SOPTO
            ASSERT_TRUE(repo->GetSectorExitPoint("SOPTO").GetName().compare("SOPTO") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("SOPTO").GetIntentionCode(mockRoute, 0, 37000).compare("K3") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("SOPTO").GetOutDirection());

            // GOLUM
            ASSERT_TRUE(repo->GetSectorExitPoint("GOLUM").GetName().compare("GOLUM") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("GOLUM").GetIntentionCode(mockRoute, 0, 37000).compare("K3") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("GOLUM").GetOutDirection());

            // LARGA
            ASSERT_TRUE(repo->GetSectorExitPoint("LARGA").GetName().compare("LARGA") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LARGA").GetIntentionCode(mockRoute, 0, 37000).compare("K3") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("LARGA").GetOutDirection());

            // Stavanger

            // PEPIN
            ASSERT_TRUE(repo->GetSectorExitPoint("PEPIN").GetName().compare("PEPIN") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("PEPIN").GetIntentionCode(mockRoute, 0, 37000).compare("Z1") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("PEPIN").GetOutDirection());

            // ORVIK
            ASSERT_TRUE(repo->GetSectorExitPoint("ORVIK").GetName().compare("ORVIK") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ORVIK").GetIntentionCode(mockRoute, 0, 37000).compare("Z2") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("ORVIK").GetOutDirection());

            // KLONN
            ASSERT_TRUE(repo->GetSectorExitPoint("KLONN").GetName().compare("KLONN") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("KLONN").GetIntentionCode(mockRoute, 0, 37000).compare("Z3") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("KLONN").GetOutDirection());

            // ALOTI
            ASSERT_TRUE(repo->GetSectorExitPoint("ALOTI").GetName().compare("ALOTI") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ALOTI").GetIntentionCode(mockRoute, 0, 37000).compare("Z4") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("ALOTI").GetOutDirection());

            // NIVUN
            ASSERT_TRUE(repo->GetSectorExitPoint("NIVUN").GetName().compare("NIVUN") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("NIVUN").GetIntentionCode(mockRoute, 0, 37000).compare("Z5") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("NIVUN").GetOutDirection());

            // BEREP
            ASSERT_TRUE(repo->GetSectorExitPoint("BEREP").GetName().compare("BEREP") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("BEREP").GetIntentionCode(mockRoute, 0, 37000).compare("Z6") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("BEREP").GetOutDirection());

            // RIGVU
            ASSERT_TRUE(repo->GetSectorExitPoint("RIGVU").GetName().compare("RIGVU") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("RIGVU").GetIntentionCode(mockRoute, 0, 37000).compare("Z7") == 0);
            ASSERT_EQ(repo->outEast | repo->outNorthEast, repo->GetSectorExitPoint("RIGVU").GetOutDirection());

            // Reykjavik

            // RATSU
            ASSERT_TRUE(repo->GetSectorExitPoint("RATSU").GetName().compare("RATSU") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("RATSU").GetIntentionCode(mockRoute, 0, 37000).compare("R") == 0);
            ASSERT_EQ(repo->outNorth | repo->outNorthWest, repo->GetSectorExitPoint("RATSU").GetOutDirection());

            // MATIK
            ASSERT_TRUE(repo->GetSectorExitPoint("MATIK").GetName().compare("MATIK") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("MATIK").GetIntentionCode(mockRoute, 0, 37000).compare("R1") == 0);
            ASSERT_EQ(repo->outNorth, repo->GetSectorExitPoint("MATIK").GetOutDirection());

            // NALAN
            ASSERT_TRUE(repo->GetSectorExitPoint("NALAN").GetName().compare("NALAN") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("NALAN").GetIntentionCode(mockRoute, 0, 37000).compare("R2") == 0);
            ASSERT_EQ(repo->outNorth, repo->GetSectorExitPoint("NALAN").GetOutDirection());

            // OSBON
            ASSERT_TRUE(repo->GetSectorExitPoint("OSBON").GetName().compare("OSBON") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("OSBON").GetIntentionCode(mockRoute, 0, 37000).compare("R3") == 0);
            ASSERT_EQ(repo->outNorth, repo->GetSectorExitPoint("OSBON").GetOutDirection());

            // PEMOS
            ASSERT_TRUE(repo->GetSectorExitPoint("PEMOS").GetName().compare("PEMOS") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("PEMOS").GetIntentionCode(mockRoute, 0, 37000).compare("R5") == 0);
            ASSERT_EQ(repo->outNorth, repo->GetSectorExitPoint("PEMOS").GetOutDirection());

            // SOSAR
            ASSERT_TRUE(repo->GetSectorExitPoint("SOSAR").GetName().compare("SOSAR") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("SOSAR").GetIntentionCode(mockRoute, 0, 37000).compare("R7") == 0);
            ASSERT_EQ(repo->outNorth, repo->GetSectorExitPoint("SOSAR").GetOutDirection());

            // GUNPA
            ASSERT_TRUE(repo->GetSectorExitPoint("GUNPA").GetName().compare("GUNPA") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("GUNPA").GetIntentionCode(mockRoute, 0, 37000).compare("R8") == 0);
            ASSERT_EQ(repo->outNorth, repo->GetSectorExitPoint("GUNPA").GetOutDirection());

            //// Shanwick

            // LUSEN
            ASSERT_TRUE(repo->GetSectorExitPoint("LUSEN").GetName().compare("LUSEN") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LUSEN").GetIntentionCode(mockRoute, 0, 37000).compare("Y8") == 0);
            ASSERT_EQ(repo->outNorthWest | repo->outNorth, repo->GetSectorExitPoint("LUSEN").GetOutDirection());

            // ATSIX
            ASSERT_TRUE(repo->GetSectorExitPoint("ATSIX").GetName().compare("ATSIX") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ATSIX").GetIntentionCode(mockRoute, 0, 37000).compare("Y7") == 0);
            ASSERT_EQ(repo->outWest | repo->outNorthWest, repo->GetSectorExitPoint("ATSIX").GetOutDirection());

            // ORTAV
            ASSERT_TRUE(repo->GetSectorExitPoint("ORTAV").GetName().compare("ORTAV") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ORTAV").GetIntentionCode(mockRoute, 0, 37000).compare("Y6") == 0);
            ASSERT_EQ(repo->outWest | repo->outNorthWest, repo->GetSectorExitPoint("ORTAV").GetOutDirection());

            // BALIX
            ASSERT_TRUE(repo->GetSectorExitPoint("BALIX").GetName().compare("BALIX") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("BALIX").GetIntentionCode(mockRoute, 0, 37000).compare("Y5") == 0);
            ASSERT_EQ(repo->outWest | repo->outNorthWest, repo->GetSectorExitPoint("BALIX").GetOutDirection());

            // ADODO
            ASSERT_TRUE(repo->GetSectorExitPoint("ADODO").GetName().compare("ADODO") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ADODO").GetIntentionCode(mockRoute, 0, 37000).compare("Y4") == 0);
            ASSERT_EQ(repo->outWest | repo->outNorthWest, repo->GetSectorExitPoint("ADODO").GetOutDirection());

            // ERAKA
            ASSERT_TRUE(repo->GetSectorExitPoint("ERAKA").GetName().compare("ERAKA") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ERAKA").GetIntentionCode(mockRoute, 0, 37000).compare("Y3") == 0);
            ASSERT_EQ(repo->outWest | repo->outNorthWest, repo->GetSectorExitPoint("ERAKA").GetOutDirection());

            // ETILO
            ASSERT_TRUE(repo->GetSectorExitPoint("ETILO").GetName().compare("ETILO") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ETILO").GetIntentionCode(mockRoute, -1, 37000).compare("Y2") == 0);
            ASSERT_EQ(repo->outWest | repo->outNorthWest, repo->GetSectorExitPoint("ETILO").GetOutDirection());

            // Shanwick NOTA

            // GOMUP
            ASSERT_TRUE(repo->GetSectorExitPoint("GOMUP").GetName().compare("GOMUP") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("GOMUP").GetIntentionCode(mockRoute, -1, 37000).compare("Y1") == 0);
            ASSERT_EQ(repo->outWest | repo->outNorthWest, repo->GetSectorExitPoint("GOMUP").GetOutDirection());

            // IBROD
            ASSERT_TRUE(repo->GetSectorExitPoint("IBROD").GetName().compare("IBROD") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("IBROD").GetIntentionCode(mockRoute, 0, 37000).compare("N7") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("IBROD").GetOutDirection());

            // AMLAD
            ASSERT_TRUE(repo->GetSectorExitPoint("AMLAD").GetName().compare("AMLAD") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("AMLAD").GetIntentionCode(mockRoute, -1, 37000).compare("N6") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("AMLAD").GetOutDirection());

            // MIMKU
            ASSERT_TRUE(repo->GetSectorExitPoint("MIMKU").GetName().compare("MIMKU") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("MIMKU").GetIntentionCode(mockRoute, -1, 37000).compare("N5") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("MIMKU").GetOutDirection());

            // APSOV
            ASSERT_TRUE(repo->GetSectorExitPoint("APSOV").GetName().compare("APSOV") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("APSOV").GetIntentionCode(mockRoute, -1, 37000).compare("N4") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("APSOV").GetOutDirection());

            // KUGUR
            ASSERT_TRUE(repo->GetSectorExitPoint("KUGUR").GetName().compare("KUGUR") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("KUGUR").GetIntentionCode(mockRoute, -1, 37000).compare("N3") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("KUGUR").GetOutDirection());

            // LUTOV
            ASSERT_TRUE(repo->GetSectorExitPoint("LUTOV").GetName().compare("LUTOV") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LUTOV").GetIntentionCode(mockRoute, -1, 37000).compare("N2") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("LUTOV").GetOutDirection());

            // NIBOG
            ASSERT_TRUE(repo->GetSectorExitPoint("NIBOG").GetName().compare("NIBOG") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("NIBOG").GetIntentionCode(mockRoute, -1, 37000).compare("N1") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("NIBOG").GetOutDirection());

            // Strumble

            // VATRY
            ASSERT_TRUE(repo->GetSectorExitPoint("VATRY").GetName().compare("VATRY") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("VATRY").GetIntentionCode(mockRoute, 0, 37000).compare("S4") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("VATRY").GetOutDirection());

            // BAKUR
            ASSERT_TRUE(repo->GetSectorExitPoint("BAKUR").GetName().compare("BAKUR") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("BAKUR").GetIntentionCode(mockRoute, 0, 37000).compare("S3") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("BAKUR").GetOutDirection());

            // SLANY
            ASSERT_TRUE(repo->GetSectorExitPoint("SLANY").GetName().compare("SLANY") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("SLANY").GetIntentionCode(mockRoute, 0, 37000).compare("S2") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("SLANY").GetOutDirection());

            // BANBA
            ASSERT_TRUE(repo->GetSectorExitPoint("BANBA").GetName().compare("BANBA") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("BANBA").GetIntentionCode(mockRoute, 0, 37000).compare("S") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("BANBA").GetOutDirection());

            // EVRIN
            ASSERT_TRUE(repo->GetSectorExitPoint("EVRIN").GetName().compare("EVRIN") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("EVRIN").GetIntentionCode(mockRoute, 0, 37000).compare("S") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("EVRIN").GetOutDirection());

            // Irish Overflights

            // MOLAK
            ASSERT_TRUE(repo->GetSectorExitPoint("MOLAK").GetName().compare("MOLAK") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("MOLAK").GetIntentionCode(mockRoute, 0, 37000).compare("G1") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("MOLAK").GetOutDirection());

            // NIPIT
            ASSERT_TRUE(repo->GetSectorExitPoint("NIPIT").GetName().compare("NIPIT") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("NIPIT").GetIntentionCode(mockRoute, 0, 37000).compare("G2") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("NIPIT").GetOutDirection());

            // ERNAN
            ASSERT_TRUE(repo->GetSectorExitPoint("ERNAN").GetName().compare("ERNAN") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ERNAN").GetIntentionCode(mockRoute, 0, 37000).compare("G3") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("ERNAN").GetOutDirection());

            // DEGOS
            ASSERT_TRUE(repo->GetSectorExitPoint("DEGOS").GetName().compare("DEGOS") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("DEGOS").GetIntentionCode(mockRoute, 0, 37000).compare("G4") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("DEGOS").GetOutDirection());

            // NIMAT
            ASSERT_TRUE(repo->GetSectorExitPoint("NIMAT").GetName().compare("NIMAT") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("NIMAT").GetIntentionCode(mockRoute, 0, 37000).compare("G5") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("NIMAT").GetOutDirection());

            // NEVRI
            ASSERT_TRUE(repo->GetSectorExitPoint("NEVRI").GetName().compare("NEVRI") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("NEVRI").GetIntentionCode(mockRoute, 0, 37000).compare("G7") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("NEVRI").GetOutDirection());

            // RUBEX
            ASSERT_TRUE(repo->GetSectorExitPoint("RUBEX").GetName().compare("RUBEX") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("RUBEX").GetIntentionCode(mockRoute, 0, 37000).compare("G9") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("RUBEX").GetOutDirection());

            // Brest

            // ANNET
            ASSERT_TRUE(repo->GetSectorExitPoint("ANNET").GetName().compare("ANNET") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ANNET").GetIntentionCode(mockRoute, 0, 37000).compare("B4") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("ANNET").GetOutDirection());

            // LIZAD
            ASSERT_TRUE(repo->GetSectorExitPoint("LIZAD").GetName().compare("LIZAD") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LIZAD").GetIntentionCode(mockRoute, 0, 37000).compare("B4") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("LIZAD").GetOutDirection());

            // GANTO
            ASSERT_TRUE(repo->GetSectorExitPoint("GANTO").GetName().compare("GANTO") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("GANTO").GetIntentionCode(mockRoute, 0, 37000).compare("B5") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("GANTO").GetOutDirection());

            // SUPAP
            ASSERT_TRUE(repo->GetSectorExitPoint("SUPAP").GetName().compare("SUPAP") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("SUPAP").GetIntentionCode(mockRoute, 0, 37000).compare("B6") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("SUPAP").GetOutDirection());

            // PEMAK
            ASSERT_TRUE(repo->GetSectorExitPoint("PEMAK").GetName().compare("PEMAK") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("PEMAK").GetIntentionCode(mockRoute, 0, 37000).compare("B7") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("PEMAK").GetOutDirection());

            // SALCO
            ASSERT_TRUE(repo->GetSectorExitPoint("SALCO").GetName().compare("SALCO") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("SALCO").GetIntentionCode(mockRoute, 0, 37000).compare("B3") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("SALCO").GetOutDirection());

            // MANIG
            ASSERT_TRUE(repo->GetSectorExitPoint("MANIG").GetName().compare("MANIG") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("MANIG").GetIntentionCode(mockRoute, 0, 37000).compare("B2") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("MANIG").GetOutDirection());

            // SKESO
            ASSERT_TRUE(repo->GetSectorExitPoint("SKESO").GetName().compare("SKESO") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("SKESO").GetIntentionCode(mockRoute, 0, 37000).compare("B") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("SKESO").GetOutDirection());

            // SKERY
            ASSERT_TRUE(repo->GetSectorExitPoint("SKERY").GetName().compare("SKERY") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("SKERY").GetIntentionCode(mockRoute, 0, 37000).compare("B") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("SKERY").GetOutDirection());

            // Hern

            // ORTAC
            ASSERT_TRUE(repo->GetSectorExitPoint("ORTAC").GetName().compare("ORTAC") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ORTAC").GetIntentionCode(mockRoute, -1, 37000).compare("H") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("ORTAC").GetOutDirection());

            // LORKU
            ASSERT_TRUE(repo->GetSectorExitPoint("LORKU").GetName().compare("LORKU") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LORKU").GetIntentionCode(mockRoute, -1, 37000).compare("H9") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("LORKU").GetOutDirection());

            // LELNA
            ASSERT_TRUE(repo->GetSectorExitPoint("LELNA").GetName().compare("LELNA") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LELNA").GetIntentionCode(mockRoute, -1, 37000).compare("H8") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("LELNA").GetOutDirection());

            // Paris

            // NEVIL
            ASSERT_TRUE(repo->GetSectorExitPoint("NEVIL").GetName().compare("NEVIL") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("NEVIL").GetIntentionCode(mockRoute, 0, 37000).compare("W") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("NEVIL").GetOutDirection());

            // ANGLO
            ASSERT_TRUE(repo->GetSectorExitPoint("ANGLO").GetName().compare("ANGLO") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ANGLO").GetIntentionCode(mockRoute, 0, 37000).compare("W") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("ANGLO").GetOutDirection());

            // ETRAT
            ASSERT_TRUE(repo->GetSectorExitPoint("ETRAT").GetName().compare("ETRAT") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ETRAT").GetIntentionCode(mockRoute, 0, 37000).compare("E") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("ETRAT").GetOutDirection());

            // VEULE
            ASSERT_TRUE(repo->GetSectorExitPoint("XAMAB").GetName().compare("XAMAB") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("XAMAB").GetIntentionCode(mockRoute, 0, 37000).compare("V") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("XAMAB").GetOutDirection());

            // PETAX
            ASSERT_TRUE(repo->GetSectorExitPoint("PETAX").GetName().compare("PETAX") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("PETAX").GetIntentionCode(mockRoute, 0, 37000).compare("P") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("PETAX").GetOutDirection());

            // Ireland

            // BAGSO
            ASSERT_TRUE(repo->GetSectorExitPoint("BAGSO").GetName().compare("BAGSO") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("BAGSO").GetIntentionCode(mockRoute, 0, 37000).compare("L") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("BAGSO").GetOutDirection());

            // BOYNE
            ASSERT_TRUE(repo->GetSectorExitPoint("BOYNE").GetName().compare("BOYNE") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("BOYNE").GetIntentionCode(mockRoute, 0, 37000).compare("L") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("BOYNE").GetOutDirection());

            // LIPGO
            ASSERT_TRUE(repo->GetSectorExitPoint("LIPGO").GetName().compare("LIPGO") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LIPGO").GetIntentionCode(mockRoute, 0, 37000).compare("T") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("LIPGO").GetOutDirection());

            // MORAG
            ASSERT_TRUE(repo->GetSectorExitPoint("MORAG").GetName().compare("MORAG") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("MORAG").GetIntentionCode(mockRoute, 0, 37000).compare("T") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("MORAG").GetOutDirection());

            // Irish Sea

            // LEDGO
            ASSERT_TRUE(repo->GetSectorExitPoint("LEDGO").GetName().compare("LEDGO") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LEDGO").GetIntentionCode(mockRoute, 0, 37000).compare("M1") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("LEDGO").GetOutDirection());

            // MOPAT
            ASSERT_TRUE(repo->GetSectorExitPoint("MOPAT").GetName().compare("MOPAT") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("MOPAT").GetIntentionCode(mockRoute, 0, 37000).compare("M1") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("MOPAT").GetOutDirection());

            // LESLU
            ASSERT_TRUE(repo->GetSectorExitPoint("LESLU").GetName().compare("LESLU") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LESLU").GetIntentionCode(mockRoute, 0, 37000).compare("M") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("LESLU").GetOutDirection());

            // NORLA
            ASSERT_TRUE(repo->GetSectorExitPoint("NORLA").GetName().compare("NORLA") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("NORLA").GetIntentionCode(mockRoute, 0, 37000).compare("M") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("NORLA").GetOutDirection());

            // SAMON
            ASSERT_TRUE(repo->GetSectorExitPoint("SAMON").GetName().compare("SAMON") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("SAMON").GetIntentionCode(mockRoute, 0, 37000).compare("M") == 0);
            ASSERT_EQ(repo->outWest, repo->GetSectorExitPoint("SAMON").GetOutDirection());

            // Lands End

            // ARKIL
            ASSERT_TRUE(repo->GetSectorExitPoint("ARKIL").GetName().compare("ARKIL") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ARKIL").GetIntentionCode(mockRoute, 0, 37000).compare("A1") == 0);
            ASSERT_EQ(repo->outWest | repo->outSouthWest, repo->GetSectorExitPoint("ARKIL").GetOutDirection());

            // LULOX
            ASSERT_TRUE(repo->GetSectorExitPoint("LULOX").GetName().compare("LULOX") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LULOX").GetIntentionCode(mockRoute, 0, 37000).compare("A1") == 0);
            ASSERT_EQ(repo->outWest | repo->outSouthWest, repo->GetSectorExitPoint("LULOX").GetOutDirection());

            // TURLU
            ASSERT_TRUE(repo->GetSectorExitPoint("TURLU").GetName().compare("TURLU") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("TURLU").GetIntentionCode(mockRoute, 0, 37000).compare("A") == 0);
            ASSERT_EQ(repo->outWest | repo->outSouthWest, repo->GetSectorExitPoint("TURLU").GetOutDirection());

            // GAPLI
            ASSERT_TRUE(repo->GetSectorExitPoint("GAPLI").GetName().compare("GAPLI") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("GAPLI").GetIntentionCode(mockRoute, 0, 37000).compare("A") == 0);
            ASSERT_EQ(repo->outWest | repo->outSouthWest, repo->GetSectorExitPoint("GAPLI").GetOutDirection());

            // RATKA
            ASSERT_TRUE(repo->GetSectorExitPoint("RATKA").GetName().compare("RATKA") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("RATKA").GetIntentionCode(mockRoute, 0, 37000).compare("A") == 0);
            ASSERT_EQ(repo->outWest | repo->outSouthWest, repo->GetSectorExitPoint("RATKA").GetOutDirection());

            // BISKI
            ASSERT_TRUE(repo->GetSectorExitPoint("BISKI").GetName().compare("BISKI") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("BISKI").GetIntentionCode(mockRoute, 0, 37000).compare("A") == 0);
            ASSERT_EQ(repo->outWest | repo->outSouthWest, repo->GetSectorExitPoint("BISKI").GetOutDirection());

            // TAKAS
            ASSERT_TRUE(repo->GetSectorExitPoint("TAKAS").GetName().compare("TAKAS") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("TAKAS").GetIntentionCode(mockRoute, 0, 37000).compare("A") == 0);
            ASSERT_EQ(repo->outWest | repo->outSouthWest, repo->GetSectorExitPoint("TAKAS").GetOutDirection());

            // EGPX SPECIFIC

            // INKOB
            ASSERT_TRUE(repo->GetSectorExitPoint("INKOB").GetName().compare("INKOB") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("INKOB").GetIntentionCode(mockRoute, 0, 37000).compare("E4") == 0);
            ASSERT_EQ(repo->outSouthEast | repo->outEast, repo->GetSectorExitPoint("INKOB").GetOutDirection());

            // SOSIM
            ASSERT_TRUE(repo->GetSectorExitPoint("SOSIM").GetName().compare("SOSIM") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("SOSIM").GetIntentionCode(mockRoute, 0, 37000).compare("E5") == 0);
            ASSERT_EQ(repo->outSouthEast | repo->outEast, repo->GetSectorExitPoint("SOSIM").GetOutDirection());

            // KELLY
            ASSERT_TRUE(repo->GetSectorExitPoint("KELLY").GetName().compare("KELLY") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("KELLY").GetIntentionCode(mockRoute, 0, 37000).compare("E7") == 0);
            ASSERT_EQ(repo->outSouthEast | repo->outEast, repo->GetSectorExitPoint("KELLY").GetOutDirection());

            // TUPEM
            ASSERT_TRUE(repo->GetSectorExitPoint("TUPEM").GetName().compare("TUPEM") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("TUPEM").GetIntentionCode(mockRoute, 0, 37000).compare("E8") == 0);
            ASSERT_EQ(repo->outSouthEast | repo->outEast, repo->GetSectorExitPoint("TUPEM").GetOutDirection());

            // BELOX
            ASSERT_TRUE(repo->GetSectorExitPoint("BELOX").GetName().compare("BELOX") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("BELOX").GetIntentionCode(mockRoute, 0, 37000).compare("E9") == 0);
            ASSERT_EQ(repo->outSouthEast | repo->outEast, repo->GetSectorExitPoint("BELOX").GetOutDirection());

            // SUBUK
            ASSERT_TRUE(repo->GetSectorExitPoint("SUBUK").GetName().compare("SUBUK") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("SUBUK").GetIntentionCode(mockRoute, 0, 37000).compare("H6") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("SUBUK").GetOutDirection());

            // LAKEY
            ASSERT_TRUE(repo->GetSectorExitPoint("LAKEY").GetName().compare("LAKEY") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("LAKEY").GetIntentionCode(mockRoute, 0, 37000).compare("H7") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("LAKEY").GetOutDirection());

            // BINTI
            ASSERT_TRUE(repo->GetSectorExitPoint("BINTI").GetName().compare("BINTI") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("BINTI").GetIntentionCode(mockRoute, 0, 37000).compare("U1") == 0);
            ASSERT_EQ(repo->outSouth | repo->outSouthWest, repo->GetSectorExitPoint("BINTI").GetOutDirection());

            // TILNI
            ASSERT_TRUE(repo->GetSectorExitPoint("TILNI").GetName().compare("TILNI") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("TILNI").GetIntentionCode(mockRoute, 0, 37000).compare("U2") == 0);
            ASSERT_EQ(repo->outSouth | repo->outSouthWest, repo->GetSectorExitPoint("TILNI").GetOutDirection());

            // ERKIT
            ASSERT_TRUE(repo->GetSectorExitPoint("ERKIT").GetName().compare("ERKIT") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ERKIT").GetIntentionCode(mockRoute, 0, 37000).compare("U3") == 0);
            ASSERT_EQ(repo->outSouth | repo->outSouthEast, repo->GetSectorExitPoint("ERKIT").GetOutDirection());

            // ROVNI
            ASSERT_TRUE(repo->GetSectorExitPoint("ROVNI").GetName().compare("ROVNI") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ROVNI").GetIntentionCode(mockRoute, 0, 37000).compare("U4") == 0);
            ASSERT_EQ(repo->outSouth | repo->outSouthWest, repo->GetSectorExitPoint("ROVNI").GetOutDirection());

            // ELNAB
            ASSERT_TRUE(repo->GetSectorExitPoint("ELNAB").GetName().compare("ELNAB") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ELNAB").GetIntentionCode(mockRoute, 0, 37000).compare("U5") == 0);
            ASSERT_EQ(repo->outSouth, repo->GetSectorExitPoint("ELNAB").GetOutDirection());

            // ADGEG
            ASSERT_TRUE(repo->GetSectorExitPoint("ADGEG").GetName().compare("ADGEG") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("ADGEG").GetIntentionCode(mockRoute, 0, 37000).compare("U6") == 0);
            ASSERT_EQ(repo->outEast | repo->outSouthEast, repo->GetSectorExitPoint("ADGEG").GetOutDirection());

            // AKOKO
            ASSERT_TRUE(repo->GetSectorExitPoint("AKOKO").GetName().compare("AKOKO") == 0);
            ASSERT_TRUE(repo->GetSectorExitPoint("AKOKO").GetIntentionCode(mockRoute, 0, 37000).compare("U7") == 0);
            ASSERT_EQ(repo->outEast | repo->outSouthEast, repo->GetSectorExitPoint("AKOKO").GetOutDirection());
        }
    } // namespace IntentionCode
} // namespace UKControllerPluginTest
