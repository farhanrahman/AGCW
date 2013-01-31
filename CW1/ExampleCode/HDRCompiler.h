#ifndef HDR_COMPILER_H
#define HDR_COMPILER_H
#include "image.h"
#include <vector>

class HDRCompiler{
public:
	HDRCompiler();
	Image compileHDR(std::vector<Image> &images);
	float weight(float z);
	float weightLowestExposure(float z);
	float weightHighestExposure(float z);
	bool isOutOfRange(float value);
private:
	float CLAMP_MAX;
	float CLAMP_MIN;
};

#endif
