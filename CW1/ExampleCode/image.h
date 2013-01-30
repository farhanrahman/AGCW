#ifndef IMAGE_H
#define IMAGE_H
#include "loadPNM.h"
#include <assert.h>

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
		this->gsBuffer = new float [width*height];
	}
	
	Image(const char * inputImage);
	
	Image(const char * inputImage, unsigned int exposure);

	~Image(void);

	float operator[] (unsigned int index);

	void writeToFile(const char* outputFile);

	void writeAsPPM(const char *outputFile);

	float *buffer;
	float* gsBuffer;
	unsigned int width;
	unsigned int height;
	unsigned int numComponents;
	unsigned int exposure;

private:
	void initialise(const char * inputImage);
};

#endif
