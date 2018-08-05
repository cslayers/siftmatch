#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<nonfree/features2d.hpp>
#include<nonfree/nonfree.hpp>
using namespace cv;

#include<iostream>
#include<string>
#include<fstream>
using namespace std;


#define AY_VERBOSE 1

void get_sift(string filename, vector<KeyPoint>&kps, Mat&desc,
	int numfeatures=0,
	float contour=0.04,
	float edgeLimit=10.f,
	float sigma=1.6f);