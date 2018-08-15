#pragma once
#include"aysift.h"
#include"util.h"
#include"exp.h"

void test_aysift()
{
	Aysift_config con;
	string imagepath="./testdata/Sample1/trxy_s2_00.tif";
	Mat desc;
	vector<KeyPoint> kps;
	get_sift(imagepath,kps,desc,con);
	aywait();
}



void test_get_corr()
{
	string imageref = "./testdata/Sample1/trxy_s2_00.tif";
	string imagetar = "./testdata/Sample1/trxy_s2_01.tif";


	vector<Point2f> ref_coor;
	vector<Point2f> tar_coor;
	Aysift_config con;
	get_crspd(imageref, imagetar, ref_coor, tar_coor,con);
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
	Aysift_config aysift_con;
	aysift_con.max_feature_num = 0;
	aysift_con.edge_limit = 10.0f;
	aysift_con.sigma = 1.6f;
	aysift_con.contour = 0.04f;
	aysift_con.kp_layer = 3;

	Run_config config;
	config.aysift_con = aysift_con;
	config.gradient_order = 4;
	config.margin = 10;
	config.gridspace = 5;
	config.NEIGH_NUM = 12;
	config.subset_radius = 16;

//	string imageref = "./testdata/sin/Sample3Reference.tif";
//	string imagetar = "./testdata/sin/sinref.tif";
	
	string imageref = "./testdata/Sample14/Sample14 Reference.tif";
	string imagetar = "./testdata/Sample14/Sample 14 L3 Amp0.1.tif";
	string path = "./out/tmp.txt";
	

	vector<Point2f> result;
	Point2f numberxy;

	numberxy=run(config,imageref,imagetar,result);



	cout << result.size() << endl;
	cout << numberxy << endl;
	
	aywait();
}


void test_registration()
{
	string ref = "C:\\Users\\cslay\\Desktop\\github\\siftmatch\\siftmatch\\testdata\\Sample3\\Sample3 Reference.tif";
	string tar = "C:\\Users\\cslay\\Desktop\\github\\siftmatch\\siftmatch\\testdata\\Sample3\\Sample3-005 X0.50 Y0.50 N2 C0 R0.tif";
	path refpath(ref);
	path tarpath(tar);
//	void registration(path refpath, path tarpath, Run_config config, Run_result result)

	Run_config config;
	Run_result result;
	registration(refpath,tarpath,config,result);


	cout << result.numberx << endl;
	cout << result.numbery << endl;
	
	write_result(result, "./out/testwrite.txt");


	if (result.data)
		free(result.data);
}


void test_common_exp()
{
	/*string dir = "testdata/sample14";
	string reffile = "Sample14 Reference.tif";
	string outputdir = "outtest";*/

	string dir = "testdata\\sample3";
	string reffile = "";
	string outputdir = "out";
	common_exp(dir, reffile, outputdir);


}