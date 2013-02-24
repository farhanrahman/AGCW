#ifndef IMAGE_H
#define IMAGE_H
#include "loadPNM.h"
#include <assert.h>
#include <vector>

#include "vectors.h"

typedef unsigned char uchar;

typedef unsigned short ushort;

typedef std::vector<std::pair<uint,uint> > vecpairuu;


class Image{
public:
	Image(
		unsigned int width, 
		unsigned int height, 
		unsigned int numComponents = 3,
		unsigned int exposure = 1
	) : 	width(width), 
		height(height), 
		numComponents(numComponents),
		exposure(exposure)
	{
		assert(width > 0 && height > 0 && exposure >= 1);
		this->buffer = new float [width*height*numComponents];
	}
	
	Image(const char * inputImage);
	
	Image(const char * inputImage, unsigned int exposure);

	~Image(void);

	float operator[] (unsigned int index) const;

	void writeToFile(const char* outputFile);

	void writeAsPPM(const char *outputFile);

	void writeAsPPMGamma(const char *outputFile);

	void SetAllPixels(const Vec3f &color);

	void SetPixel(unsigned int x, unsigned int y, const Vec3f &color);

	void generateCDF(void);

	vecpairuu getEnvMapSamplesFromCDF(uint numSamples);

	void highlightSamples(vecpairuu samples);

	float *buffer;
	unsigned int width;
	unsigned int height;
	unsigned int numComponents;
	unsigned int exposure;
	ushort *cdfX;
	ushort **cdfY;


private:
	void initialise(const char * inputImage);
};

#endif
