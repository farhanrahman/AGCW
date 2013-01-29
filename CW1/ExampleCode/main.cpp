/*//////////////////////////////////////////////////////////////////////////
Author: Abhijeet Ghosh
Year: 2013
//////////////////////////////////////////////////////////////////////////*/


//#define PI 3.14159265358979323

#include <iostream>
#include <string>
#include <vector>
#include "util.h"
#include "image.h"

int main(int argc, char** argv)
{  
	/*std::cerr<<"main invoked: arguments - <image_out (.pfm)> "<< std::endl;
  	std::cerr<<"main invoked: arguments - <image_in (.ppm)> <image_out (.ppm)> "<< std::endl;
  
  	int count = argc;

  	if(count < 2){
    		std::cout << "Too few arguments! .... exiting." << std::endl;
    		return 0;
  	}        

	if(count == 2) {  
	    CreateAndSavePFM(argv[1]); //Creates and saves a PFM
	} else if(count == 3){  
	//  LoadAndSavePPM(argv[1], argv[2]); //Loads and saves a PPM file
	//  LoadAndSavePFM(argv[1], argv[2]); //Loads and saves a PFM file
	//  LoadPPMAndSavePFM(argv[1], argv[2]); //Loads PPM and saves a PFM file
	    LoadPFMAndSavePPM(argv[1], argv[2]); //Loads PFM and saves a PPM file
	}  
	 
	*/
	std::string inputImageNames[] = {"../Memorial/memorial1.pfm", "../Memorial/memorial2.pfm", "../Memorial/memorial3.pfm",
				"../Memorial/memorial4.pfm", "../Memorial/memorial5.pfm", "../Memorial/memorial6.pfm",
				"../Memorial/memorial7.pfm"};

	std::string outputImageNames[] = {"memorial1.ppm", "memorial2.ppm", "memorial3.ppm",
                                 "memorial4.ppm", "memorial5.ppm", "memorial6.ppm",
                                 "memorial7.ppm"};

	unsigned int exposures[] = {1, 4, 16, 64, 256, 1028, 4096};

	std::vector<Image> images;

	for (int i = 0; i < 7; ++i){		
		images.push_back(Image(inputImageNames[i].c_str(), exposures[i]));
	}


  	return 0;
}


