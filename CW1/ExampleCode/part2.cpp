#include "part2.h"

#define LL_IMAGE "../GraceCathedral/grace_latlong.pfm"

char *_inputName       	= NULL;
char *_outputName   	= NULL;

unsigned int _width      = 511;
unsigned int _height     = 511;

void renderScene(EnvironmentMap& em, SceneParser &, Image &);

int main (int argc, char * argv[]){
    // sample command lines:
    // raycast -input input.txt -size 100 100 -output output.ppm
    // raycast -input input.txt -size 100 100 -depth 5.5 8.8 output.ppm
	if (argc == 1) {
		//default values
		_inputName = "scene1.txt";
		_outputName = "part2.ppm";
	} else {    
		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i], "-input") == 0) {
			    i++;assert (i < argc);
			    _inputName = argv[i];;
			} else if (strcmp(argv[i], "-size") == 0) {
			    i++;assert (i < argc);
			    _width = atoi(argv[i]);
			    i++;assert (i < argc);
			    _height = atoi(argv[i]);
			} else if (strcmp(argv[i], "-output") == 0) {
			    i++;assert (i < argc);
			    _outputName = argv[i];
			} else {
			    printf(
				   "Error with command line argument %d: '%s'\n",
				   i, argv[i]);
			    assert(0);
			}
		}
	}

	SceneParser scene(_inputName);

	Image image(_width, _height);
	
	Image llImage(LL_IMAGE);

	EnvironmentMap em(llImage);



	renderScene(em, scene, image);

	image.writeAsPPM(_outputName);

	return 0;
}

void renderScene(EnvironmentMap& em, SceneParser &scene, Image &image){
    /*First set the background color to the specified background color.
     *Then iterate through the viewport co-ordinates to generate ray
     *for every co-ordinate (i,j)*/
    
    image.SetAllPixels(scene.getBackgroundColor());
    for(int i = 0; i < _width; i++)
        for(int j = 0; j < _height; j++){
            /*Initialise the HIT object h to have a distance of INFINITY
             *and color set to specified background color*/
            
            Hit h(INFINITY, scene.getBackgroundColor());
            
            /*Generate ray from Camera for viewport co-ordinate (i,j)*/
            Ray r = (*scene.getCamera()).generateRay(Vec2f(((float)i)/((float)_width),((float)j)/((float)_height)));
            if((*scene.getGroup()).intersect(em, r,h)){
                /*If there is an intersection, set the pixel on the image
                 *to be the color of the nearest object that intersects*/
                image.SetPixel(i, j, h.getColor());
            }
        }
}

