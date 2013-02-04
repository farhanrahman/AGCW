#ifndef IMAGE_H
#define IMAGE_H
#include "loadPNM.h"
#include <assert.h>

#include "vectors.h"

#define uchar unsigned char

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

	void writeAsHDRPPM(const char* outputFile);
	
	void writeAsHDRPPM2(const char *outputFile);

	void histogramEqualization(unsigned char **imgPP, unsigned int px, unsigned int py, unsigned int windowsize, unsigned int iw, unsigned int ih);

	void SetAllPixels(const Vec3f &color);

	void SetPixel(unsigned int x, unsigned int y, const Vec3f &color);

	float *buffer;
	unsigned int width;
	unsigned int height;
	unsigned int numComponents;
	unsigned int exposure;

private:
	void initialise(const char * inputImage);
};

#endif
