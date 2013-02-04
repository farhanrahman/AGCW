#ifndef CONV_UTIL_H
#define CONV_UTIL_H
#include "colour.h"

#define uint unsigned int

//HSL RGBtoHSL(const RGB& color);

HSL RGBtoHSL(const RGB &rgb);

RGB HSLtoRGB(const HSL& hsl);

//void HSLtoRGB_Subfunction(uint& c, const double& temp1, const double& temp2, const double& temp3);

float hue2rgb(float p, float q, float t);

#endif
