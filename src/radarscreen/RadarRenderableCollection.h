#pragma once

// Forward declaration
namespace UKControllerPlugin {
    namespace RadarScreen {
        class RadarRenderableInterface;
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Windows {
        class GdiGraphicsInterface;
    }  // namespace Windows
    namespace Euroscope {
        class EuroscopeRadarLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace RadarScreen {

        /*
            A collection of objects that render things to the screen.
        */
        class RadarRenderableCollection
        {
            public:
                RadarRenderableCollection(void);
                size_t CountRenderers(void) const;
                size_t CountRenderersInPhase(int phase) const;
                size_t CountScreenObjects(void) const;
                int GetRendererIdForScreenObject(int objectId) const;
                void LeftClickScreenObject(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) const;
                void MoveScreenObject(int objectId, std::string objectDescription, RECT position) const;
                void RegisterRenderer(
                    int rendererId,
                    std::shared_ptr<UKControllerPlugin::RadarScreen::RadarRenderableInterface> renderer,
                    int renderPhase
                );
                int ReserveRendererIdentifier(void);
                int ReserveScreenObjectIdentifier(int rendererId);
                void Render(
                    int phase,
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) const;
                void RightClickScreenObject(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) const;
                void ResetPosition(void) const;

                // Constant dictating that rendering should take place in the initial bitmap
                const int initialPhase = 0;

                // Constant dictating that rendering should take place before tags
                const int beforeTags = 1;

                // Constant to dictate that rendering should take place after tags
                const int afterTags = 2;

                // Constant to dicatate that rendering should take place after lists.
                const int afterLists = 3;

            private:

                void RenderGroup(
                    const std::vector<int> & group,
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) const;

                // All the renderers to render during the initial phase
                std::vector<int> initialPhaseRenders;

                // All the renderers to render before tags
                std::vector<int> beforeTagRenders;

                // All the renderers to render after tags
                std::vector<int> afterTagRenders;

                // All the renderers to render after lists
                std::vector<int> afterListRenders;

                // Map of all the renderers
                std::map<int, std::shared_ptr<RadarRenderableInterface>> allRenderers;

                // Maps a given screen object ID to a given renderer.
                std::map<int, int> screenObjectMap;

                // The next renderer ID available for use - used to link screen objects to classes
                int nextRendererId;

                // The next available screen object ID
                int nextScreenObjectId;

                // The currently active radar screen
                int activeScreen;
        };
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
