#pragma once
#include"aysift.h"


#include<boost\timer.hpp>
#include<boost\progress.hpp>
using namespace boost;
#include<boost\filesystem.hpp>
using namespace boost::filesystem;


#include<iostream>
#include<set>
using namespace std;



Point2f get_resolution(string imagepath)
{
	Mat m = imread(imagepath, 1);
	Point2f res;
	res.x = m.size().width;
	res.y = m.size().height;
	return res;
}

int feature_num(string imagepath)
{
	Aysift_config con;
	Mat desc;
	vector<KeyPoint> kps;
	get_sift(imagepath, kps, desc, con);
	return kps.size();
}

void test_file()
{

	/*
		to test features number of all image files in the directory. 
	*/

	set<string> ext_list={".tif",".tiff",".jpg",".png",".bmp"};
	path dir = "testdata";
	if (dir.is_relative())
	//	dir = system_complete(dir);
	;


	if (is_regular_file(dir))
	{
		string ext = dir.extension().string();
		if (ext_list.count(ext))
			cout << "[" << feature_num(dir.string()) << "] " << dir.string() << endl;
		else
			cout << dir << "\t\n is not image file" << endl;
		return;
	}
	else if(!is_directory(dir))
	{
		cout << dir.filename() << ": " << "Neither File or DIR" << endl;
		return;
	}



	//recursively 
	stack<path> q;
	q.push(dir);

	directory_iterator end;
	while (!q.empty())
	{
		path cur = q.top();	q.pop();
		vector<int> features;
		Point2f resolu;

		for (directory_iterator pos(cur); pos != end; ++pos)
		{

			path ptmp = *pos;
			cout << ptmp.string() << endl;
			if (is_directory(ptmp))
			{
				q.push(ptmp);
			}
			else if (is_regular_file(ptmp))
			{
				string ext = ptmp.extension().string();
				if (ext_list.count(ext))
				{
					int count = feature_num(ptmp.string());
					resolu = get_resolution(ptmp.string());
					cout << "[" << count << "] ";
					cout << ptmp.string() << endl;
					features.push_back(count);
				}
			}
		}

		int avg = 0;
		for (auto i : features)
			avg += i;

		if (features.size() > 0)
			avg /= features.size();
		else
			continue;


		cout << "[" << avg << "] features averaged" << "\t[DIR]: ";
		cout << cur.string() << endl;
		cout << "resolution: " << resolu << "\t";
		cout << "density: " << 100*avg / (resolu.x*resolu.y) << endl;
		cout << endl << endl;
	}



}

vector<string> find_file(string dir, string target)
{
	path p(dir);
	if (!is_directory(p))
	{
		cout << "can not open " << p.string() << endl;
		return vector<string>();
	}

	recursive_directory_iterator end;
	for (recursive_directory_iterator pos(dir); pos != end; pos++)
	{
		if (is_regular_file(*pos))
		{
			string tmp = (*pos).path().string();
			if (tmp.find(target) !=string::npos)
			{
				cout << pos->path().filename() << "\t" << pos->path().generic_path() << endl;
			}

		}
	}

	return vector<string>();

}

void boost_timer()
{

	progress_timer pt;

	timer t;

	Sleep(1000);
	cout << t.elapsed() << endl;

	t.restart();
	cout <<"elapsed_max(): "<< t.elapsed_max() << endl;
	cout <<"elapsed min(): "<<t.elapsed_min() << endl;
	cout << t.elapsed() << endl;
	
}

void boost_progress_display()
{
	vector<string> v(100);
	std::ofstream fs("out");

	progress_display pd(v.size());
	for (int i = 0; i < 100; i++)
	{
		++pd;
		Sleep(100);
	}
}



