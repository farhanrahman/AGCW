#include "convUtil.h"
#include <iostream>

// This function extracts the hue, saturation, and luminance from "color" 
// and places these values in h, s, and l respectively.
/*HSL RGBtoHSL(const RGB& color)
{
	uint r = (uint)color.r;
	uint g = (uint)color.g;
	uint b = (uint)color.b;
	uint h = 0;
	uint s = 0;
	uint l = 0;

	double r_percent = ((double)r)/255.0f;
	double g_percent = ((double)g)/255.0f;
	double b_percent = ((double)b)/255.0f;

	double max_color = 0;
	if((r_percent >= g_percent) && (r_percent >= b_percent))
		max_color = r_percent;
	if((g_percent >= r_percent) && (g_percent >= b_percent))
		max_color = g_percent;
	if((b_percent >= r_percent) && (b_percent >= g_percent))
		max_color = b_percent;

	double min_color = 0;
	if((r_percent <= g_percent) && (r_percent <= b_percent))
		min_color = r_percent;
	if((g_percent <= r_percent) && (g_percent <= b_percent))
		min_color = g_percent;
	if((b_percent <= r_percent) && (b_percent <= g_percent))
		min_color = b_percent;

	double L = 0;
	double S = 0;
	double H = 0;

	L = (max_color + min_color)/2;

	if(max_color == min_color)
	{
		S = 0;
		H = 0;
	}
	else
	{
		if(L < .50)
		{
			S = (max_color - min_color)/(max_color + min_color);
		}
		else
		{
			S = (max_color - min_color)/(2 - max_color - min_color);
		}
		if(max_color == r_percent)
		{
			H = (g_percent - b_percent)/(max_color - min_color);
		}
		if(max_color == g_percent)
		{
			H = 2 + (b_percent - r_percent)/(max_color - min_color);
		}
		if(max_color == b_percent)
		{
			H = 4 + (r_percent - g_percent)/(max_color - min_color);
		}
	}
	s = (uint)(S*100);
	l = (uint)(L*100);
	H = H*60;
	if(H < 0)
		H += 360;
	h = (uint)H;
	
	HSL ret;
	ret.h = h;
	ret.s = s;
	ret.l = l;
	return ret;
}

// This function converts the "color" object to the equivalent RGB values of
// the hue, saturation, and luminance passed as h, s, and l respectively
RGB HSLtoRGB(const HSL& hsl)
{
	uint r = 0;
	uint g = 0;
	uint b = 0;

	double L = ((double)hsl.l)/100;
	double S = ((double)hsl.s)/100;
	double H = ((double)hsl.h)/360;

	if(hsl.s == 0)
	{
		r = hsl.l;
		g = hsl.l;
		b = hsl.l;
	}
	else
	{
		double temp1 = 0;
		if(L < .50)
		{
			temp1 = L*(1 + S);
		}
		else
		{
			temp1 = L + S - (L*S);
		}

		double temp2 = 2*L - temp1;

		double temp3 = 0;
		for(int i = 0 ; i < 3 ; i++)
		{
			switch(i)
			{
			case 0: // red
				{
					temp3 = H + .33333;
					if(temp3 > 1)
						temp3 -= 1;
					HSLtoRGB_Subfunction(r,temp1,temp2,temp3);
					break;
				}
			case 1: // green
				{
					temp3 = H;
					HSLtoRGB_Subfunction(g,temp1,temp2,temp3);
					break;
				}
			case 2: // blue
				{
					temp3 = H - .33333;
					if(temp3 < 0)
						temp3 += 1;
					HSLtoRGB_Subfunction(b,temp1,temp2,temp3);
					break;
				}
			default:
				{

				}
			}
		}
	}
	r = (uint)((((double)r)/100)*255);
	g = (uint)((((double)g)/100)*255);
	b = (uint)((((double)b)/100)*255);
	RGB color;
	color.r = r;
	color.g = g;
	color.b = b;
	return color;
}

// This is a subfunction of HSLtoRGB
void HSLtoRGB_Subfunction(uint& c, const double& temp1, const double& temp2, const double& temp3)
{
	if((temp3 * 6) < 1)
		c = (uint)((temp2 + (temp1 - temp2)*6*temp3)*100);
	else
	if((temp3 * 2) < 1)
		c = (uint)(temp1*100);
	else
	if((temp3 * 3) < 2)
		c = (uint)((temp2 + (temp1 - temp2)*(.66666 - temp3)*6)*100);
	else
		c = (uint)(temp2*100);
	return;
}*/




/**
 * Converts an RGB color value to HSL. Conversion formula
 * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
 * Assumes r, g, and b are contained in the set [0, 255] and
 * returns h, s, and l in the set [0, 1].
 *
 * @param   Number  r       The red color value
 * @param   Number  g       The green color value
 * @param   Number  b       The blue color value
 * @return  Array           The HSL representation
 */
HSL RGBtoHSL(const RGB &rgb){
	float r = (float) rgb.r;
	float g = (float) rgb.g;
	float b = (float) rgb.b;
   
	r /= 255.0f, g /= 255.0f, b /= 255.0f;
    	float max = std::max(r, std::max(g, b));
	float min = std::min(r, std::min(g, b));
    	float h, s, l;
	h = (max + min) / 2;
	s = h;
	l = h;

	if(max == min){
		h = s = 0; // achromatic
	}else{
		float d = max - min;
		s = l > 0.5 ? d / (2.0f - max - min) : d / (max + min);
		if (max == r) {
			h = (g - b) / d + (g < b ? 6.0f : 0.0f);
		}else if(max == g){
			 h = (b - r) / d + 2.0f;
		}else if(max == b) {
			h = (r - g) / d + 4.0f;
		}
		h /= 6.0f;
	}

	HSL hsl;
	hsl.h = h;
	hsl.s = s;
	hsl.l = l;
    return hsl;
}

/**
 * Converts an HSL color value to RGB. Conversion formula
 * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
 * Assumes h, s, and l are contained in the set [0, 1] and
 * returns r, g, and b in the set [0, 255].
 *
 * @param   Number  h       The hue
 * @param   Number  s       The saturation
 * @param   Number  l       The lightness
 * @return  Array           The RGB representation
 */
RGB HSLtoRGB(const HSL& hsl){
	unsigned int r, g, b;
	float h, s, l;
	h = hsl.h;
	s = hsl.s;
	l = hsl.l;
	if(s == 0){
		r = g = b = l; // achromatic
	}else{
		float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
		float p = 2 * l - q;
		r = static_cast<unsigned int>(hue2rgb(p, q, h + 1/3));
		g = static_cast<unsigned int>(hue2rgb(p, q, h));
		b = static_cast<unsigned int>(hue2rgb(p, q, h - 1/3));
	}

	RGB color;
	color.r = r*255;
	color.g = g*255;
	color.b = b*255;

	return color;
}


float hue2rgb(float p, float q, float t){
	if(t < 0) t += 1;
	if(t > 1) t -= 1;
	if(t < 1/6) return p + (q - p) * 6 * t;
	if(t < 1/2) return q;
	if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;
	return p;
}

