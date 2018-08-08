#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/nonfree/features2d.hpp>
#include<opencv2/nonfree/nonfree.hpp>
using namespace cv;

#include<iostream>
#include<string>
#include<fstream>
using namespace std;

#include<Eigen/Dense>
using namespace Eigen;

#define DESC_LENGTH (128)
#define MATCH_THRE_SQUARE (0.36f)
#define RANSAC_ITER_NUM (50)
#define RANSAC_ERROR_THRES (1.f)



#define AY_VERBOSE 1
#define AY_USE_LOCAL_COO

typedef struct
{
	int max_feature_num=0;
	int kp_layer=3;
	float contour=0.04f;
	float edge_limit=10.0f;
	float sigma=1.6f;
}Aysift_config;//see opencv sift

typedef struct
{
	Aysift_config aysift_con;
	int NEIGH_NUM=12;
	int gridspace=5;
	int gradient_order=4;
	int margin=10;
	int subset_radius=16;
}Run_config;//using square subset, same order,gridspace, and margin

void get_sift(string filename, vector<KeyPoint>&kps, Mat&desc,
	Aysift_config con);


/*
To get corresponding locations of two image by using sift and force match
*/
void get_crspd(string path_ref, string path_tar, vector<Point2f>&coor_ref, vector<Point2f>&coor_tar,Aysift_config con);

int nn_match_single(Mat & desc_ref, Mat & desc_tar, vector<KeyPoint>& kp_ref, 
	vector<KeyPoint>& kp_tar, vector<Point2f>& coor_ref, vector<Point2f>& coor_tar);

void write_crspd(vector<Point2f>&coor_ref, vector<Point2f>&coor_tar, string path);

void read_crspd(vector<Point2f>&coor_ref, vector<Point2f>&coor_tar, string path);

void filter(vector<Point2f>&coor_ref, vector<Point2f>&coor_tar, 
	vector<Point2f>&coor_ref_filtered, vector<Point2f>&coor_tar_filtered);

int ransac(vector<Point2f>&coor_ref, vector<Point2f>& coor_tar, 
	vector<Point2f>&coor_ref_final, vector<Point2f>& coor_tar_final, MatrixXd& result);

Point2f run(Run_config config,string ref,string tar, vector<Point2f>& result);

