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

	LatLong(const LatLong& latLong) {
		this->data[0] = latLong.zenith();
		this->data[1] = latLong.azimuth();		
	}

	LatLong(const Vec3f& vec){
		LatLong ll = LatLong::v3f2ll(vec);
		this->data[0] = ll.zenith();
		this->data[1] = ll.azimuth();
	}
	
	~LatLong(){}

	/*Getter functions*/
	float zenith(void) const { return data[0]; }
	float theta(void) const {return data[0]; }
	
	float azimuth(void) const { return data[1]; }
	float phi(void) const {return data[1]; }

	/*Setter methods*/
	void setZenith(float z){ this->data[0] = z; }
	void setAzimuth(float a){ this->data[1] = a; }
	void setLatLong(float z, float a) {
		this->setZenith(z);
		this->setAzimuth(a);
	}

	/*Utility methods*/

	static Vec3f ll2v3f(const LatLong& latLong){
		Vec3f ret;
		float x,y,z;
		float theta = latLong.theta();
		float phi = latLong.phi();
		x = sin(theta)*sin(phi);
		y = cos(theta);
		z = sin(theta)*cos(phi);
		ret.Set(x,y,z);
		return ret;
	}

	static LatLong v3f2ll(const Vec3f& vec){
		LatLong l;
		l.setZenith(acos(vec.y()));
		if (vec.z() != 0.0){
			l.setAzimuth(atan(vec.x() / vec.z()));
		} else {
			l.setAzimuth(0.0);
		}
	}

private:
	/*Zenith or theta (0 -> PI): data[0]*/
	/*Azimuth or phi: (0 -> 2PI): data[1]*/
	float data[2];
};

#endif