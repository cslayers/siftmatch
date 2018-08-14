#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/nonfree/features2d.hpp>
#include<opencv2/nonfree/nonfree.hpp>

using namespace cv;

#include<cstdio>
#include<vector>
#include<iostream>
using namespace std;
void aywait();

void print_some_pairs(vector<Point2f>& ref_coor, vector<Point2f>& tar_coor, int num = 10);

void print_matrix(Mat& mat);



typedef struct
{
	int a;
	int b;
}S;//compile error without initialize


typedef struct
{
	int a=5;
	int b;
}St;


typedef struct
{
	int a=1;
	int b=2;
}Sti;

void cppexp();
