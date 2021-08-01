set(PROJECT_NAME UKControllerCoreTest)

################################################################################
# Source groups
################################################################################
set(test__airfield
    "airfield/AirfieldCollectionFactoryTest.cpp"
    "airfield/AirfieldCollectionTest.cpp"
    "airfield/AirfieldModelTest.cpp"
    "airfield/NormaliseSidTest.cpp"
)
source_group("test\\airfield" FILES ${test__airfield})

set(test__api
    "api/ApiConfigurationMenuItemTest.cpp"
)
source_group("test\\api" FILES ${test__api})

set(test__bootstrap
    "bootstrap/BootstrapWarningMessageTest.cpp"
    "bootstrap/CollectionBootstrapTest.cpp"
    "bootstrap/EventHandlerCollectionBootstrapTest.cpp"
    "bootstrap/ExternalsBootstrapTest.cpp"
    "bootstrap/HelperBootstrapTest.cpp"
)
source_group("test\\bootstrap" FILES ${test__bootstrap})

set(test__command
    "command/CommandHandlerCollectionTest.cpp"
)
source_group("test\\command" FILES ${test__command})

set(test__components
    "components/BrushSwitcherTest.cpp"
    "components/ButtonTest.cpp"
    "components/ClickableAreaTest.cpp"
    "components/TitleBarTest.cpp"
)
source_group("test\\components" FILES ${test__components})

set(test__controller
    "controller/ActiveCallsignCollectionTest.cpp"
    "controller/ActiveCallsignMonitorTest.cpp"
    "controller/ActiveCallsignTest.cpp"
    "controller/ControllerBootstrapTest.cpp"
    "controller/ControllerPositionCollectionFactoryTest.cpp"
    "controller/ControllerPositionCollectionTest.cpp"
    "controller/ControllerPositionHierarchyFactoryTest.cpp"
    "controller/ControllerPositionHierarchyTest.cpp"
    "controller/ControllerPositionParserTest.cpp"
    "controller/ControllerPositionTest.cpp"
    "controller/ControllerStatusEventHandlerCollectionTest.cpp"
    "controller/HandoffEventHandlerCollectionTest.cpp"
    "controller/TranslateFrequencyAbbreviationTest.cpp"
)
source_group("test\\controller" FILES ${test__controller})

set(test__countdown
    "countdown/CountdownModuleTest.cpp"
    "countdown/CountdownRendererTest.cpp"
    "countdown/CountdownTimerTest.cpp"
    "countdown/GlobalCountdownSettingsFunctionsTest.cpp"
    "countdown/TimerConfigurationManagerTest.cpp"
    "countdown/TimerConfigurationTest.cpp"
)
source_group("test\\countdown" FILES ${test__countdown})

set(test__datablock
    "datablock/DatablockBootstrapTest.cpp"
    "datablock/DatablockFunctionsTest.cpp"
    "datablock/DisplayTimeTest.cpp"
)
source_group("test\\datablock" FILES ${test__datablock})

set(test__dependency
    "dependency/DependencyLoaderTest.cpp"
    "dependency/UpdateDependenciesTest.cpp"
)
source_group("test\\dependency" FILES ${test__dependency})

set(test__euroscope
    "euroscope/AsrEventHandlerCollectionTest.cpp"
    "euroscope/CallbackFunctionTest.cpp"
    "euroscope/CompareFunctionsTest.cpp"
    "euroscope/GeneralSettingsConfigurationBootstrapTest.cpp"
    "euroscope/GeneralSettingsConfigurationTest.cpp"
    "euroscope/LoadDefaultUserSettingsTest.cpp"
    "euroscope/RadarTargetEventHandlerCollectionTest.cpp"
    "euroscope/RunwayDialogAwareCollectionTest.cpp"
    "euroscope/UserSettingAwareCollectionTest.cpp"
    "euroscope/UserSettingTest.cpp"
)
source_group("test\\euroscope" FILES ${test__euroscope})

set(test__flightinformationservice
    "flightinformationservice/FlightInformationServiceModuleTest.cpp"
    "flightinformationservice/FlightInformationServiceTagItemTest.cpp"
)
source_group("test\\flightinformationservice" FILES ${test__flightinformationservice})

set(test__flightplan
    "flightplan/FlightPlanEventHandlerCollectionTest.cpp"
    "flightplan/FlightplanStorageBootstrapTest.cpp"
    "flightplan/StoredFlightplanCollectionTest.cpp"
    "flightplan/StoredFlightplanEventHandlerTest.cpp"
    "flightplan/StoredFlightplanTest.cpp"
)
source_group("test\\flightplan" FILES ${test__flightplan})

set(test__handoff
    "handoff/HandoffCollectionFactoryTest.cpp"
    "handoff/HandoffCollectionTest.cpp"
    "handoff/HandoffEventHandlerTest.cpp"
    "handoff/HandoffFrequencyUpdatedEventTest.cpp"
    "handoff/HandoffModuleTest.cpp"
)
source_group("test\\handoff" FILES ${test__handoff})

set(test__historytrail
    "historytrail/AircraftHistoryTrailTest.cpp"
    "historytrail/HistoryTrailEventHandlerTest.cpp"
    "historytrail/HistoryTrailModuleTest.cpp"
    "historytrail/HistoryTrailRendererTest.cpp"
    "historytrail/HistoryTrailRepositoryTest.cpp"
)
source_group("test\\historytrail" FILES ${test__historytrail})

set(test__hold
    "hold/BlockedHoldLevelRestrictionSerializerTest.cpp"
    "hold/BlockedHoldLevelRestrictionTest.cpp"
    "hold/CompareHoldingAircraftTest.cpp"
    "hold/CompareHoldsDescriptionTest.cpp"
    "hold/CompareHoldsTest.cpp"
    "hold/DeemedSeparatedHoldSerializerTest.cpp"
    "hold/HoldConfigurationMenuItemTest.cpp"
    "hold/HoldDisplayFactoryTest.cpp"
    "hold/HoldDisplayFunctionsTest.cpp"
    "hold/HoldDisplayManagerTest.cpp"
    "hold/HoldDisplayTest.cpp"
    "hold/HoldEventHandlerTest.cpp"
    "hold/HoldingAircraftTest.cpp"
    "hold/HoldingDataSerializerTest.cpp"
    "hold/HoldingDataTest.cpp"
    "hold/HoldManagerTest.cpp"
    "hold/HoldModuleTest.cpp"
    "hold/HoldRendererTest.cpp"
    "hold/HoldRestrictionSerializerTest.cpp"
    "hold/HoldSelectionMenuTest.cpp"
    "hold/MinStackHoldLevelRestrictionSerializerTest.cpp"
    "hold/MinStackHoldLevelRestrictionTest.cpp"
    "hold/PublishedHoldCollectionFactoryTest.cpp"
    "hold/PublishedHoldCollectionTest.cpp"
)
source_group("test\\hold" FILES ${test__hold})

set(test__initialaltitude
    "initialaltitude/InitialAltitudeEventHandlerTest.cpp"
    "initialaltitude/InitialAltitudeModuleTest.cpp"
)
source_group("test\\initialaltitude" FILES ${test__initialaltitude})

set(test__initialheading
    "initialheading/InitialHeadingEventHandlerTest.cpp"
    "initialheading/InitialHeadingModuleTest.cpp"
)
source_group("test\\initialheading" FILES ${test__initialheading})

set(test__integration
    "integration/ClientInitialisationManagerTest.cpp"
    "integration/ExternalMessageEventHandlerTest.cpp"
    "integration/InboundIntegrationMessageHandlerTest.cpp"
    "integration/InboundMessageTest.cpp"
    "integration/InitialisationFailureMessageTest.cpp"
    "integration/InitialisationSuccessMessageTest.cpp"
    "integration/IntegrationClientManagerTest.cpp"
    "integration/IntegrationClientTest.cpp"
    "integration/IntegrationConnectionTest.cpp"
    "integration/IntegrationModuleTest.cpp"
    "integration/MessageTypeTest.cpp"
    "integration/OutboundIntegrationMessageHandlerTest.cpp"
    "integration/SocketConnectionTest.cpp"
)
source_group("test\\integration" FILES ${test__integration})

set(test__intention
    "intention/AmsterdamAirfieldGroupTest.cpp"
    "intention/BrusselsAirfieldGroupTest.cpp"
    "intention/DublinAirfieldGroupTest.cpp"
    "intention/HomeAirfieldGroupTest.cpp"
    "intention/IntentionCodeCacheTest.cpp"
    "intention/IntentionCodeEventHandlerTest.cpp"
    "intention/IntentionCodeGeneratorTest.cpp"
    "intention/IntentionCodeModuleTest.cpp"
    "intention/IntentionCodeUpdatedMessageTest.cpp"
    "intention/SectorExitPointEtratTest.cpp"
    "intention/SectorExitPointKonanTest.cpp"
    "intention/SectorExitPointLelnaTest.cpp"
    "intention/SectorExitPointScottishTest.cpp"
    "intention/SectorExitPointTest.cpp"
    "intention/SectorExitPointVeuleTest.cpp"
    "intention/SectorExitRepositoryTest.cpp"
    "intention/ShannonAirfieldGroupTest.cpp"
)
source_group("test\\intention" FILES ${test__intention})

set(test__login
    "login/LoginModuleTest.cpp"
    "login/LoginTest.cpp"
)
source_group("test\\login" FILES ${test__login})

set(test__message
    "message/UserMessagerBootstrapTest.cpp"
    "message/UserMessagerTest.cpp"
)
source_group("test\\message" FILES ${test__message})

set(test__metar
    "metar/MetarEventHandlerCollectionTest.cpp"
    "metar/MetarParsingFunctionsTest.cpp"
    "metar/PressureChangeMessageTest.cpp"
    "metar/PressureMonitorBootstrapTest.cpp"
    "metar/PressureMonitorTest.cpp"
)
source_group("test\\metar" FILES ${test__metar})

set(test__minstack
    "minstack/MinStackManagerTest.cpp"
    "minstack/MinStackModuleTest.cpp"
    "minstack/MinStackRendererConfigurationTest.cpp"
    "minstack/MinStackRendererTest.cpp"
)
source_group("test\\minstack" FILES ${test__minstack})

set(test__mock
    "mock/MockAbstractTimedEvent.h"
    "mock/MockActiveCallsignEventHandler.h"
    "mock/MockAirfieldGroup.h"
    "mock/MockAsrEventHandlerInterface.h"
    "mock/MockConfigurableDisplay.h"
    "mock/MockConnection.h"
    "mock/MockControllerStatusEventHandlerInterface.h"
    "mock/MockDependencyLoader.h"
    "mock/MockEuroScopeCControllerInterface.h"
    "mock/MockEuroScopeCFlightplanInterface.h"
    "mock/MockEuroScopeCRadarTargetInterface.h"
    "mock/MockEuroscopeExtractedRouteInterface.h"
    "mock/MockEuroscopeFlightplanList.h"
    "mock/MockEuroscopePluginLoopbackInterface.h"
    "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
    "mock/MockEuroscopeSectorFileElementInterface.h"
    "mock/MockExternalMessageHandlerInterface.h"
    "mock/MockFlightPlanEventHandlerInterface.h"
    "mock/MockFlightplanRadarTargetPair.h"
    "mock/MockGraphicsInterface.h"
    "mock/MockHistoryTrailDialog.h"
    "mock/MockHistoryTrailRepository.h"
    "mock/MockInboundIntegrationMessageProcessor.h"
    "mock/MockMinStack.h"
    "mock/MockOutboundIntegrationEventHandler.h"
    "mock/MockPushEventConnection.h"
    "mock/MockPushEventProcessor.h"
    "mock/MockRadarRendererableInterface.h"
    "mock/MockRadarTargetEventHandlerInterface.h"
    "mock/MockRunwayDialogAwareInterface.h"
    "mock/MockSectorFileProviderInterface.h"
    "mock/MockSocket.h"
    "mock/MockSquawkEventHandler.h"
    "mock/MockTaskRunnerInterface.h"
    "mock/MockUserSettingAwareInterface.h"
    "mock/MockUserSettingProviderInterface.h"
)
source_group("test\\mock" FILES ${test__mock})

set(test__navaids
    "navaids/CompareNavaidsTest.cpp"
    "navaids/NavaidCollectionTest.cpp"
    "navaids/NavaidModuleTest.cpp"
    "navaids/NavaidTest.cpp"
)
source_group("test\\navaids" FILES ${test__navaids})

set(test__notifications
    "notifications/NotificationsEventHandlerTest.cpp"
    "notifications/NotificationsMenuItemTest.cpp"
    "notifications/NotificationsModuleTest.cpp"
    "notifications/NotificationsRepositoryFactoryTest.cpp"
    "notifications/NotificationsRepositoryTest.cpp"
    "notifications/NotificationTest.cpp"
    "notifications/UnreadNotificationsMessageTest.cpp"
)
source_group("test\\notifications" FILES ${test__notifications})

set(test__oceanic
    "oceanic/OceanicEventHandlerTest.cpp"
    "oceanic/OceanicModuleTest.cpp"
)
source_group("test\\oceanic" FILES ${test__oceanic})

set(test__offblock
    "offblock/ActualOffBlockTimeBootstrapTest.cpp"
    "offblock/ActualOffBlockTimeEventHandlerTest.cpp"
    "offblock/EstimatedDepartureTimeBootstrapTest.cpp"
    "offblock/EstimatedDepartureTimeEventHanderTest.cpp"
    "offblock/EstimatedOffBlockTimeBootstrapTest.cpp"
    "offblock/EstimatedOffBlockTimeEventHandlerTest.cpp"
)
source_group("test\\offblock" FILES ${test__offblock})

set(test__ownership
    "ownership/AirfieldOwnerQueryMessageTest.cpp"
    "ownership/AirfieldOwnershipHandlerTest.cpp"
    "ownership/AirfieldOwnershipManagerTest.cpp"
    "ownership/AirfieldOwnershipModuleTest.cpp"
    "ownership/AirfieldsOwnedQueryMessageTest.cpp"
)
source_group("test\\ownership" FILES ${test__ownership})

set(test__pch
    "pch/pch.cpp"
    "pch/pch.h"
)
source_group("test\\pch" FILES ${test__pch})

set(test__plugin
    "plugin/ForceUpdateTest.cpp"
    "plugin/FunctionCallEventHandlerTest.cpp"
    "plugin/PluginChangelogTest.cpp"
    "plugin/PluginHelpPageTest.cpp"
    "plugin/PluginInformationBootstrapTest.cpp"
    "plugin/PluginInformationMessageTest.cpp"
    "plugin/PopupMenuItemTest.cpp"
    "plugin/UkPluginBootstrapTest.cpp"
)
source_group("test\\plugin" FILES ${test__plugin})

set(test__prenote
    "prenote/AbstractPrenoteTest.cpp"
    "prenote/AirfieldPairingPrenoteTest.cpp"
    "prenote/DeparturePrenoteTest.cpp"
    "prenote/PrenoteEventHandlerTest.cpp"
    "prenote/PrenoteFactoryTest.cpp"
    "prenote/PrenoteMessageTest.cpp"
    "prenote/PrenoteModuleTest.cpp"
    "prenote/PrenoteServiceFactoryTest.cpp"
    "prenote/PrenoteServiceTest.cpp"
)
source_group("test\\prenote" FILES ${test__prenote})

set(test__push
    "push/InterpretPushEventTest.cpp"
    "push/PollingPushEventConnectionTest.cpp"
    "push/PushEventBootstrapTest.cpp"
    "push/PushEventProcessorCollectionTest.cpp"
    "push/PushEventProtocolHandlerTest.cpp"
    "push/PushEventProxyConnetionTest.cpp"
    "push/PushEventProxyHandlerTest.cpp"
)
source_group("test\\push" FILES ${test__push})

set(test__radarscreen
    "radarscreen/ConfigurableDisplayCollectionTest.cpp"
    "radarscreen/PositionResetCommandTest.cpp"
    "radarscreen/RadarRenderableCollectionTest.cpp"
    "radarscreen/ScreenControlsBootstrapTest.cpp"
    "radarscreen/ScreenControlsTest.cpp"
)
source_group("test\\radarscreen" FILES ${test__radarscreen})

set(test__regional
    "regional/RegionalPressureManagerFactoryTest.cpp"
    "regional/RegionalPressureManagerTest.cpp"
    "regional/RegionalPressureModuleTest.cpp"
    "regional/RegionalPressureRendererConfigurationTest.cpp"
    "regional/RegionalPressureRendererTest.cpp"
)
source_group("test\\regional" FILES ${test__regional})

set(test__releases
    "releases/CompareDepartureReleasesTest.cpp"
    "releases/CompareEnrouteReleaseTypesTest.cpp"
    "releases/DepartureReleaseCountdownColoursTest.cpp"
    "releases/DepartureReleaseDecisionListTest.cpp"
    "releases/DepartureReleaseEventHandlerTest.cpp"
    "releases/DepartureReleaseRequestTest.cpp"
    "releases/EnrouteReleaseEventHandlerTest.cpp"
    "releases/EnrouteReleaseTypesSerializerTest.cpp"
    "releases/ReleaseModuleTest.cpp"
    "releases/ToggleDepartureReleaseDecisionListTest.cpp"
)
source_group("test\\releases" FILES ${test__releases})

set(test__sectorfile
    "sectorfile/RunwayCollectionTest.cpp"
    "sectorfile/RunwayTest.cpp"
    "sectorfile/SectorFileBootstrapTest.cpp"
    "sectorfile/SectorFileCoordinatesTest.cpp"
)
source_group("test\\sectorfile" FILES ${test__sectorfile})

set(test__sid
    "sid/SidCollectionFactoryTest.cpp"
    "sid/SidCollectionTest.cpp"
    "sid/SidModuleTest.cpp"
    "sid/StandardInstrumentDepartureTest.cpp"
)
source_group("test\\sid" FILES ${test__sid})

set(test__squawk
    "squawk/ApiSquawkAllocationHandlerTest.cpp"
    "squawk/SquawkAssignmentTest.cpp"
    "squawk/SquawkEventHandlerTest.cpp"
    "squawk/SquawkGeneratorTest.cpp"
    "squawk/SquawkModuleTest.cpp"
    "squawk/SquawkRequestTest.cpp"
)
source_group("test\\squawk" FILES ${test__squawk})

set(test__srd
    "srd/SrdModuleTest.cpp"
    "srd/SrdSearchDialogTest.cpp"
    "srd/SrdSearchHandlerTest.cpp"
)
source_group("test\\srd" FILES ${test__srd})

set(test__stands
    "stands/CompareStandsTest.cpp"
    "stands/StandEventHandlerTest.cpp"
    "stands/StandModuleTest.cpp"
    "stands/StandSerializerTest.cpp"
)
source_group("test\\stands" FILES ${test__stands})

set(test__tag
    "tag/TagDataTest.cpp"
    "tag/TagFunctionTest.cpp"
    "tag/TagItemCollectionTest.cpp"
)
source_group("test\\tag" FILES ${test__tag})

set(test__time
    "time/ParseTimeStringsTest.cpp"
    "time/SystemClockTest.cpp"
)
source_group("test\\time" FILES ${test__time})

set(test__timedevent
    "timedevent/TimedEventCollectionTest.cpp"
)
source_group("test\\timedevent" FILES ${test__timedevent})

set(test__timer
    "timer/TimerDisplayTest.cpp"
)
source_group("test\\timer" FILES ${test__timer})

set(test__wake
    "wake/CreateWakeMappingsTest.cpp"
    "wake/WakeCategoryEventHandlerTest.cpp"
    "wake/WakeCategoryMapperTest.cpp"
    "wake/WakeModuleTest.cpp"
)
source_group("test\\wake" FILES ${test__wake})

set(ALL_FILES
    ${test__airfield}
    ${test__api}
    ${test__bootstrap}
    ${test__command}
    ${test__components}
    ${test__controller}
    ${test__countdown}
    ${test__datablock}
    ${test__dependency}
    ${test__euroscope}
    ${test__flightinformationservice}
    ${test__flightplan}
    ${test__handoff}
    ${test__historytrail}
    ${test__hold}
    ${test__initialaltitude}
    ${test__initialheading}
    ${test__integration}
    ${test__intention}
    ${test__login}
    ${test__message}
    ${test__metar}
    ${test__minstack}
    ${test__mock}
    ${test__navaids}
    ${test__notifications}
    ${test__oceanic}
    ${test__offblock}
    ${test__ownership}
    ${test__pch}
    ${test__plugin}
    ${test__prenote}
    ${test__push}
    ${test__radarscreen}
    ${test__regional}
    ${test__releases}
    ${test__sectorfile}
    ${test__sid}
    ${test__squawk}
    ${test__srd}
    ${test__stands}
    ${test__tag}
    ${test__time}
    ${test__timedevent}
    ${test__timer}
    ${test__wake}
    $<TARGET_OBJECTS:UKControllerPluginCore>
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_precompile_headers(${PROJECT_NAME} PRIVATE "pch/pch.h")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE UKControllerPluginTest)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebugDLL
    >
    $<$<CONFIG:Release>:
        MultiThreadedDLL
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/plugin;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/utils;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../packages/gmock.1.10.0/lib/native/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../testingutils;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../resource;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party"
)

################################################################################
# Link directories
################################################################################
target_link_directories(
        ${PROJECT_NAME} PUBLIC
        $ENV{CURL_DEBUG_LIBRARYDIR}
        $ENV{CURL_LIBRARYDIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "WIN32;"
    "_CONSOLE;"
    "STATIC_LIBCURL;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            -Zm200
        >
        $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /Gy;
            -Zm120
        >
        /MP;
        /std:c++17;
        /sdl;
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /DEBUG:FASTLINK;
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /NODEFAULTLIB:LIBCMT;
        /SUBSYSTEM:CONSOLE;
    )
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    UKControllerPluginCore
    UKControllerPluginUtils
    GoogleMock
    UKControllerPluginTestingUtils
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    UKControllerPluginCore
    UKControllerPluginUtils
    GoogleMock
    UKControllerPluginTestingUtils
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "EuroScopePlugInDll;"
        "dbghelp;"
        "gdiplus;"
        "libcurl_a_debug"
    ">"
    "$<$<CONFIG:Release>:"
        "EuroScopePluginDll;"
        "libcurl_a"
    ">"
    "kernel32;"
    "user32;"
    "gdi32;"
    "winspool;"
    "Winmm;"
    "comdlg32;"
    "shell32;"
    "ole32;"
    "oleaut32;"
    "uuid;"
    "odbc32;"
    "odbccp32;"
    "crypt32;"
    "Ws2_32;"
    "Wldap32;"
    "normaliz;"
    "advapi32"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/lib/test"
)