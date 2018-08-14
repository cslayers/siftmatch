#pragma once
#include"aysift.h"
#include<boost\filesystem.hpp>
using namespace boost;
using namespace boost::filesystem;


#include<iostream>
#include<set>
using namespace std;


#define print_negative cout<<"[-]"
#define print_positive cout<<"[+]"
#define print_infotive cout<<"[#]"
#define print_qmarkive cout<<"[?]"
#define print_linetive cout<<endl

#define infoinline(x) print_infotive; cout<<x<<" "
#define info(x) print_infotive; cout<<x<<" "; print_linetive
#define good(x) print_positive; cout<<x<<" "; print_linetive
#define badd(x) print_negative; cout<<x<<" "; print_linetive
#define askk(x) print_qmarkive; cout<<x<<"?"; print_linetive


#define exp_verbose 1


void common_exp(string dir, string reference,string outputdir);//run exp in a dir, need specify the reference image,and a output dir

void registration(path refpath, path tarpath, Run_config config, Run_result& result);// get guess using sift

void print_guess(Sift_Guess guess);

void write_result(Run_result result,string outpath);

void print_run_result(Run_result result);
