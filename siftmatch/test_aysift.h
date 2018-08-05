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


void test_get_corr()
{
	string imageref = "./testdata/Sample1/trxy_s2_00.tif";
	string imagetar = "./testdata/Sample1/trxy_s2_01.tif";

	vector<Point2f> ref_coor;
	vector<Point2f> tar_coor;

	get_crspd(imageref, imagetar, ref_coor, tar_coor);
	aywait();

}