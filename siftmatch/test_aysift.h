#pragma once
#include"aysift.h"
#include"util.h"


void test_aysift()
{
	string imagepath="./testdata/Sample1/trxy_s2_00.tif";
	Mat desc;
	vector<KeyPoint> kps;
	get_sift(imagepath,kps,desc);
	aywait();
}
