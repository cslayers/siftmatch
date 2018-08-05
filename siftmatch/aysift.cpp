#include"aysift.h"
#include<omp.h>
#include<iomanip>


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
		cout << " [detect: " << time_detect_end - time_start << "s]";
		cout << " [gen: " << time_gen_end - time_detect_end<<"s]";
		cout << endl;
		cout << "[t]sift time: " << time_gen_end - time_start << "s" << endl;
	}
}






float distance(Mat & desc_ref, Mat & desc_tar, int i, int j)
{
	float result = 0.0f;
	for (int t = 0; t < DESC_LENGTH; t++)
	{
		float temp = (desc_ref.at<float>(i, t) - desc_tar.at<float>(j, t));
		result += temp*temp;
	}
	return result;
}

int nn_match_single(Mat & desc_ref, Mat & desc_tar, vector<KeyPoint>& kp_ref, vector<KeyPoint>& kp_tar, vector<Point2f>& coor_ref, vector<Point2f>& coor_tar)
{
	int ref_num = desc_ref.rows;
	int tar_num = desc_tar.rows;
#pragma omp parallel for
	for (int i = 0; i < ref_num; i++)
	{
		int minIdx = 0;
		float minDis = FLT_MAX;

		int second = 0;
		float secondDis = FLT_MAX;
		for (int j = 0; j < tar_num; j++)
		{
			float temp = distance(desc_ref, desc_tar, i, j);
			if (temp < minDis)
			{
				second = minIdx;
				secondDis = minDis;

				minIdx = j;
				minDis = temp;
			}
			else if (temp<secondDis)
			{
				second = j;
				secondDis = temp;
			}
		}
		if (minDis / secondDis < MATCH_THRE_SQUARE)
		{

			#pragma omp critical
			{
				coor_ref.push_back(kp_ref[i].pt);
				coor_tar.push_back(kp_tar[minIdx].pt);
			}
		}
	}
	return 0;
}

void get_crspd(string path_ref, string path_tar, vector<Point2f>& coor_ref, vector<Point2f>& coor_tar)
{
	double time_start = omp_get_wtime();

	Mat desc_ref;
	vector<KeyPoint> kps_ref;
	get_sift(path_ref, kps_ref, desc_ref);


	Mat desc_tar;
	vector<KeyPoint> kps_tar;
	get_sift(path_tar, kps_tar, desc_tar);




	if (AY_VERBOSE)
		cout << "Matching features...";

	double time_match_start = omp_get_wtime();
	nn_match_single(desc_ref, desc_tar, kps_ref, kps_tar, coor_ref, coor_tar);


	double time_end = omp_get_wtime();

	if (AY_VERBOSE)
	{
		cout << "   Done" << endl;
		cout << "ref kp num: " << kps_ref.size() << endl;
		cout << "tar kp num: " << kps_tar.size() << endl;
		cout << "match sucess: " << coor_ref.size() << " ";
		cout << "match rate: " << coor_ref.size() / (kps_ref.size() + 0.0001f) << endl;

		cout << "[t]time for match: " << time_end - time_match_start << "s" << endl;
		cout << "[t]total time for get_coor: " << time_end - time_start << "s" << endl;

	}

}

void filter(vector<Point2f>&coor_ref, vector<Point2f>&coor_tar, vector<Point2f>&coor_ref_filter, vector<Point2f>&coor_tar_filter)
{
	double time_start = omp_get_wtime();

	int num = coor_ref.size();
//#pragma omp parallel for
	for (int i = 0; i < num; i++)
	{

		//special
		int  spe = 1;

		for (int j = i - 1; j >= 0; j--)			
		//for (int j = 0; j < i; j++)
		{
			if (
				(coor_ref[i].x == coor_ref[j].x) &&
				(coor_ref[i].y == coor_ref[j].y)
				)
			{
				spe = -1;
				break;
			}
		}

		#pragma omp critical
		if (spe>0)
		{
			coor_ref_filter.push_back(coor_ref[i]);
			coor_tar_filter.push_back(coor_tar[i]);
		}
	}//for end

	double time_end = omp_get_wtime();
	cout << "[t]location filter time: " << time_end - time_start << "s " << endl;
}//function end





void write_crspd(vector<Point2f>&coor_ref, vector<Point2f>&coor_tar, string path)
{
	ofstream out;
	out.open(path);

	for (int i = 0; i < coor_ref.size(); i++)
	{
		out << fixed << setprecision(6);
		out << coor_ref[i].x << " " << coor_ref[i].y << " ";
		out << coor_tar[i].x << " " << coor_tar[i].y << " ";
		out << endl;
	}
	out.close();
}

void read_crspd(vector<Point2f>&coor_ref, vector<Point2f>&coor_tar, string path)
{
	ifstream in;
	in.open(path);
	float tmp = 0.0;
	float tmp1 = 0.0;
	while (in >> tmp)
	{
		in >> tmp1;
		coor_ref.push_back(Point2f(tmp, tmp1));

		in >> tmp;
		in >> tmp1;
		coor_tar.push_back(Point2f(tmp, tmp1));
	}
	in.close();
}


