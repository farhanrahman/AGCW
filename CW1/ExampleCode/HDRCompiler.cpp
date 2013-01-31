#include "HDRCompiler.h"
#include <limits>
#include <math.h>
#include <string>
#include <sstream>


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
				float average = (0.2126 * inPix[0] + 0.7152 * inPix[1] + 0.0722 * inPix[2]);
				float pixelWeight;
				if (k == numImages - 1) {
					pixelWeight = weightHighestExposure(average);
				}
				else if (k == 0) {
					pixelWeight = weightLowestExposure(average);
				}
				else {
					pixelWeight = weight(average);
				}

				uint exposure = img.exposure;

				for(uint c = 0; c < numChannels; c++){
					float channel = inPix[c];
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

	for (int i = 0; i < 13; i++) {

		result.exposure = pow(2,i);
		std::stringstream ss;
		std::string out = "out";
		std::string str;
		std::string ppm = ".ppm";
		ss << out << i << ppm;
		ss >> str;
		result.writeAsPPM(str.c_str());
	}
	result.writeToFile("out.pfm");

//	result.writeAsPPM("out.ppm");
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

	if (z > CLAMP_MAX) return 0.0001f;
	if (z < CLAMP_MIN) return 0.0001f;

	return 2.0f * (0.5f - fabsf(0.5f - z));
}

/*Weight function*/
float HDRCompiler::weightLowestExposure(float z){

	if (z < CLAMP_MIN) return 0.0001f;
	return 2.0f * (0.5f - fabsf(0.5f - z));
}

/*Weight function*/
float HDRCompiler::weightHighestExposure(float z){
	if (z > CLAMP_MAX) return 0.0001f;
	return 2.0f * (0.5f - fabsf(0.5f - z));
}
