#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "loadPNM.h"

#define uint unsigned int

void CreateAndSavePFM(const char *image_out);

void LoadAndSavePPM(const char *image_in, const char *image_out);

void LoadAndSavePFM(const char *image_in, const char *image_out);

void LoadPPMAndSavePFM(const char *image_in, const char *image_out);

void LoadPFMAndSavePPM(const char *image_in, const char *image_out);


#endif
