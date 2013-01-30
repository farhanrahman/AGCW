#include "HDRCompiler.h"
#include <limits>
#include <math.h>

#define uint unsigned int

HDRCompiler::HDRCompiler(){
	CLAMP_MAX = 0.92;
	CLAMP_MIN = 0.005;
}

Image HDRCompiler::compileHDR(std::vector<Image> &images){
	this->pixelMax = 0.0;
	this->pixelMin = std::numeric_limits<float>::max();
	std::vector<Image>::iterator it;
	uint w = 0;
	uint h = 0;
	/*Acquire the pixel minimum and maximum values*/
	for (it = images.begin(); it != images.end(); ++it){
		float *image = (*it).buffer;
		uint width = (*it).width;
		uint height = (*it).height;
		uint channels = (*it).numComponents;
		w = std::max(width, w);
		h = std::max(height, h);
		for(uint i = 0; i < height; ++i)
			for(uint j = 0; j < width; ++j)
				for(uint k = 0; k < channels; ++k){
					uint index = i*channels*width + j*channels + k;
					pixelMax = std::max(pixelMax, image[index]);
					pixelMin = std::min(pixelMin, image[index]);
		}
	}

	this->clampMax(pixelMax);
	this->clampMin(pixelMin);
	this->avg = 0.5*(pixelMax+pixelMin);

	Image result(w, h);
	uint numComponents = result.numComponents;
	for(int i = 0; i < h; ++i)
		for(int j = 0; j < w; ++j)
			for(int k = 0; k < numComponents; ++k){
				uint index = i*numComponents*w + j*numComponents + k;
				float weightSum = 0.0;
				float numerator = 0.0;
				for(it = images.begin(); it != images.end(); ++it){
					uint exposure = (*it).exposure;
					float pixel = (*it).buffer[index];
					float pixelWeight = weight(pixel);
					weightSum += pixelWeight;
					numerator += pixelWeight*(pixel * log(1/(float) exposure));
				}
				
				result.buffer[index] = exp((float) (numerator / weightSum));
				this->getInRange(result.buffer[index]);
	}

	result.writeAsPPM("out.ppm");	

}

void HDRCompiler::clampMax(float &value){
	value = std::min(CLAMP_MAX, value);
}

void HDRCompiler::clampMin(float &value){
	value = std::max(CLAMP_MIN, value);
}

void HDRCompiler::getInRange(float &value){
	if(value > CLAMP_MAX){
		value = CLAMP_MAX;
	} else if (value < CLAMP_MIN){
		value = CLAMP_MIN;
	}
}

/*Weight function*/
float HDRCompiler::weight(float z){
	this->getInRange(z);
	if (z <= avg){
		return (z - pixelMin);
	} else {
		return (pixelMax - z);
	}
}
