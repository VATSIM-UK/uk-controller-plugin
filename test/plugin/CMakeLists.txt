set(PROJECT_NAME UKControllerPluginCoreTest)

################################################################################
# Source groups
################################################################################
set(test__aircraft
        aircraft/CallsignSelectionListTest.cpp
        aircraft/CallsignSelectionListFactoryTest.cpp
        aircraft/CallsignSelectionListFactoryBootstrapTest.cpp
        aircraft/AircraftTypeTest.cpp
        aircraft/AircraftTypeCollectionTest.cpp
        aircraft/AircraftTypeFactoryTest.cpp
        aircraft/AircraftTypeCollectionFactoryTest.cpp
        aircraft/AircraftModuleTest.cpp
        aircraft/FlightplanAircraftTypeMapperTest.cpp)
source_group("test\\aircraft" FILES ${test__aircraft})

set(test__airfield
    "airfield/AirfieldCollectionFactoryTest.cpp"
    "airfield/AirfieldCollectionTest.cpp"
    "airfield/AirfieldModelTest.cpp"
    airfield/AirfieldModuleTest.cpp)
source_group("test\\airfield" FILES ${test__airfield})

set(test__api
    "api/ApiConfigurationMenuItemTest.cpp"
        api/BootstrapApiTest.cpp api/FirstTimeApiAuthorisationCheckerTest.cpp api/FirstTimeApiConfigLoaderTest.cpp)
source_group("test\\api" FILES ${test__api})

set(test__approach
        approach/ApproachBootstrapProviderTest.cpp
        approach/ApproachSequenceTest.cpp
        approach/ApproachSequencedAircraftTest.cpp
        approach/ApproachSequencerDisplayTest.cpp
        approach/ApproachSequencerTest.cpp
        approach/ApproachSequencerDisplayOptionsTest.cpp
        approach/ApproachSequencerDisplayAsrLoaderTest.cpp
        approach/ToggleApproachSequencerDisplayTest.cpp
        approach/SequencerAirfieldSelectorTest.cpp approach/ApproachModuleFactoryTest.cpp approach/AircraftSelectionProviderTest.cpp approach/TargetSelectorListTest.cpp wake/ApproachSpacingCalculatorTest.cpp approach/ApproachSequencerOptionsTest.cpp approach/ApproachSequencerOptionsLoaderTest.cpp approach/AirfieldTargetSelectorListTest.cpp approach/ApproachSequencerDistanceOptionsTest.cpp approach/AirfieldMinimumSeparationSelectorListTest.cpp approach/RemoveLandedAircraftTest.cpp approach/ApproachFlightplanEventHandlerTest.cpp
        approach/GlideslopeDeviationEstimatorTest.cpp
        approach/GlideslopeDeviationTagItemTest.cpp)
source_group("test\\approach" FILES ${test__approach})

set(test__bootstrap
    "bootstrap/BootstrapWarningMessageTest.cpp"
    "bootstrap/CollectionBootstrapTest.cpp"
    "bootstrap/EventHandlerCollectionBootstrapTest.cpp"
    "bootstrap/ExternalsBootstrapTest.cpp"
    "bootstrap/HelperBootstrapTest.cpp"
    bootstrap/ModuleBootstrapTest.cpp
    bootstrap/ModuleFactoriesTest.cpp
    bootstrap/BootstrapProviderCollectionTest.cpp
    bootstrap/BootstrapProviderCollectionFactoryTest.cpp)
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

set(test__departure
        
        departure/DepartureCoordinationListTest.cpp departure/ToggleDepartureCoordinationListTest.cpp departure/DepartureModuleTest.cpp departure/DepartureMonitorTest.cpp departure/UserShouldClearDepartureDataMonitorTest.cpp)
source_group("test\\departure" FILES ${test__departure})

set(test__dependency
    "dependency/DependencyLoaderTest.cpp"
    "dependency/UpdateDependenciesTest.cpp"
)
source_group("test\\dependency" FILES ${test__dependency})

set(test__ecfmp ecfmp/HttpClientTest.cpp ecfmp/TriggerEcfmpEventLoopTest.cpp ecfmp/ECFMPModuleFactoryTest.cpp ecfmp/ECFMPBootstrapProviderTest.cpp ecfmp/AircraftFlowMeasureMapTest.cpp ecfmp/AircraftFlowMeasureTagItemTest.cpp ecfmp/ListAircraftFlowMeasuresTest.cpp ecfmp/HomeFirsFlowMeasureFilterTest.cpp ecfmp/ControllerFlowMeasureRelevanceTest.cpp)
source_group("test\\ecfmp" FILES ${test__ecfmp})

set(test__euroscope
    "euroscope/AsrEventHandlerCollectionTest.cpp"
    "euroscope/CallbackFunctionTest.cpp"
        "euroscope/GeneralSettingsConfigurationBootstrapTest.cpp"
    "euroscope/GeneralSettingsConfigurationTest.cpp"
    "euroscope/LoadDefaultUserSettingsTest.cpp"
    "euroscope/RadarTargetEventHandlerCollectionTest.cpp"
    "euroscope/RunwayDialogAwareCollectionTest.cpp"
    "euroscope/UserSettingAwareCollectionTest.cpp"
    "euroscope/UserSettingTest.cpp"
        euroscope/PluginSettingsProviderCollectionTest.cpp euroscope/PluginUserSettingBootstrapTest.cpp euroscope/RadarScreenCallbackFunctionTest.cpp)
source_group("test\\euroscope" FILES ${test__euroscope})

set(test__eventhandler
        eventhandler/ProcessEventBusEventsOnTimerTest.cpp eventhandler/EventBusBootstrapTest.cpp)
source_group("test\\eventhandler" FILES ${test__eventhandler})

set(test__flightinformationservice
    "flightinformationservice/FlightInformationServiceModuleTest.cpp"
    "flightinformationservice/FlightInformationServiceTagItemTest.cpp"
)
source_group("test\\flightinformationservice" FILES ${test__flightinformationservice})

set(test__flightplan
    "flightplan/FlightplanPointTest.cpp"
    "flightplan/FlightPlanEventHandlerCollectionTest.cpp"
    "flightplan/FlightplanStorageBootstrapTest.cpp"
    "flightplan/ParsedFlightplanFactoryTest.cpp"
    "flightplan/ParsedFlightplanTest.cpp"
    "flightplan/StoredFlightplanCollectionTest.cpp"
    "flightplan/StoredFlightplanEventHandlerTest.cpp"
    "flightplan/StoredFlightplanTest.cpp"
)
source_group("test\\flightplan" FILES ${test__flightplan})

set(test__flightrule
        flightrule/FlightRuleCollectionTest.cpp flightrule/FlightRuleCollectionFactoryTest.cpp flightrule/FlightRuleModuleTest.cpp)
source_group("test\\flightrule" FILES ${test__flightrule})

set(test__geometry
        geometry/LineTest.cpp geometry/DistanceRadiusToScreenRadiusTest.cpp geometry/MeasurementUnitFactoryTest.cpp
        geometry/AngleTest.cpp
        geometry/LengthTest.cpp)
source_group("test\\geometry" FILES ${test__geometry})

set(test__handoff
    "handoff/HandoffCollectionFactoryTest.cpp"
    "handoff/HandoffCollectionTest.cpp"
    "handoff/HandoffEventHandlerTest.cpp"
    "handoff/HandoffFrequencyUpdatedEventTest.cpp"
    "handoff/HandoffModuleTest.cpp"
    handoff/FlightplanSidHandoffMapperTest.cpp
    handoff/FlightplanAirfieldHandoffMapperTest.cpp
        handoff/DefaultDepartureHandoffResolutionStrategyTest.cpp
        handoff/InvalidateHandoffsOnActiveCallsignChangesTest.cpp
        handoff/InvalidateHandoffsOnRunwayDialogSaveTest.cpp handoff/DepartureHandoffIntegrationDataInitialiserTest.cpp mock/MockDepartureHandoffResolver.cpp mock/MockDepartureHandoffResolver.h handoff/SendHandoffFrequencyUpdatedIntegrationMessageTest.cpp)
source_group("test\\handoff" FILES ${test__handoff})

set(test__headings
    "headings/HeadingTest.cpp"
        headings/HeadingTest.cpp)
source_group("test\\headings" FILES ${test__headings})

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
        hold/AssignHoldCommandTest.cpp hold/AddToHoldCallsignProviderTest.cpp hold/CompareProximityHoldsTest.cpp hold/AircraftEnteredHoldingAreaEventHandlerTest.cpp hold/AircraftExitedHoldingAreaEventHandlerTest.cpp hold/ProximityHoldTest.cpp)
source_group("test\\hold" FILES ${test__hold})

set(test__initialaltitude
    "initialaltitude/InitialAltitudeEventHandlerTest.cpp"
    "initialaltitude/InitialAltitudeModuleTest.cpp"
)
source_group("test\\initialaltitude" FILES ${test__initialaltitude})

set(test__initialheading
    "initialheading/InitialHeadingEventHandlerTest.cpp"
    "initialheading/InitialHeadingModuleTest.cpp"
        initialaltitude/ClearInitialAltitudeTest.cpp initialheading/ClearInitialHeadingTest.cpp)
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
        intention/IntentionCodeBootstrapProviderTest.cpp intention/IntentionCodeModelTest.cpp)
source_group("test\\integration" FILES ${test__integration})

set(test__intention
    "intention/ExitConeTest.cpp"
    "intention/FirExitPointTest.cpp"
    "intention/IntentionCodeUpdatedMessageTest.cpp"
    intention/FirExitPointCollectionTest.cpp
    intention/CachedAircraftFirExitGeneratorTest.cpp
    intention/FirExitPointCollectionFactoryTest.cpp
    intention/IntentionCodeModuleFactoryTest.cpp
    intention/AllOfTest.cpp
    intention/AnyOfTest.cpp
    intention/NotTest.cpp
    intention/ArrivalAirfieldsTest.cpp
    intention/ArrivalAirfieldPatternTest.cpp
    intention/MaximumCruisingAltitudeTest.cpp
    intention/CruisingLevelAboveTest.cpp
    intention/ControllerPositionStartsWithTest.cpp
    intention/ExitingFirAtPointTest.cpp
    intention/RoutingViaTest.cpp
    intention/SingleCodeTest.cpp
    intention/AirfieldIdentifierTest.cpp
    intention/FullAirfieldIdentifierTest.cpp
    intention/IntentionCodeCollectionTest.cpp
    intention/IntentionCodeCollectionFactoryTest.cpp
    intention/CachedAircraftIntentionCodeGeneratorTest.cpp
    intention/IntentionCodeEventHandlerCollectionTest.cpp
    intention/SendIntentionCodeUpdatedIntegrationMessageTest.cpp
    intention/IntentionCodeTagItemTest.cpp integration/IntegrationDataInitialisersTest.cpp intention/IntentionCodeIntegrationDataInitialiserTest.cpp)
source_group("test\\intention" FILES ${test__intention})

set(test__list
        list/PopupListTest.cpp list/PopupListFactoryTest.cpp list/PopupListFactoryBootstrapTest.cpp)
source_group("test\\list" FILES ${test__list})

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
    "metar/PressureChangeMessageTest.cpp"
    "metar/PressureMonitorBootstrapTest.cpp"
    "metar/PressureMonitorTest.cpp"
    metar/PressureComponentTest.cpp
    metar/ParsedMetarTest.cpp
    metar/ParsedMetarCollectionTest.cpp
    metar/PressureComponentFactoryTest.cpp
    metar/MetarComponentsFactoryTest.cpp
    metar/ParsedMetarFactoryTest.cpp
    metar/MetarsUpdatedPushEventProcessorTest.cpp
    metar/MetarModuleTest.cpp
    metar/PressureQueryMessageTest.cpp
    metar/PressureNotFoundMessageTest.cpp
    metar/PressureQueryCommandHandlerTest.cpp)
source_group("test\\metar" FILES ${test__metar})

set(test__minstack
    "minstack/MinStackManagerTest.cpp"
    "minstack/MinStackModuleTest.cpp"
    "minstack/MinStackRendererConfigurationTest.cpp"
    "minstack/MinStackRendererTest.cpp"
)
source_group("test\\minstack" FILES ${test__minstack})

set(test__missedapproach
        missedapproach/MissedApproachTest.cpp
        missedapproach/CompareMissedApproachesTest.cpp
        missedapproach/MissedApproachCollectionTest.cpp
        missedapproach/RemoveExpiredMissedApproachesTest.cpp
        missedapproach/MissedApproachModuleTest.cpp
        missedapproach/NewMissedApproachPushEventHandlerTest.cpp
        missedapproach/TriggerMissedApproachTest.cpp
        missedapproach/MissedApproachRendererTest.cpp
        missedapproach/MissedApproachRenderOptionsTest.cpp
        missedapproach/MissedApproachOptionsTest.cpp
        missedapproach/MissedApproachAudioAlertTest.cpp
        missedapproach/MissedApproachUserSettingHandlerTest.cpp
        missedapproach/MissedApproachButtonTest.cpp
        missedapproach/ToggleMissedApproachButtonTest.cpp
        missedapproach/ConfigureMissedApproachesTest.cpp
        missedapproach/MissedApproachTriggeredMessageTest.cpp
        missedapproach/TriggerMissedApproachMessageHandlerTest.cpp
        missedapproach/MissedApproachAcknowledgedUserMessageTest.cpp
        missedapproach/MissedApproachAcknowledgedPushEventProcessorTest.cpp
        missedapproach/MissedApproachIndicatorTest.cpp
        missedapproach/AcknowledgeMissedApproachTagFunctionTest.cpp
        missedapproach/MissedApproachTriggeredMessageTest.cpp
        missedapproach/TriggerMissedApproachMessageHandlerTest.cpp)

source_group("test\\missedapproach" FILES ${test__missedapproach})

set(test__mock
    "mock/MockAbstractTimedEvent.h" mock/MockAbstractTimedEvent.cpp
    "mock/MockActiveCallsignEventHandler.h" mock/MockActiveCallsignEventHandler.cpp
    "mock/MockAsrEventHandlerInterface.h" mock/MockAsrEventHandlerInterface.cpp
    "mock/MockConfigurableDisplay.h" mock/MockConfigurableDisplay.cpp
    mock/MockCondition.h mock/MockCondition.cpp
    "mock/MockConnection.h"  mock/MockConnection.cpp
    "mock/MockControllerStatusEventHandlerInterface.h" mock/MockControllerStatusEventHandlerInterface.cpp
    "mock/MockDependencyLoader.h" mock/MockDependencyLoader.cpp
    "mock/MockEuroScopeCControllerInterface.h" mock/MockEuroScopeCControllerInterface.cpp
    "mock/MockEuroScopeCFlightplanInterface.h" mock/MockEuroScopeCFlightplanInterface.cpp
    "mock/MockEuroScopeCRadarTargetInterface.h"  mock/MockEuroScopeCRadarTargetInterface.cpp
    "mock/MockEuroscopeExtractedRouteInterface.h" mock/MockEuroscopeExtractedRouteInterface.cpp
    "mock/MockEuroscopeFlightplanList.h" mock/MockEuroscopeFlightplanList.cpp
    "mock/MockEuroscopeCoordinateInterface.h" mock/MockEuroscopeCoordinateInterface.cpp
    "mock/MockEuroscopePluginLoopbackInterface.h" mock/MockEuroscopePluginLoopbackInterface.cpp
    "mock/MockEuroscopeRadarScreenLoopbackInterface.h" mock/MockEuroscopeRadarScreenLoopbackInterface.cpp
    "mock/MockEuroscopeSectorFileElementInterface.h" mock/MockEuroscopeSectorFileElementInterface.cpp
    "mock/MockExitDetermination.h" mock/MockExitDetermination.cpp
    "mock/MockExternalMessageHandlerInterface.h"  mock/MockExternalMessageHandlerInterface.cpp
    "mock/MockFlightPlanEventHandlerInterface.h" mock/MockFlightPlanEventHandlerInterface.cpp
    "mock/MockFlightplanRadarTargetPair.h"
    "mock/MockGraphicsInterface.h" mock/MockGraphicsInterface.cpp
    "mock/MockIntegrationActionProcessor.h"  mock/MockIntegrationActionProcessor.cpp
    mock/MockMenuToggleableDisplay.h mock/MockMenuToggleableDisplay.cpp
    mock/MockMetarEventHandler.h mock/MockMetarEventHandler.cpp
    "mock/MockOutboundIntegrationEventHandler.h"  mock/MockOutboundIntegrationEventHandler.cpp
    "mock/MockPushEventConnection.h" mock/MockPushEventConnection.cpp
    "mock/MockPushEventProcessor.h" mock/MockPushEventProcessor.cpp
    "mock/MockRadarRendererableInterface.h"  mock/MockRadarRenderableInterface.cpp
    "mock/MockRadarTargetEventHandlerInterface.h" mock/MockRadarTargetEventHandlerInterface.cpp
    "mock/MockRunwayDialogAwareInterface.h" mock/MockRunwayDialogAwareInterface.cpp
    "mock/MockSectorFileProviderInterface.h" mock/MockSectorFileProviderInterface.cpp
    mock/MockSidMapperInterface.h mock/MockSidMapperInterface.cpp
    "mock/MockSocket.h" mock/MockSocket.cpp
    "mock/MockUserSettingAwareInterface.h" mock/MockUserSettingAwareInterface.cpp
    "mock/MockUserSettingProviderInterface.h" mock/MockUserSettingProviderInterface.cpp
    mock/MockCallsignSelectionProvider.h mock/MockCallsignSelectionProvider.cpp
    mock/MockAircraftTypeMapper.h mock/MockAircraftTypeMapper.cpp
    mock/MockWakeCategoryMapper.h mock/MockWakeCategoryMapper.cpp
    mock/MockListItemProvider.h mock/MockListItemProvider.cpp
    mock/MockPopupList.h mock/MockPopupList.cpp
    mock/MockBootstrapProvider.h mock/MockBootstrapProvider.cpp
    mock/MockPluginSettingsProvider.h mock/MockPluginSettingsProvider.cpp
    mock/MockPrenoteMessageEventHandlerInterface.h mock/MockPrenoteMessageEventHandlerInterface.cpp
    mock/MockPrenoteUserRelevanceChecker.h mock/MockPrenoteUserRelevanceChecker.cpp
    mock/MockAircraftFirExitGenerator.h mock/MockAircraftFirExitGenerator.cpp mock/MockIntegrationDataInitialiser.cpp mock/MockIntegrationDataInitialiser.h mock/MockAircraftIntentionCodeGenerator.cpp mock/MockAircraftIntentionCodeGenerator.h)
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

set(test__number
        number/NumberFormatTest.cpp)
source_group("test\\number" FILES ${test__number})

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
     ownership/AirfieldServiceProviderCollectionTest.cpp)
source_group("test\\ownership" FILES ${test__ownership})

set(test__plugin
    "plugin/ForceUpdateTest.cpp"
    "plugin/FunctionCallEventHandlerTest.cpp"
    "plugin/PluginChangelogTest.cpp"
    "plugin/PluginHelpPageTest.cpp"
    "plugin/PluginInformationBootstrapTest.cpp"
    "plugin/PluginInformationMessageTest.cpp"
    "plugin/PopupMenuItemTest.cpp"
    "plugin/UkPluginBootstrapTest.cpp"
        plugin/OpenDataFolderTest.cpp plugin/OpenLogsFolderTest.cpp)
source_group("test\\plugin" FILES ${test__plugin})

set(test__prenote
        "prenote/PrenoteEventHandlerTest.cpp"
        "prenote/PrenoteUserMessageTest.cpp"
        "prenote/PrenoteModuleTest.cpp"
        "prenote/PrenoteServiceTest.cpp"
        "prenote/PrenoteMessageTest.cpp"
        prenote/ComparePrenoteMessagesTest.cpp
        prenote/PrenoteMessageCollectionTest.cpp
        prenote/NewPrenotePushEventHandlerTest.cpp
        prenote/PrenoteAcknowledgedPushEventHandlerTest.cpp
        prenote/PrenoteDeletedPushEventHandlerTest.cpp
        prenote/PrenoteMessageTimeoutTest.cpp
        "prenote/PrenoteStatusIndicatorTagItemTest.cpp"
        "prenote/CancelPrenoteMessageMenuTest.cpp"
        "prenote/SendPrenoteMenuTest.cpp"
        "prenote/TriggerPrenoteMessageStatusViewTest.cpp"
        "prenote/AcknowledgePrenoteMessageTest.cpp"
        prenote/PublishedPrenoteCollectionTest.cpp
        prenote/PublishedPrenoteCollectionFactoryTest.cpp
        prenote/PublishedPrenoteMapperTest.cpp
        prenote/PrenoteMessageEventHandlerCollectionTest.cpp
        prenote/PlayNewPrenoteMessageSoundTest.cpp
        prenote/PrenoteIsTargetedToUserTest.cpp
        prenote/SendNewPrenoteChatAreaMessageTest.cpp
        prenote/SendPrenoteCancelledChatAreaMessageTest.cpp
        prenote/PrenoteIsSentFromUserPositionTest.cpp prenote/SendPrenoteAcknowledgedChatAreaMessageTest.cpp prenote/SendPrenoteTimeoutChatAreaMessageTest.cpp)
source_group("test\\prenote" FILES ${test__prenote})

set(test__push
    "push/InterpretPushEventTest.cpp"
    "push/PollingPushEventConnectionTest.cpp"
    "push/PushEventBootstrapTest.cpp"
    "push/PushEventProcessorCollectionTest.cpp"
    "push/PushEventProtocolHandlerTest.cpp"
    "push/PushEventProxyConnetionTest.cpp"
    "push/PushEventProxyHandlerTest.cpp"
    push/ProxyPushDataSyncTest.cpp)
source_group("test\\push" FILES ${test__push})

set(test__radarscreen
    "radarscreen/ConfigurableDisplayCollectionTest.cpp"
    "radarscreen/PositionResetCommandTest.cpp"
    "radarscreen/RadarRenderableCollectionTest.cpp"
    "radarscreen/ScreenControlsBootstrapTest.cpp"
    "radarscreen/ScreenControlsTest.cpp"
        radarscreen/ToggleDisplayFromMenuTest.cpp radarscreen/MenuToggleableDisplayFactoryTest.cpp radarscreen/ConfigurableDisplayCallbackFactoryTest.cpp)
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
    "releases/DepartureReleaseEventHandlerTest.cpp"
    "releases/DepartureReleaseRequestTest.cpp"
    "releases/EnrouteReleaseEventHandlerTest.cpp"
    "releases/EnrouteReleaseTypesSerializerTest.cpp"
    "releases/ReleaseModuleTest.cpp"
    releases/ReleaseApprovalRemarksUserMessageTest.cpp
    releases/ReleaseRejectionRemarksUserMessageTest.cpp releases/ReleaseIsTargetedAtUserTest.cpp releases/SendReleaseRequestedChatAreaMessageTest.cpp handoff/DefaultDepartureHandoffResolverTest.cpp)
source_group("test\\releases" FILES ${test__releases})

set(test__runway
    runway/RunwayTest.cpp
    runway/RunwayCollectionTest.cpp
    runway/RunwayCollectionFactoryTest.cpp
    runway/RunwayModuleTest.cpp)
source_group("test\\runway" FILES ${test__runway})

set(test__sectorfile
    "sectorfile/SectorfileRunwayCollectionTest.cpp"
    "sectorfile/SectorfileRunwayTest.cpp"
    "sectorfile/SectorFileBootstrapTest.cpp"
    "sectorfile/SectorFileCoordinatesTest.cpp"
)
source_group("test\\sectorfile" FILES ${test__sectorfile})

set(test__selcal
        selcal/ParsedSelcalTest.cpp
        selcal/SelcalParserTest.cpp selcal/SelcalTagItemTest.cpp selcal/SelcalModuleTest.cpp)
source_group("test\\selcal" FILES ${test__selcal})

set(test__sid
    "sid/SidCollectionFactoryTest.cpp"
    "sid/SidCollectionTest.cpp"
    "sid/SidModuleTest.cpp"
    "sid/StandardInstrumentDepartureTest.cpp"
    sid/FlightplanSidMapperTest.cpp)
source_group("test\\sid" FILES ${test__sid})

set(test__squawk
    "squawk/ApiSquawkAllocationHandlerTest.cpp"
    "squawk/SquawkAssignmentTest.cpp"
    "squawk/SquawkEventHandlerTest.cpp"
    "squawk/SquawkGeneratorTest.cpp"
    "squawk/SquawkModuleTest.cpp"
    "squawk/SquawkRequestTest.cpp"
        squawk/ResetSquawkOnFailedDeleteTest.cpp squawk/SquawkAssignmentMenuTest.cpp squawk/SquawkCodeValidTest.cpp)
source_group("test\\squawk" FILES ${test__squawk})

set(test__srd
    "srd/SrdModuleTest.cpp"
    "srd/SrdSearchDialogTest.cpp"
    "srd/SrdSearchHandlerTest.cpp"
    srd/ContainsFreeRouteAirspaceTest.cpp)
source_group("test\\srd" FILES ${test__srd})

set(test__stands
    "stands/CompareStandsTest.cpp"
    "stands/StandEventHandlerTest.cpp"
    "stands/StandModuleTest.cpp"
    "stands/StandSerializerTest.cpp"
    "stands/StandAssignedMessageTest.cpp"
    "stands/StandUnassignedMessageTest.cpp"
)
source_group("test\\stands" FILES ${test__stands})

set(test__tag
    "tag/TagDataTest.cpp"
    "tag/TagFunctionTest.cpp"
    "tag/TagItemCollectionTest.cpp"
)
source_group("test\\tag" FILES ${test__tag})

set(test__test
        test/BootstrapProviderTestCase.h test/BootstrapProviderTestCase.cpp)
source_group("test\\test" FILES ${test__test})

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
    "wake/WakeCategoryEventHandlerTest.cpp"
    "wake/WakeModuleTest.cpp"
    wake/WakeCategoryTest.cpp
    wake/WakeSchemeTest.cpp
    wake/DepartureWakeIntervalFactoryTest.cpp
    wake/WakeCategoryFactoryTest.cpp
    wake/WakeSchemeFactoryTest.cpp
    wake/WakeSchemeCollectionTest.cpp
    wake/WakeSchemeCollectionFactoryTest.cpp
    wake/FlightplanWakeCategoryMapperTest.cpp wake/FollowingWakeCallsignProviderTest.cpp wake/LeadWakeCallsignProviderTest.cpp wake/WakeCalculatorOptionsTest.cpp wake/WakeSchemeProviderTest.cpp wake/WakeCalculatorDisplayTest.cpp wake/WakeIntervalFormatterTest.cpp wake/ArrivalWakeIntervalFactoryTest.cpp wake/ArrivalWakeIntervalTest.cpp wake/DepartureWakeIntervalTest.cpp wake/WakeCategoryMapperCollectionTest.cpp wake/WakeCategoryMapperCollectionFactoryTest.cpp)
source_group("test\\wake" FILES ${test__wake})

set(ALL_FILES
    ${test__aircraft}
    ${test__airfield}
    ${test__api}
    ${test__approach}
    ${test__bootstrap}
    ${test__command}
    ${test__components}
    ${test__controller}
    ${test__countdown}
    ${test__datablock}
    ${test__departure}
    ${test__dependency}
    ${test__ecfmp}
    ${test__euroscope}
    ${test__eventhandler}
    ${test__flightinformationservice}
    ${test__flightplan}
    ${test__flightrule}
    ${test__geometry}
    ${test__handoff}
    ${test__headings}
    ${test__historytrail}
    ${test__hold}
    ${test__initialaltitude}
    ${test__initialheading}
    ${test__integration}
    ${test__intention}
    ${test__list}
    ${test__login}
    ${test__message}
    ${test__metar}
    ${test__minstack}
    ${test__missedapproach}
    ${test__mock}
    ${test__navaids}
    ${test__notifications}
    ${test__number}
    ${test__oceanic}
    ${test__offblock}
    ${test__ownership}
    ${test__plugin}
    ${test__prenote}
    ${test__push}
    ${test__radarscreen}
    ${test__regional}
    ${test__releases}
    ${test__runway}
    ${test__sectorfile}
    ${test__selcal}
    ${test__sid}
    ${test__squawk}
    ${test__srd}
    ${test__stands}
    ${test__tag}
    ${test__test}
    ${test__time}
    ${test__timedevent}
    ${test__timer}
    ${test__wake}
    $<TARGET_OBJECTS:UKControllerPluginCore> intention/IntentionCodeBootstrapProviderTest.cpp tag/RadarScreenTagFunctionTest.cpp)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

target_precompile_headers(${PROJECT_NAME} PRIVATE "pch/pch.h")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE UKControllerPluginTest)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/."
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
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
        ${CMAKE_BINARY_DIR}/lib
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
            -Zm200;
            /Zi;
        >
        $<$<CONFIG:Release>:
            -Zm120;
        >
        /O1;
        /std:c++20;
        /sdl;
        /W4;
        /WX;
        -ftime-trace;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
        -Wno-unused-parameter # Lots of interfaces don't use everything
        -Wno-missing-field-initializers # Windows has loads of this sadly
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /DEBUG:GHASH;
            /INCREMENTAL
            -flto=thin
        >
        $<$<CONFIG:Release>:
            /DEBUG:NONE;
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
    gmock
    UKControllerPluginTestingUtils
    ecfmp_sdk
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    UKControllerPluginCore
    UKControllerPluginUtils
    gmock
    UKControllerPluginTestingUtils
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "EuroScopePlugInDll;"
        "dbghelp;"
        "libcurl_a_debug"
    ">"
    "$<$<CONFIG:Release>:"
        "EuroScopePlugInDll;"
        "libcurl_a"
    ">"
    "gdiplus;"
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

# Post-build copy the EuroScope binary
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/EuroScopePlugInDll.dll" "${PROJECT_BINARY_DIR}/bin/EuroScopePlugInDll.dll"
    COMMENT "Copied EuroScope shared library to ${PROJECT_BINARY_DIR}/bin/EuroScopePlugInDll.dll"
)
