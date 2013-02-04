#ifndef LATLONG_H
#define LATLONG_H


#include <math.h>
#include "vectors.h"

class LatLong{
public:
	LatLong(float zenith = 0.0, float azimuth = 0.0){
		data[0] = zenith;
		data[1] = azimuth;
	}

	~LatLong(){}

	

private:
	float data[2];

};

#endif
