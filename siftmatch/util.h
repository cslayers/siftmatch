#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<nonfree/features2d.hpp>
#include<nonfree/nonfree.hpp>
using namespace cv;

#include<vector>
#include<iostream>
using namespace std;
void aywait();

void print_some_pairs(vector<Point2f> ref_coor, vector<Point2f> tar_coor, int num = 10);

void print_matrix(Mat& mat);


