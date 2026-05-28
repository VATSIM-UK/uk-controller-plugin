#include "Theme.h"

// When updating - please update the commented hex code as well ;)

namespace UKControllerPlugin::Graphics {

    // DEFAULT — same as current ColourPaletteDefinitions Default*
    const Theme DEFAULT_THEME = {
        Gdiplus::Color(58, 57, 58),    // background
        Gdiplus::Color(215, 215, 215), // border
        Gdiplus::Color(130, 50, 154),  // header
        Gdiplus::Color(255, 153, 255), // highlightedHeader
        Gdiplus::Color(227, 227, 227), // text
        Gdiplus::Color(255, 255, 0),   // highlightedText
        Gdiplus::Color(7, 237, 7),     // mainAircraftText
        Gdiplus::Color(246, 181, 4),   // highlightedAircraftText
        Gdiplus::Color(0, 255, 0),     // timerGreen
        Gdiplus::Color(255, 255, 0),   // timerYellow
        Gdiplus::Color(255, 0, 0),     // timerRed
    };

    const Theme NODE_THEME = {
        Gdiplus::Color(0, 0, 0),         // background              #000000
        Gdiplus::Color(255, 255, 255),   // border                  #ffffff
        Gdiplus::Color(160, 111, 112),   // header                  #a06f70
        Gdiplus::Color(255, 100, 50),    // highlightedHeader       #ff6432
        Gdiplus::Color(255, 255, 255),   // text                    #ffffff
        Gdiplus::Color(232, 163, 0),     // highlightedText         #e8a300
        Gdiplus::Color(104, 221, 69),    // mainAircraftText        #68dd45
        Gdiplus::Color(232, 163, 0),     // highlightedAircraftText #e8a300
        Gdiplus::Color(104, 221, 69),    // timerGreen              #68dd45
        Gdiplus::Color(232, 163, 0),     // timerYellow             #e8a300
        Gdiplus::Color(255, 100, 50),    // timerRed                #ff6432
    };

    const Theme NERC_THEME = {
        Gdiplus::Color(150, 150, 150), // background                #969696
        Gdiplus::Color(83, 83, 83),    // border                    #535353
        Gdiplus::Color(135, 135, 135), // header                    #878787
        Gdiplus::Color(255, 255, 255), // highlightedHeader         #ffffff
        Gdiplus::Color(0, 0, 0),       // text                      #000000
        Gdiplus::Color(255, 255, 255), // highlightedText           #ffffff
        Gdiplus::Color(0, 0, 0),       // mainAircraftText          #000000
        Gdiplus::Color(255, 255, 255), // highlightedAircraftText   #ffffff
        Gdiplus::Color(180, 255, 125), // timerGreen                #B4FF7D
        Gdiplus::Color(255, 216, 0),   // timerYellow               #FFD800
        Gdiplus::Color(192, 0, 0),     // timerRed                  #C00000
    };

    const Theme NOVA_THEME = {
        Gdiplus::Color(180, 180, 180), // background                #B4B4B4
        Gdiplus::Color(83, 83, 83),    // border                    #535353
        Gdiplus::Color(0, 140, 200),   // header                    #008CC8
        Gdiplus::Color(255, 255, 255), // highlightedHeader         #ffffff
        Gdiplus::Color(0, 0, 0),       // text                      #000000
        Gdiplus::Color(255, 255, 255), // highlightedText           #ffffff
        Gdiplus::Color(0, 0, 0),       // mainAircraftText          #000000
        Gdiplus::Color(255, 255, 255), // highlightedAircraftText   #ffffff
        Gdiplus::Color(255, 255, 255), // timerGreen                #ffffff
        Gdiplus::Color(255, 216, 0),   // timerYellow               #FFD800
        Gdiplus::Color(192, 0, 0),     // timerRed                  #C00000
    };

    const Theme ITEC_THEME = {
        Gdiplus::Color(195, 195, 185), // background                #C3C3B9
        Gdiplus::Color(0, 0, 0),       // border                    #000000
        Gdiplus::Color(214, 216, 150), // header                    #D6D896
        Gdiplus::Color(255, 255, 255), // highlightedHeader         #ffffff
        Gdiplus::Color(0, 0, 0),       // text                      #000000
        Gdiplus::Color(0, 0, 255),     // highlightedText           #0000FF
        Gdiplus::Color(0, 0, 0),       // mainAircraftText          #000000
        Gdiplus::Color(0, 0, 255),     // highlightedAircraftText   #0000FF
        Gdiplus::Color(0, 0, 255),     // timerGreen                #0000FF
        Gdiplus::Color(130, 111, 0),   // timerYellow               #826F00
        Gdiplus::Color(192, 0, 0),     // timerRed                  #C00000
    };

} // namespace UKControllerPlugin::Graphics
