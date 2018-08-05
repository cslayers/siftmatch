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


void print_some_pairs(vector<Point2f> ref_coor, vector<Point2f> tar_coor, int num = 10)
{
	cout << "head" << endl;
	cout << "---------------------------------------------------------------" << endl;
	for (int i = 0; i < num; i++)
	{
		if (i < ref_coor.size())
		{
			cout << ref_coor[i] << " <-> " << tar_coor[i] << endl;
		}
	}
	cout << endl;

	cout << "tail" << endl;
	cout << "---------------------------------------------------------------" << endl;
	for (int i = ref_coor.size()-num; i < ref_coor.size(); i++)
	{
		if (i >= 0 &&  i < ref_coor.size())
		{
			cout << ref_coor[i] << " <-> " << tar_coor[i] << endl;
		}
	}


	cout << endl;
	cout << "---------------------------------------------------------------" << endl<<endl;

}

void test_get_corr()
{
	string imageref = "./testdata/Sample1/trxy_s2_00.tif";
	string imagetar = "./testdata/Sample1/trxy_s2_01.tif";


	vector<Point2f> ref_coor;
	vector<Point2f> tar_coor;
	get_crspd(imageref, imagetar, ref_coor, tar_coor);
	//print_some_pairs(ref_coor,tar_coor);
	cout << "[+]Found " << ref_coor.size() << " correspondences by SIFT and nearest matches. " << endl;


	//need filter here
	//sift case: one keypoint with multiple orientation
	vector<Point2f> ref_coor_final;
	vector<Point2f> tar_coor_final;
	filter(ref_coor, tar_coor, ref_coor_final, tar_coor_final);
	//print_some_pairs(ref_coor_final, tar_coor_final);
	cout << "[+]Left " << ref_coor_final.size() << " correspondences after filtering. " << endl;


	aywait();

}