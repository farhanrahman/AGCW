#include "HDRCompiler.h"
#include <limits>
#include <math.h>

#define uint unsigned int

HDRCompiler::HDRCompiler(){
	CLAMP_MAX = 0.92;
	CLAMP_MIN = 0.005;
}

Image HDRCompiler::compileHDR(std::vector<Image> &images){

	uint numImages = images.size();
	uint height = images[0].height;
	uint width = images[0].width;
	uint numChannels = images[0].numComponents;

	Image result(width,height,numChannels);

	for (uint i = 0; i < height; i++) {
		for (uint j = 0; j < width; j++) {
			uint pixel = i * width + j;
			uint index = pixel * numChannels;
			float weightSum[] = {0,0,0};
			float numerator[] = {0,0,0};
			for (uint k = 0; k < numImages; k++) {

				Image img = images[k];

				float inPix[] = {
					img.buffer[index],
					img.buffer[index+1],
					img.buffer[index+2],
				};
				bool OOR = false;
				for (uint c = 0; c < numChannels; c++) {
					if (isOutOfRange(inPix[c])) {
						OOR = true;
						break;
					}
				}
				if (OOR) {
					continue;
				}

				uint exposure = img.exposure;

				for(uint c = 0; c < numChannels; c++){
					float channel = inPix[c];
					float pixelWeight = weight(channel);
					weightSum[c] += pixelWeight;
					numerator[c] += pixelWeight*(log((1/(float) exposure) * channel));
				}
			}
			for (uint c = 0; c < numChannels; c++) {
				float r = exp((float) (numerator[c] / weightSum[c]));
				result.buffer[index + c] = r;
			}
		}
	}

	result.writeAsPPM("out.ppm");
}

bool HDRCompiler::isOutOfRange(float value) {
	if(value > CLAMP_MAX){
		return true;
	} else if (value < CLAMP_MIN){
		return true;
	}
	return false;
}

/*Weight function*/
float HDRCompiler::weight(float z){
//	return 0.05;
	return 2.0f * (0.5f - fabsf(0.5f - z));
}
