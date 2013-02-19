/*
 * main.cpp
 *
 *  Created on: 19 Feb 2013
 *      Author: fr909
 */

#include <iostream>
#include <fstream>
#include <math.h>
#define ETA_I 1.0
#define ETA_T 1.5
#define PI 3.1415926535897932384626433832795L

double fresnelPerpendicular(double etaI, double etaT, double thetaI, double thetaT){
	return pow(fabs((etaI*cos(thetaI) - etaT*cos(thetaT)) / (etaI*cos(thetaI) + etaT*cos(thetaT))) , 2.0);
}

double fresnelParallel(double etaI, double etaT, double thetaI, double thetaT){
	return pow(fabs((etaT*cos(thetaI) - etaI*cos(thetaT)) / (etaT*cos(thetaI) + etaI*cos(thetaT))) , 2.0);
}


int main(int argc, char * argv[]){

	std::ofstream enter, exit;
	enter.open("enter.txt");
	exit.open("exit.txt");

	for (unsigned i = 0; i <= 90; ++i){
		/*Light entering medium*/
		double thetaI = (double) i * PI /180.0;
		double thetaT = asin(ETA_I*sin(thetaI)/ETA_T);
		double rParallel = fresnelParallel(ETA_I, ETA_T, thetaI, thetaT);
		double rPerpendicular = fresnelPerpendicular(ETA_I, ETA_T, thetaI, thetaT);
		enter << i << " " << rParallel << " " << rPerpendicular << std::endl;

		/*Light exiting medium*/
		double thetaIExit = thetaT;
		double thetaTExit = asin(ETA_T*sin(thetaIExit)/ETA_I);
		double rParallelExit = fresnelPerpendicular(ETA_T, ETA_I, thetaIExit, thetaTExit);
		double rPerpendicularExit = fresnelParallel(ETA_T, ETA_I, thetaIExit, thetaTExit);

		exit << i << " " << rParallelExit << " " << rPerpendicularExit << std::endl;
	}

	enter.close();
	exit.close();

	return 0;
}
