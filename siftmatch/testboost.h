#pragma once


#define BOOST_TIMER_NO_LIB
#define BOOST_PROGRESS_NO_LIB
#define BOOST_FILESYSTEM_NO_LIB


#include<boost\timer.hpp>
#include<boost\progress.hpp>
using namespace boost;
#include<boost\filesystem.hpp>
using namespace boost::filesystem;


#include<iostream>
using namespace std;

#include<Windows.h>


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


void test_file()
{
	path dir = "./out";
	cout <<"is empty?"<< dir.empty() << endl;
}