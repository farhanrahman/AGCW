//============================================================================
// Name        : CW2Part2.cpp
//============================================================================

#include <iostream>
#include "image.h"
#include <sstream>

/*For some reason it needs the full path. Change it to
 *your own directory if you have to compile it on your
 *own machine.*/
#ifdef __CYGWIN__
#define LL_IMAGE "../media/grace_latlong.pfm"
#else
#define LL_IMAGE "/home/fr909/git/AGCW/CW2/Part2/GraceCathedral/grace_latlong.pfm"
#endif

int main(int argc, char * argv[]) {

	Image latLongImg(LL_IMAGE);

	latLongImg.generateCDF();

	float exp[] = {1.0, 10.0, 50.0, 200.0};

	for (uint i = 0; i < sizeof(exp)/sizeof(uint); ++i){
		Image sampleImg(LL_IMAGE);
		sampleImg.exposure = 10;
		sampleImg.generateSamplesFromPhongBRDF(exp[i], 256);

		stringstream ppm;
		ppm << "env" << exp[i] << ".ppm";
		sampleImg.writeAsPPM(ppm.str().c_str());

		stringstream pfm;
		pfm << "env" << exp[i] << ".pfm";
		sampleImg.writeToFile(pfm.str().c_str());
	}

	return 0;
}
