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


#define DESC_LENGTH (128)
#define MATCH_THRE_SQUARE (0.36f)
#define RANSAC_ITER_NUM (50)
#define RANSAC_ERROR_THRES (1.f)



#define AY_VERBOSE 1

void get_sift(string filename, vector<KeyPoint>&kps, Mat&desc,
	int numfeatures=0,
	float contour=0.04,
	float edgeLimit=10.f,
	float sigma=1.6f);


/*
To get corresponding locations of two image by using sift and force match
*/
void get_crspd(string path_ref, string path_tar, vector<Point2f>&coor_ref, vector<Point2f>&coor_tar);
int nn_match_single(Mat & desc_ref, Mat & desc_tar, vector<KeyPoint>& kp_ref, vector<KeyPoint>& kp_tar, vector<Point2f>& coor_ref, vector<Point2f>& coor_tar);
