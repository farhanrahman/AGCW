#include "util.h"

unsigned int width;

unsigned int height;

unsigned int numComponents;

void CreateAndSavePFM(const char *image_out)
{
	width = 511; // set size of image to 511x511 pixels
	height = 511;
	numComponents = 3;

	float *img_out = new float [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
	{
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image
				//set image to white
				img_out[index] = 1.0f; //R
				img_out[index + 1] = 1.0f;//G
				img_out[index + 2] = 1.0f;//B
			}
		}
	}

	WritePFM(image_out, width, height, numComponents, img_out);
}



void LoadAndSavePPM(const char *image_in, const char *image_out)
{
	unsigned char *img_in = loadPNM(image_in, width, height, numComponents);
	unsigned char *img_out = new unsigned char [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    	{
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image

				img_out[index] = img_in[index]; //R
				img_out[index + 1] = img_in[index + 1];//G
				img_out[index + 2] = img_in[index + 2];//B
			}
		}
	}

	WritePNM(image_out, width, height, numComponents, img_out);
}



void LoadAndSavePFM(const char *image_in, const char *image_out)
{
	float *img_in = loadPFM(image_in, width, height, numComponents);
	float *img_out = new float [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    	{
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image

				img_out[index] = img_in[index]; //R
				img_out[index + 1] = img_in[index + 1];//G
				img_out[index + 2] = img_in[index + 2];//B
			}
		}
	}

	WritePFM(image_out, width, height, numComponents, img_out);
}



void LoadPPMAndSavePFM(const char *image_in, const char *image_out)
{
	unsigned char *img_in = loadPNM(image_in, width, height, numComponents);
	float *img_out = new float [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    	{
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image
				//typecast 0 - 255 values to the 0.0f -> 1.0f range 

				img_out[index] = static_cast<float>(img_in[index])/255.0f; //R
				img_out[index + 1] = static_cast<float>(img_in[index + 1])/255.0f;//G
				img_out[index + 2] = static_cast<float>(img_in[index + 2])/255.0f;//B
			}
		}
	}

	WritePFM(image_out, width, height, numComponents, img_out);
}



void LoadPFMAndSavePPM(const char *image_in, const char *image_out)
{
	float *img_in = loadPFM(image_in, width, height, numComponents);
	unsigned char *img_out = new unsigned char [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    	{
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image

				//typecast 0.0f -> 1.0f values to the 0 - 255 range 

				img_out[index] = static_cast<unsigned char>(img_in[index]*255.0f); //R
				img_out[index + 1] = static_cast<unsigned char>(img_in[index + 1]*255.0f);//G
				img_out[index + 2] = static_cast<unsigned char>(img_in[index + 2]*255.0f);//B
			}
		}
	}

	WritePNM(image_out, width, height, numComponents, img_out);
}
