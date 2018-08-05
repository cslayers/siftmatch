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
	//print_some_pairs(ref_coor,tar_coor);
	cout << "[+]Found " << ref_coor.size() << " correspondences by SIFT and nearest matches. " << endl;


	//need filter here
	//sift case: one keypoint with multiple orientation
	vector<Point2f> ref_coor_final;
	vector<Point2f> tar_coor_final;
	filter(ref_coor, tar_coor, ref_coor_final, tar_coor_final);
	//print_some_pairs(ref_coor_final, tar_coor_final);
	cout << "[+]Left " << ref_coor_final.size() << " correspondences after filtering. " << endl;


	//test ransac function below
	MatrixXd aff;
	vector<Point2f> ref_coor_kernel;
	vector<Point2f> tar_coor_kernel;
	ransac(ref_coor_final, tar_coor_final, ref_coor_kernel, tar_coor_kernel,aff);
	cout << "[+]Left " << ref_coor_kernel.size() << " correspondences after global ransac. " << endl;
	cout << aff << endl;



	aywait();

}


void test_run()
{
	Run_config config;
	config.gradient_order = 4;
	config.margin = 10;
	config.gridspace = 5;
	config.NEIGH_NUM = 12;
	config.subset_radius = 16;

	string imageref = "./testdata/sin/Sample3Reference.tif";
	string imagetar = "./testdata/sin/sinref.tif";

	run(config,imageref,imagetar);

	aywait();
}
