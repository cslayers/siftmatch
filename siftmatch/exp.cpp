#include"exp.h"

#include<fstream>

string find_ref_file(vector<path>& files)
{
	for (auto& file : files)
	{
		if (file.filename().string().find("eference") != string::npos)
		{
			//founded
			string res = file.filename().string();
			
			//note that the ref image should not in the tar list
			//replace current one with last one ,then delete the last
			file = files[files.size() - 1];
			files.pop_back();
			
			return res;
		}
	}

	//file not found
	string res="";
	for (int i=0;i<files.size();i++)
	{
		info("(" << i+1 << ")" << files[i].filename());
	}
	askk("Which reference image file");
	int select; cin >> select; select--;
	assert(select >=0 && select < files.size());
	res = files[select].filename().string();
	files[select] = files[files.size() - 1];
	files.pop_back();
	return res;
}

void print_guess(Sift_Guess guess)
{
	assert(guess.dp0);

	cout << "[";
	cout << guess.x << ",";
	cout << guess.y << "] ";
	for(int i=0;i<6;i++)
		cout << guess.dp0[i] << " ";
	cout << endl;
}

void write_result(Run_result result, string outpath)
{
	assert(result.data);
	assert(result.numberx > 0);
	assert(result.numbery > 0);
	assert(result.data[0].dp0);
	assert(outpath.length() > 0);
	
#define sep " "

	std::ofstream out;
	out.open(outpath.c_str());
	if (!out.is_open())
	{
		cerr << "In " << __FUNCTION__ << "()" << endl;
		cerr << "Cannot open file: " << outpath << endl;
	}
	else
	{
		out.unsetf(std::ios::floatfield);
		out.precision(6);
		out.setf(std::ios::fixed, std::ios::floatfield);

		int totalnumber = result.numberx*result.numbery;
		for (int i = 0; i < totalnumber; i++)
		{
			Sift_Guess guess = result.data[i];
			out << guess.x << sep;
			out << guess.y << sep;
			for (int i = 0; i < 6; i++)
			{
				out << guess.dp0[i] << sep;
			}
			out << endl;
		}
	}
	out.close();
	
#undef sep

	return;
}


void print_run_result(Run_result result)
{
	int num = result.numberx*result.numbery;
	assert((num > 0));
	assert(result.data);

	for (int i = 0; i < num; i++)
	{
		Sift_Guess guess = result.data[i];
		print_guess(guess);
	}
}


void registration(path refpath,path tarpath,Run_config config,Run_result& result)
{
	string imageref = refpath.string();
	string imagetar = tarpath.string();

	vector<Point2f> coor_ref_init;
	vector<Point2f> coor_tar_init;
	get_crspd(imageref, imagetar, coor_ref_init, coor_tar_init, config.aysift_con);

	vector<Point2f> coor_ref;
	vector<Point2f> coor_tar;
	filter(coor_ref_init, coor_tar_init, coor_ref, coor_tar);

	Mat source = Mat(coor_ref).reshape(1);
	source.convertTo(source, CV_32F);
	flann::KDTreeIndexParams indexParam(2);
	flann::Index kdtree(source, indexParam);

	//set configuration
	int gridspacex = config.gridspace;
	int gridspacey = config.gridspace;
	int marginx = config.margin;
	int marginy = config.margin;
	int gradientOrder = config.gradient_order;
	int subset_radiusx = config.subset_radius;
	int subset_radiusy = config.subset_radius;

	//read some information about ref
	Mat ref = imread(imageref);
	int width = ref.cols;
	int height = ref.rows;

	//calculate poi matrix
	int subsetx = subset_radiusx * 2 + 1;
	int subsety = subset_radiusy * 2 + 1;
	int roiWidth = width - 2 * gradientOrder;
	int roiHeight = height - 2 * gradientOrder;
	int numberx = (roiWidth - 2 * subset_radiusx - 2 * marginx) / gridspacex + 1;
	int numbery = (roiHeight - 2 * subset_radiusy - 2 * marginy) / gridspacey + 1;
	int totalnumber = numberx*numbery;


	result.numberx = numberx;
	result.numbery = numbery;
	result.data = (Sift_Guess*)malloc(sizeof(Sift_Guess)*result.numberx*result.numbery);


	vector<float> query;
	for (int i = 0; i < totalnumber; i++)
	{
		int xx = i % numberx;
		int yy = i / numberx;
		int x = gradientOrder + marginx + subset_radiusx + xx*gridspacex;
		int y = gradientOrder + marginy + subset_radiusy + yy*gridspacey;
		query.push_back((float)x);
		query.push_back((float)y);
	}
	Mat querys(query);
	querys = querys.reshape(1, totalnumber);

	int NEIGH_NUM = config.NEIGH_NUM;
	Mat indices(totalnumber, NEIGH_NUM, CV_8SC1);
	Mat dists(totalnumber, NEIGH_NUM, CV_32FC1);
	kdtree.knnSearch(querys, indices, dists, NEIGH_NUM);

	for (int i = 0; i < totalnumber; i++)
	{

		Point2f poi(query[2 * i], query[2 * i + 1]);


		vector<Point2f> icoor1;
		vector<Point2f> icoor2;
		for (int j = 0; j < NEIGH_NUM; j++)
		{
			int globalidx = indices.at<int>(i, j);
			Point2f tmp = coor_ref[globalidx];
			Point2f tmp2 = coor_tar[globalidx];
			tmp -= poi;
			tmp2 -= poi;
			icoor1.push_back(tmp);
			icoor2.push_back(tmp2);
		}


		vector<Point2f> icoor1_f;
		vector<Point2f> icoor2_f;
		MatrixXd affine;
		ransac(icoor1, icoor2, icoor1_f, icoor2_f, affine);


		float ux = (float)affine(0, 0) - 1;
		float uy = (float)affine(1, 0);

		float vx = (float)affine(0, 1);
		float vy = (float)affine(1, 1) - 1;

		float u = (float)affine(2, 0);
		float v = (float)affine(2, 1);



		// store guess
		Sift_Guess guess;
		guess.x = poi.x;
		guess.y = poi.y;
		guess.dp0[0] = u;
		guess.dp0[1] = ux;
		guess.dp0[2] = uy;
		guess.dp0[3] = v;
		guess.dp0[4] = vx;
		guess.dp0[5] = vy;

		result.data[i] = guess;
	}

}

void common_exp(string dir,string ref_file,string outputdir)
{
	set<string> ext_list = { ".tif",".tiff",".jpg",".png",".bmp" };
	path workdir(dir);
	
	path ref_filep(dir + "\\" + ref_file);
	cout << ref_filep << endl;
	if(	
		(!is_regular_file(ref_filep))	||
		(ext_list.count(ref_filep.extension().string()) < 0 ))
	{
		ref_file = "";
		badd("Wrong ref file name");
		info("selecting ref file: ");
	}

	/*assert(
		(is_regular_file(ref_filep) &&
		(ext_list.count(ref_filep.extension().string()) > 0))
		|| ((cout << "file not exist: " << ref_filep.string() << endl) ? 0 : 0)
	);*/

	if (is_directory(dir))
	{
		workdir = system_complete(workdir);
		vector<path> tarfilenames;
		for (auto&&pos : directory_iterator(workdir))
		{
			if(pos.path().filename()!=ref_file&&
				ext_list.count(pos.path().extension().string())>0)
				tarfilenames.push_back(pos.path());
		}
		ref_file = ( ref_file == "") ? find_ref_file(tarfilenames) : ref_file;

		if (exp_verbose)
		{
			info("REF FILE: " << workdir.string()+"\\"+ref_file);
			info("tar files:");
			for (auto&tar : tarfilenames)
			{
				info("\t" << tar.string());
			}
		}


		path ref_path(workdir.string()+"\\"+ref_file);
		Run_config config;
		for (auto&tarpath : tarfilenames)
		{
		
			Run_result result;
			string outfilename;
			outfilename.append(outputdir);
			outfilename.append("\\");
			outfilename.append(ref_file);
			outfilename.append(tarpath.filename().string());
			outfilename.append(".txt");
			
			registration(ref_path, tarpath, config, result);
			path outdir(outputdir);
			if (!is_directory(outdir))
			{
				badd("Path "<<outputdir<<" do not exists");
				info("creating output dirctory: "<<outputdir);
				try
				{
					create_directory(outdir);
				}
				catch (const std::exception& e)
				{
					badd(e.what());
				}
				assert(is_directory(outdir));
			}
			write_result(result, outfilename);
			
			good(tarpath.string() << " done");
			infoinline("\t");
			good("Write to: " << outfilename);

		}
	}
	else
	{
		print_negative;
		cout << "Directory " << dir << " not found!" << endl;
		print_linetive;
	}
}


void sift_time(path workdir)
{
	set<string> ext_list = { ".tif",".tiff",".jpg",".png",".bmp" };


	if (is_directory(workdir))
	{
		workdir = system_complete(workdir);


		vector<path> tarfilenames;


		//statistic
		double totaltime = 0;
		int totalfeatures=0;
		double totaldensity = 0;
		int num_files = 0;



		for (auto&&pos : recursive_directory_iterator(workdir))
		{
			if (is_regular_file(pos))
			{
				if(ext_list.count(pos.path().extension().string()))
				{
					string fileps= pos.path().string();
					vector<KeyPoint> kps;
					Mat desc;
					Mat image = imread(fileps.c_str(), 1);
					SIFT sift;
					
					


					timer t;
					sift.detect(image, kps);
					sift.compute(image, kps, desc);

					unsigned int pixels = image.size().width*image.size().height;
					double featuredensity = double(kps.size()) / double(pixels) * 100;



					//view
					infoinline(t.elapsed());
					infoinline(kps.size());
					infoinline(featuredensity);
					infoinline(image.size());
					infoinline(pixels);
					infoinline(pos.path().filename().string());
					info(pos.path().parent_path().filename().string());

					//statistic
					totaltime += t.elapsed();
					totalfeatures += kps.size();
					totaldensity += featuredensity;
					num_files++;
				}
			}
		}

		assert(num_files != 0);
		
		totaltime /= num_files;
		totalfeatures /= num_files;
		totaldensity /= num_files;

		good("time " << totaltime);
		good("features " << totalfeatures);
		good("density "<<totaldensity);
	}
}