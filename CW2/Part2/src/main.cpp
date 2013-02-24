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

	uint numSamps[] = {64,256,1024};

	for (uint i = 0; i < sizeof(numSamps)/sizeof(uint); i++) {
		vecpairuu samples = latLongImg.getEnvMapSamplesFromCDF(numSamps[i]);

		Image envsamp = Image(LL_IMAGE);
		envsamp.exposure = 10;
		envsamp.highlightSamples(samples);

		stringstream ppm;
		ppm << "env" << numSamps[i] << ".ppm";
		envsamp.writeAsPPM(ppm.str().c_str());

		stringstream pfm;
		pfm << "env" << numSamps[i] << ".pfm";
		envsamp.writeToFile(pfm.str().c_str());
	}


	return 0;
}
