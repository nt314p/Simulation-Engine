#include <cglm\cglm.h>

// Ripped straight off of
// https://docs.oracle.com/javase/8/javafx/api/javafx/scene/paint/Color.html

#define RGB(r, g, b) (vec3) { r / 255.0f, g / 255.0f, b / 255.0f }
#define COLOR_ALICEBLUE RGB(240, 248, 255)
#define COLOR_ANTIQUEWHITE RGB(250, 235, 215)
#define COLOR_AQUA RGB(0, 255, 255)
#define COLOR_AQUAMARINE RGB(127, 255, 212)
#define COLOR_AZURE RGB(240, 255, 255)
#define COLOR_BEIGE RGB(245, 245, 220)
#define COLOR_BISQUE RGB(255, 228, 196)
#define COLOR_BLACK RGB(0, 0, 0)
#define COLOR_BLANCHEDALMOND RGB(255, 235, 205)
#define COLOR_BLUE RGB(0, 0, 255)
#define COLOR_BLUEVIOLET RGB(138, 43, 226)
#define COLOR_BROWN RGB(165, 42, 42)
#define COLOR_BURLYWOOD RGB(222, 184, 135)
#define COLOR_CADETBLUE RGB(95, 158, 160)
#define COLOR_CHARTREUSE RGB(127, 255, 0)
#define COLOR_CHOCOLATE RGB(210, 105, 30)
#define COLOR_CORAL RGB(255, 127, 80)
#define COLOR_CORNFLOWERBLUE RGB(100, 149, 237)
#define COLOR_CORNSILK RGB(255, 248, 220)
#define COLOR_CRIMSON RGB(220, 20, 60)
#define COLOR_CYAN RGB(0, 255, 255)
#define COLOR_DARKBLUE RGB(0, 0, 139)
#define COLOR_DARKCYAN RGB(0, 139, 139)
#define COLOR_DARKGOLDENROD RGB(184, 134, 11)
#define COLOR_DARKGRAY RGB(169, 169, 169)
#define COLOR_DARKGREEN RGB(0, 100, 0)
#define COLOR_DARKGREY RGB(169, 169, 169)
#define COLOR_DARKKHAKI RGB(189, 183, 107)
#define COLOR_DARKMAGENTA RGB(139, 0, 139)
#define COLOR_DARKOLIVEGREEN RGB(85, 107, 47)
#define COLOR_DARKORANGE RGB(255, 140, 0)
#define COLOR_DARKORCHID RGB(153, 50, 204)
#define COLOR_DARKRED RGB(139, 0, 0)
#define COLOR_DARKSALMON RGB(233, 150, 122)
#define COLOR_DARKSEAGREEN RGB(143, 188, 143)
#define COLOR_DARKSLATEBLUE RGB(72, 61, 139)
#define COLOR_DARKSLATEGRAY RGB(47, 79, 79)
#define COLOR_DARKSLATEGREY RGB(47, 79, 79)
#define COLOR_DARKTURQUOISE RGB(0, 206, 209)
#define COLOR_DARKVIOLET RGB(148, 0, 211)
#define COLOR_DEEPPINK RGB(255, 20, 147)
#define COLOR_DEEPSKYBLUE RGB(0, 191, 255)
#define COLOR_DIMGRAY RGB(105, 105, 105)
#define COLOR_DIMGREY RGB(105, 105, 105)
#define COLOR_DODGERBLUE RGB(30, 144, 255)
#define COLOR_FIREBRICK RGB(178, 34, 34)
#define COLOR_FLORALWHITE RGB(255, 250, 240)
#define COLOR_FORESTGREEN RGB(34, 139, 34)
#define COLOR_FUCHSIA RGB(255, 0, 255)
#define COLOR_GAINSBORO RGB(220, 220, 220)
#define COLOR_GHOSTWHITE RGB(248, 248, 255)
#define COLOR_GOLD RGB(255, 215, 0)
#define COLOR_GOLDENROD RGB(218, 165, 32)
#define COLOR_GRAY RGB(128, 128, 128)
#define COLOR_GREEN RGB(0, 128, 0)
#define COLOR_GREENYELLOW RGB(173, 255, 47)
#define COLOR_GREY RGB(128, 128, 128)
#define COLOR_HONEYDEW RGB(240, 255, 240)
#define COLOR_HOTPINK RGB(255, 105, 180)
#define COLOR_INDIANRED RGB(205, 92, 92)
#define COLOR_INDIGO RGB(75, 0, 130)
#define COLOR_IVORY RGB(255, 255, 240)
#define COLOR_KHAKI RGB(240, 230, 140)
#define COLOR_LAVENDER RGB(230, 230, 250)
#define COLOR_LAVENDERBLUSH RGB(255, 240, 245)
#define COLOR_LAWNGREEN RGB(124, 252, 0)
#define COLOR_LEMONCHIFFON RGB(255, 250, 205)
#define COLOR_LIGHTBLUE RGB(173, 216, 230)
#define COLOR_LIGHTCORAL RGB(240, 128, 128)
#define COLOR_LIGHTCYAN RGB(224, 255, 255)
#define COLOR_LIGHTGOLDENRODYELLOW RGB(250, 250, 210)
#define COLOR_LIGHTGRAY RGB(211, 211, 211)
#define COLOR_LIGHTGREEN RGB(144, 238, 144)
#define COLOR_LIGHTGREY RGB(211, 211, 211)
#define COLOR_LIGHTPINK RGB(255, 182, 193)
#define COLOR_LIGHTSALMON RGB(255, 160, 122)
#define COLOR_LIGHTSEAGREEN RGB(32, 178, 170)
#define COLOR_LIGHTSKYBLUE RGB(135, 206, 250)
#define COLOR_LIGHTSLATEGRAY RGB(119, 136, 153)
#define COLOR_LIGHTSLATEGREY RGB(119, 136, 153)
#define COLOR_LIGHTSTEELBLUE RGB(176, 196, 222)
#define COLOR_LIGHTYELLOW RGB(255, 255, 224)
#define COLOR_LIME RGB(0, 255, 0)
#define COLOR_LIMEGREEN RGB(50, 205, 50)
#define COLOR_LINEN RGB(250, 240, 230)
#define COLOR_MAGENTA RGB(255, 0, 255)
#define COLOR_MAROON RGB(128, 0, 0)
#define COLOR_MEDIUMAQUAMARINE RGB(102, 205, 170)
#define COLOR_MEDIUMBLUE RGB(0, 0, 205)
#define COLOR_MEDIUMORCHID RGB(186, 85, 211)
#define COLOR_MEDIUMPURPLE RGB(147, 112, 219)
#define COLOR_MEDIUMSEAGREEN RGB(60, 179, 113)
#define COLOR_MEDIUMSLATEBLUE RGB(123, 104, 238)
#define COLOR_MEDIUMSPRINGGREEN RGB(0, 250, 154)
#define COLOR_MEDIUMTURQUOISE RGB(72, 209, 204)
#define COLOR_MEDIUMVIOLETRED RGB(199, 21, 133)
#define COLOR_MIDNIGHTBLUE RGB(25, 25, 112)
#define COLOR_MINTCREAM RGB(245, 255, 250)
#define COLOR_MISTYROSE RGB(255, 228, 225)
#define COLOR_MOCCASIN RGB(255, 228, 181)
#define COLOR_NAVAJOWHITE RGB(255, 222, 173)
#define COLOR_NAVY RGB(0, 0, 128)
#define COLOR_OLDLACE RGB(253, 245, 230)
#define COLOR_OLIVE RGB(128, 128, 0)
#define COLOR_OLIVEDRAB RGB(107, 142, 35)
#define COLOR_ORANGE RGB(255, 165, 0)
#define COLOR_ORANGERED RGB(255, 69, 0)
#define COLOR_ORCHID RGB(218, 112, 214)
#define COLOR_PALEGOLDENROD RGB(238, 232, 170)
#define COLOR_PALEGREEN RGB(152, 251, 152)
#define COLOR_PALETURQUOISE RGB(175, 238, 238)
#define COLOR_PALEVIOLETRED RGB(219, 112, 147)
#define COLOR_PAPAYAWHIP RGB(255, 239, 213)
#define COLOR_PEACHPUFF RGB(255, 218, 185)
#define COLOR_PERU RGB(205, 133, 63)
#define COLOR_PINK RGB(255, 192, 203)
#define COLOR_PLUM RGB(221, 160, 221)
#define COLOR_POWDERBLUE RGB(176, 224, 230)
#define COLOR_PURPLE RGB(128, 0, 128)
#define COLOR_RED RGB(255, 0, 0)
#define COLOR_ROSYBROWN RGB(188, 143, 143)
#define COLOR_ROYALBLUE RGB(65, 105, 225)
#define COLOR_SADDLEBROWN RGB(139, 69, 19)
#define COLOR_SALMON RGB(250, 128, 114)
#define COLOR_SANDYBROWN RGB(244, 164, 96)
#define COLOR_SEAGREEN RGB(46, 139, 87)
#define COLOR_SEASHELL RGB(255, 245, 238)
#define COLOR_SIENNA RGB(160, 82, 45)
#define COLOR_SILVER RGB(192, 192, 192)
#define COLOR_SKYBLUE RGB(135, 206, 235)
#define COLOR_SLATEBLUE RGB(106, 90, 205)
#define COLOR_SLATEGRAY RGB(112, 128, 144)
#define COLOR_SLATEGREY RGB(112, 128, 144)
#define COLOR_SNOW RGB(255, 250, 250)
#define COLOR_SPRINGGREEN RGB(0, 255, 127)
#define COLOR_STEELBLUE RGB(70, 130, 180)
#define COLOR_TAN RGB(210, 180, 140)
#define COLOR_TEAL RGB(0, 128, 128)
#define COLOR_THISTLE RGB(216, 191, 216)
#define COLOR_TOMATO RGB(255, 99, 71)
#define COLOR_TURQUOISE RGB(64, 224, 208)
#define COLOR_VIOLET RGB(238, 130, 238)
#define COLOR_WHEAT RGB(245, 222, 179)
#define COLOR_WHITE RGB(255, 255, 255)
#define COLOR_WHITESMOKE RGB(245, 245, 245)
#define COLOR_YELLOW RGB(255, 255, 0)
#define COLOR_YELLOWGREEN RGB(154, 205, 50)