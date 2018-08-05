#include"aysift.h"
#include<omp.h>



void get_sift(string filename, vector<KeyPoint>&kps, Mat&desc, int numfeatures,float contr,float edge,float sigma)
{
	double time_init = omp_get_wtime();

	Mat gray=imread(filename,1);
	kps.resize(100000);
	
	SIFT sift(numfeatures, 3, contr, edge, sigma);
	

	//SIFT start
	double time_start = omp_get_wtime();
	
	sift.detect(gray, kps);
	double time_detect_end = omp_get_wtime();

	sift.compute(gray, kps, desc);
	double time_gen_end = omp_get_wtime();



	//print some infomation
	if (AY_VERBOSE)
	{
		cout << filename << " ";
		cout << "[features num:" << kps.size() << "]";
		cout << " [preparetime: " << time_start - time_init << "s]";
		cout << " [sifttime: " << time_gen_end - time_start << "s]";
		cout << " [detect: " << time_detect_end - time_start << "s]";
		cout << " [gen: " << time_gen_end - time_detect_end<<"s]";
		cout << endl;
	}
}