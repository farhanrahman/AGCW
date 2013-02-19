//============================================================================
// Name        : CW2Part2.cpp
//============================================================================

#include <iostream>
#include "image.h"

/*For some reason it needs the full path. Change it to
 *your own directory if you have to compile it on your
 *own machine.*/
#define LL_IMAGE "/home/fr909/git/AGCW/CW2/Part2/GraceCathedral/grace_latlong.pfm"

int main(int argc, char * argv[]) {

	Image latLongImg(LL_IMAGE);

	latLongImg.generateCDF();

	return 0;
}
