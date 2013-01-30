#ifndef HDR_COMPILER_H
#define HDR_COMPILER_H
#include "image.h"
#include <vector>

class HDRCompiler{
public:
	HDRCompiler();
	Image compileHDR(std::vector<Image> &images);
	float weight(float z);
	void getInRange(float &value);
private:
	float CLAMP_MAX;
	float CLAMP_MIN;
	float avg;
	float pixelMax;
	float pixelMin;
	void clampMax(float &value);
	void clampMin(float &value);

};

#endif
